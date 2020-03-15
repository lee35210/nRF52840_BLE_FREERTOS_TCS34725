#include <stdio.h>
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_gpio.h"
#include "tcs34725.h"

ret_code_t tcs34725_write_reg(tcs34725_instance_t const * p_instance,
                              tcs34725_reg_data_t *       p_reg_data)
{
    return nrf_twi_sensor_reg_write(p_instance->p_sensor_data,
                                    p_instance->sensor_addr,
                                    p_reg_data->reg_addr|0x80,
                                    (uint8_t *)&p_reg_data->reg_data,
                                    TCS34725_REGISTER_SIZE);
}

ret_code_t tcs34725_read_reg(tcs34725_instance_t const * p_instance,
                              tcs34725_reg_data_t *      p_reg_data,
                              tcs34725_data_callback_t   user_cb)
{
    return nrf_twi_sensor_reg_read(p_instance->p_sensor_data,
                                   p_instance->sensor_addr,
                                   p_reg_data->reg_addr|0x80,
                                   (nrf_twi_sensor_reg_cb_t) user_cb,
                                   (uint8_t *) p_reg_data,
                                   TCS34725_REGISTER_SIZE);
}

ret_code_t tcs34725_init(tcs34725_instance_t const * p_instance)
{
    //int enable, wait enable, rgbc enable, power on/off
    ret_code_t err_code;
    tcs34725_reg_data_t enable_reg;

    tcs34725_config_enable_t init_config;
    init_config.rgbc_interrupt=false;   //bit4
    init_config.wait_enable=true;       //bit3
    init_config.rgbc_enable=true;       //bit1
    init_config.power_on=true;          //bit0

//    err_code=tcs34725_set_init(p_instance, &init_config);
    enable_reg.reg_addr=TCS34725_REG_ENABLE;
    enable_reg.reg_data=(init_config.rgbc_interrupt << TCS34725_INT_POS)|
                        (init_config.wait_enable << TCS34725_WAIT_POS)|
                        (init_config.rgbc_enable << TCS34725_RGBC_ENABLE_POS)|
                        (init_config.power_on << TCS34725_POWER_ON_POS);

    err_code=tcs34725_write_reg(p_instance, &enable_reg);
    return err_code;
}

void tcs34725_read_all_config(tcs34725_instance_t const * p_instance, tcs34725_data_callback_t user_cb)
{
    static tcs34725_reg_data_t enable,timing,waittime,persistence,config,control,id,status;

    enable.reg_addr=TCS34725_REG_ENABLE;
    tcs34725_read_reg(p_instance, &enable, user_cb);

    timing.reg_addr=TCS34725_REG_TIMING;
    tcs34725_read_reg(p_instance, &timing, user_cb);

    waittime.reg_addr=TCS34725_REG_WAIT_TIME;
    tcs34725_read_reg(p_instance, &waittime, user_cb);

    persistence.reg_addr=TCS34725_REG_PERSISTENCE;
    tcs34725_read_reg(p_instance, &persistence, user_cb);

    config.reg_addr=TCS34725_REG_CONFIG;
    tcs34725_read_reg(p_instance, &config, user_cb);

    control.reg_addr=TCS34725_REG_CONTROL;
    tcs34725_read_reg(p_instance, &control, user_cb);

    id.reg_addr=TCS34725_REG_ID;
    tcs34725_read_reg(p_instance, &id, user_cb);

    status.reg_addr=TCS34725_REG_STATUS;
    tcs34725_read_reg(p_instance, &status, user_cb);
}

ret_code_t tcs34725_set_timing(tcs34725_instance_t const * p_instance,
                                uint16_t atime)
{
    ret_code_t err_code;
    if((atime==0)||(256 < atime))
    {
        err_code=NRF_ERROR_INVALID_DATA;
        return err_code;
    }

    tcs34725_reg_data_t timing_str;
    timing_str.reg_addr=TCS34725_REG_TIMING;
    timing_str.reg_data=(256-atime);
    
    err_code=tcs34725_write_reg(p_instance,&timing_str);
    return err_code;
}

ret_code_t tcs34725_set_wait_time(tcs34725_instance_t const * p_instance,
                                  uint8_t wait_val)
{
    ret_code_t err_code;
    if((wait_val==0)||(256 < wait_val))
    {
        err_code=NRF_ERROR_INVALID_DATA;
        return err_code;
    }

    tcs34725_reg_data_t wait_time_str;
    wait_time_str.reg_addr=TCS34725_REG_WAIT_TIME;
    wait_time_str.reg_data=256-wait_val;
    
    err_code=tcs34725_write_reg(p_instance,&wait_time_str);
    return err_code;
}


ret_code_t tcs34725_set_persistence(tcs34725_instance_t const * p_instance,
                                    tcs34725_persistence_t out_of_range_val)
{
    ret_code_t err_code;
    tcs34725_reg_data_t persistence;
    persistence.reg_addr=TCS34725_REG_PERSISTENCE;
    persistence.reg_data=out_of_range_val;
    err_code=tcs34725_write_reg(p_instance, &persistence);
    return err_code;
}

ret_code_t tcs34725_set_wait_long(tcs34725_instance_t const * p_instance,
                                   tcs34725_wait_long_t wait_long_val)
{
    ret_code_t err_code;
    tcs34725_reg_data_t wait_long;
    wait_long.reg_addr=TCS34725_REG_WAIT_TIME;
    wait_long.reg_data=wait_long_val << TCS34725_WAIT_LONG_POS;
    err_code=tcs34725_write_reg(p_instance, &wait_long);
    return err_code;
}

