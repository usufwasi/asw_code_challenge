#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>

typedef enum {
    STATUS_OK,
    STATUS_ERROR
} status_t;

#define CTRL_REG1   0x20
#define CTRL_REG2   0x21
#define CTRL_REG3   0x22
#define INT_CFG     0x30
#define OUT_X_L     0x28
#define OUT_X_H     0x29
#define OUT_Y_L     0x2A
#define OUT_Y_H     0x2B
#define OUT_Z_L     0x2C
#define OUT_Z_H     0x2D


status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

/*
    Get full scale gauss selection
*/
status_t get_full_scale_config(uint8_t *full_scale_config);

/*
    Get the currently set data_rate value
    data_rate -> Reference to store the data_rate
*/
status_t get_data_rate(float *data_rate);

/*
    Acceptable data_rate values are:
    0.625
    1.25
    2.5
    5
    10
    20
    40
    80
    155
    300
    560
    1000
*/
status_t set_data_rate(float data_rate);

/*
    pin_status -> 1 = Enable interrupt
    pin_status -> 0 = Disable interrupt
*/
status_t set_interrupt_pin(uint8_t pin_status);

/* 
    axis_mask -> bit0 = X Output
    axis_mask -> bit1 = Y Output
    axis_mask -> bit2 = Z Output

    axis_output -> An array of length 3 to store output data
*/
status_t get_output_data(uint8_t axis_mask, uint16_t *axis_output);

/*
    This function is used to test the functionality of 
    get_data_rate() function.
*/
status_t test_get_data_rate(float set_data_rate);

#endif