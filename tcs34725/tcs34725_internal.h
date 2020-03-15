#ifndef TCS34725_INTERNAL_H
#define TCS34725_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define TCS34725_REGISTER_SIZE 1
#define TCS34725_RGBC_BYTES 8
#define TCS34725_THRESHOLD_BYTES 2

#define TCS34725_REG_ENABLE 0x00
#define TCS34725_REG_TIMING 0x01
#define TCS34725_REG_WAIT_TIME 0x03
#define TCS34725_REG_THRESHOLD_LOW_L 0x04
#define TCS34725_REG_THRESHOLD_LOW_H 0x05
#define TCS34725_REG_THRESHOLD_HIGH_L 0x06
#define TCS34725_REG_THRESHOLD_HIGH_H 0x07
#define TCS34725_REG_PERSISTENCE 0x0C
#define TCS34725_REG_CONFIG 0x0D
#define TCS34725_REG_CONTROL 0x0F
#define TCS34725_REG_ID 0x12
#define TCS34725_REG_STATUS 0x13
#define TCS34725_REG_CLEAR 0x14
#define TCS34725_REG_CLEARH 0x15
#define TCS34725_REG_RED 0x16
#define TCS34725_REG_GREEN 0X18
#define TCS34725_REG_BLUE 0X1A

#define TCS34725_INT_POS 4
#define TCS34725_INT_MASK (1 << TCS34725_INT_POS)

#define TCS34725_WAIT_POS 3
#define TCS34725_WAIT_MASK (1 << TCS34725_WAIT_POS)

#define TCS34725_RGBC_ENABLE_POS 1
#define TCS34725_RGBC_ENABLE_MASK (1 << TCS34725_RGBC_ENABLE_POS)

#define TCS34725_POWER_ON_POS 0
#define TCS34725_POWER_ON_MASK (1 << TCS34725_PON_POS)

#define TCS34725_WAIT_LONG_POS 1
#define TCS34725_WAIT_LONG_MASK (1 << TCS34725_WAIT_LONG_POS)



/**
 * @brief Structure holding sensor instance
 */
typedef struct
{
    nrf_twi_sensor_t * const p_sensor_data;
    uint8_t const            sensor_addr;
} tcs34725_instance_t;


/**
 * @brief Macro that creates sensor instance.
 */
#define TCS34725_INTERNAL_INSTANCE_DEF(_tcs34725_inst_name, _p_twi_sensor, _sensor_address)\
    static tcs34725_instance_t _tcs34725_inst_name =                                     \
    {                                                                                    \
        .p_sensor_data = _p_twi_sensor,                                                  \
        .sensor_addr   = _sensor_address,                                                \
    }



#endif // TCS34725_INTERNAL_H