ret_code_t tcs34725_set_gain(tcs34725_instance_t const * p_instance,
                                 tcs34725_gain_t gain_val)
{
    ret_code_t err_code;
    tcs34725_reg_data_t gain;
    gain.reg_addr=TCS34725_REG_CONTROL;
    gain.reg_data=gain_val;
    err_code=tcs34725_write_reg(p_instance, &gain);
    return err_code;
}

ret_code_t tcs34725_read_rgbc(tcs34725_instance_t const * p_instance,
                               tcs34725_color_data_t *     rgbc_str,
                               tcs34725_rgbc_callback_t    user_cb)
{
    ret_code_t err_code;
    err_code=nrf_twi_sensor_reg_read(p_instance->p_sensor_data,
                           p_instance->sensor_addr,
                           (TCS34725_REG_CLEAR|0xA0),
                           (nrf_twi_sensor_reg_cb_t) user_cb,
                           (uint8_t *) rgbc_str,
                           TCS34725_RGBC_BYTES);
    if(err_code!=NRF_SUCCESS)
    {
        NRF_LOG_INFO("Read RGBC Failed");
    }
    return err_code;
}

void tcs34725_rgbc_print(tcs34725_color_data_t * color_str)
{
    uint16_t c_red,c_green,c_blue;

    c_red=(int)((double)color_str->red/color_str->clear*255);
    c_green=(int)((double)color_str->green/color_str->clear*255);
    c_blue=(int)((double)color_str->blue/color_str->clear*255);


    NRF_LOG_INFO("Clear : %d",color_str->clear);
    NRF_LOG_INFO("Red   : %d",c_red);
    NRF_LOG_INFO("Green : %d",c_green);
    NRF_LOG_INFO("Blue  : %d",c_blue);
}

ret_code_t tcs34725_set_threshold(tcs34725_instance_t const * p_instance,
                                  tcs34725_threshold_lh_t threshold_low_high,
                                  uint16_t threshold_val)
{
    ret_code_t err_code;
    tcs34725_threshold_data_t threshold_str;
    threshold_str.threshold_data=threshold_val;

    if(threshold_low_high==TCS34725_THRESHOLD_LOW)
    {
        threshold_str.reg_addr=TCS34725_REG_THRESHOLD_LOW_L;
    }
    else if(threshold_low_high==TCS34725_THRESHOLD_HIGH)
    {
        threshold_str.reg_addr=TCS34725_REG_THRESHOLD_HIGH_L;
    }
    else
    {
        err_code=NRF_ERROR_INVALID_ADDR;
        return err_code;
    }

    NRF_LOG_INFO("Set threshold value : %d",threshold_str.threshold_data);
    err_code=nrf_twi_sensor_reg_write(p_instance->p_sensor_data,
                                      p_instance->sensor_addr,
                                      threshold_str.reg_addr|0xA0,
                                      (uint8_t *)&threshold_str,
                                      TCS34725_THRESHOLD_BYTES);
    return err_code;
}

ret_code_t tcs34725_read_threshold(tcs34725_instance_t const * p_instance, 
                                   tcs34725_threshold_lh_t thr_low_high,
                                   tcs34725_threshold_callback_t user_cb)
{
    ret_code_t err_code;
    static tcs34725_threshold_data_t thr_data_str;

    if(thr_low_high==TCS34725_THRESHOLD_LOW)
    {
        thr_data_str.reg_addr=TCS34725_REG_THRESHOLD_LOW_L;
    }
    else if(thr_low_high==TCS34725_THRESHOLD_HIGH)
    {
        thr_data_str.reg_addr=TCS34725_REG_THRESHOLD_HIGH_L;
    }
    else
    {
        err_code=NRF_ERROR_INVALID_ADDR;
        return err_code;
    }

    err_code=nrf_twi_sensor_reg_read(p_instance->p_sensor_data,
                                     p_instance->sensor_addr,
                                     thr_data_str.reg_addr|0xA0,
                                     (nrf_twi_sensor_reg_cb_t) user_cb,
                                     (uint8_t *) &thr_data_str,
                                     TCS34725_THRESHOLD_BYTES);
    return err_code;
}

ret_code_t tcs34725_int_clear(tcs34725_instance_t const * p_instance)
{
    ret_code_t err_code;
    uint8_t interrupt_cmd=0x66|0x80;    //special function+cmd 1
    err_code=nrf_twi_sensor_write(p_instance->p_sensor_data, p_instance->sensor_addr, &(interrupt_cmd), 
                         TCS34725_REGISTER_SIZE, true);
    return err_code;
}

ret_code_t tcs34725_set_interrupt(tcs34725_instance_t const * p_instance,
                                  tcs34725_int_enable_t int_enable)
{
    ret_code_t err_code;
    tcs34725_reg_data_t enable_reg_str;
    enable_reg_str.reg_addr=TCS34725_REG_ENABLE;
    tcs34725_read_reg(p_instance,&enable_reg_str,NULL);
    
    do
    {
        nrf_delay_us(10);
    }while(nrf_twi_mngr_is_idle(p_instance->p_sensor_data->p_twi_mngr)!=true);
    

    if(int_enable==TCS34725_INTERRUPT_ENABLE)
    {
        enable_reg_str.reg_data=(enable_reg_str.reg_data|(TCS34725_INT_MASK));
    }
    else if(int_enable==TCS34725_INTERRUPT_DISABLE)
    {
        enable_reg_str.reg_data=(enable_reg_str.reg_data&~(TCS34725_INT_MASK));
    }
    else
    {
        err_code=NRF_ERROR_INVALID_PARAM;
        return err_code;
    }
    tcs34725_write_reg(p_instance, &enable_reg_str);
}