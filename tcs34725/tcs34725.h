#ifndef TCS34725_H
#define TCS34725_H

#include "nrf_twi_sensor.h"
#include "tcs34725_internal.h"

/**
 * @brief Possible sensor addresses.
 */
#define TCS34725_ADDR 0x29

// Minimum nrf_twi_sensor message buffer size and nrf_twi_mngr queue length.
#define TCS34725_MIN_QUEUE_SIZE 20

/**
 * @brief Macro creating mlx90614 sensor instance.
 *
 * @param[in] _mlx90614_inst_name    Sensor instance name.
 * @param[in] _p_twi_sensor         Pointer to common TWI sensor instance.
 * @param[in] _sensor_address       Sensor base address.
 */
#define TCS34725_INSTANCE_DEF(_tcs34725_inst_name, _p_twi_sensor, _sensor_address)                \
    TCS34725_INTERNAL_INSTANCE_DEF(_tcs34725_inst_name, _p_twi_sensor, _sensor_address)


/*
- MLX9061 transfer struct
reg_data : save data reading from mlx90614 or be written in mlx90614 register.
pec : Packet Error Code
reg_addr : MLX90614 Register Address
*/
typedef struct
{
    uint8_t reg_data;
    uint8_t reg_addr;
} tcs34725_reg_data_t;

typedef struct
{
    uint16_t clear;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} tcs34725_rgbc_data_t;

typedef struct
{
    uint16_t threshold_data;
    uint8_t reg_addr;
} tcs34725_threshold_data_t;

typedef struct{
    bool rgbc_interrupt;
    bool wait_enable;
    bool rgbc_enable;
    bool power_on;
}tcs34725_config_enable_t;

typedef struct{
    char cmd[4],data[6];
}tcs34725_cmd_t;

typedef struct{
    char send_data[9];
}tcs34725_ble_reg_t;

typedef enum
{
    TCS34725_OUT_OF_RANGE_0,
    TCS34725_OUT_OF_RANGE_1,
    TCS34725_OUT_OF_RANGE_2,
    TCS34725_OUT_OF_RANGE_3,
    TCS34725_OUT_OF_RANGE_5,
    TCS34725_OUT_OF_RANGE_10,
    TCS34725_OUT_OF_RANGE_15,
    TCS34725_OUT_OF_RANGE_20,
    TCS34725_OUT_OF_RANGE_25,
    TCS34725_OUT_OF_RANGE_30,
    TCS34725_OUT_OF_RANGE_35,
    TCS34725_OUT_OF_RANGE_40,
    TCS34725_OUT_OF_RANGE_45,
    TCS34725_OUT_OF_RANGE_50,
    TCS34725_OUT_OF_RANGE_55,
    TCS34725_OUT_OF_RANGE_60
}tcs34725_persistence_t;

typedef enum
{
    TCS34725_LIST_ENABLE=0x00,
    TCS34725_LIST_TIMING=0x01,
    TCS34725_LIST_WAIT_TIME=0x03,
    TCS34725_LIST_PERSISTENCE=0x0C,
    TCS34725_LIST_CONFIG=0x0D,
    TCS34725_LIST_CONTROL=0x0F,
    TCS34725_LIST_ID=0x12,
    TCS34725_LIST_STATUS=0x13,
}tcs34725_list_t;

typedef enum
{
    TCS34725_WAIT_LONG_DISABLE,
    TCS34725_WAIT_LONG_ENABLE
}tcs34725_wait_long_t;

typedef enum
{
    TCS34725_INTERRUPT_DISABLE,
    TCS34725_INTERRUPT_ENABLE
}tcs34725_int_enable_t;

typedef enum
{
    TCS34725_THRESHOLD_LOW,
    TCS34725_THRESHOLD_HIGH
}tcs34725_threshold_lh_t;

typedef enum
{
    TCS34725_GAIN_1x,
    TCS34725_GAIN_4x,
    TCS34725_GAIN_16x,
    TCS34725_GAIN_60x
}tcs34725_gain_t;

/**
 * @brief Data callback prototype.
 *
 * @param[in] result      Result of operation (NRF_SUCCESS on success,
 *                        otherwise a relevant error code).
 * @param[in] p_raw_data  Pointer to raw sensor data structure.
 */
typedef void (* tcs34725_data_callback_t)(ret_code_t result, tcs34725_reg_data_t * p_raw_data);

typedef void (* tcs34725_rgbc_callback_t)(ret_code_t result, tcs34725_rgbc_data_t * p_raw_data);

typedef void (* tcs34725_threshold_callback_t)(ret_code_t result, tcs34725_threshold_data_t * p_raw_data);

ret_code_t tcs34725_write_reg(tcs34725_instance_t const * p_instance,
                              tcs34725_reg_data_t *       p_reg_data);

ret_code_t tcs34725_read_reg(tcs34725_instance_t const * p_instance,
                              tcs34725_reg_data_t *      p_reg_data,
                              tcs34725_data_callback_t   user_cb);

ret_code_t tcs34725_init(tcs34725_instance_t const * p_instance);

ret_code_t tcs34725_set_timing(tcs34725_instance_t const * p_instance,
                                uint16_t atime);

ret_code_t tcs34725_set_wait_time(tcs34725_instance_t const * p_instance,
                                  uint16_t wait_val);

ret_code_t tcs34725_set_persistence(tcs34725_instance_t const * p_instance,
                                    tcs34725_persistence_t out_of_range_val);
                                    
ret_code_t tcs34725_set_enable(tcs34725_instance_t const * p_instance,
                               tcs34725_wait_long_t enable_reg_val);

ret_code_t tcs34725_set_wait_long(tcs34725_instance_t const * p_instance,
                                   tcs34725_wait_long_t wait_long_val);

ret_code_t tcs34725_set_gain(tcs34725_instance_t const * p_instance,
                                 tcs34725_gain_t gain_val);

ret_code_t tcs34725_read_rgbc(tcs34725_instance_t const * p_instance,
                               tcs34725_rgbc_data_t *     rgbc_str,
                               tcs34725_rgbc_callback_t    user_cb);

void tcs34725_rgbc_print(tcs34725_rgbc_data_t * color_str);

ret_code_t tcs34725_set_threshold(tcs34725_instance_t const * p_instance,
                                  tcs34725_threshold_lh_t threshold_low_high,
                                  uint16_t threshold_val);

ret_code_t tcs34725_read_threshold(tcs34725_instance_t const * p_instance, 
                                   tcs34725_threshold_lh_t thr_low_high,
                                   tcs34725_threshold_callback_t user_cb);

void tcs34725_read_all_config(tcs34725_instance_t const * p_instance, tcs34725_data_callback_t user_cb);

ret_code_t tcs34725_int_clear(tcs34725_instance_t const * p_instance);

ret_code_t tcs34725_set_interrupt(tcs34725_instance_t const * p_instance,
                                  tcs34725_int_enable_t int_enable);

int tcs34725_per_bintodec(tcs34725_persistence_t bit_value);
tcs34725_persistence_t tcs34725_per_dectobin(uint8_t dec_value);

#endif // TCS34725_H
