#include "i2c.h"

#include <stdint.h>
#include <stdio.h>

/* This global variable is used for testing purposes */
uint8_t g_data_rate_buffer;

status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "read [%d] bytes from bus [0x%x] for register [0x%x]\n",
        length,
        bus_address,
        register_address);

    /* Setting the output to some arbitrary value */
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = 0xA0;
    }

    return STATUS_OK;
}

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "write [%d] bytes to bus [0x%x] for register [0x%x]\n\t",
        length,
        bus_address,
        register_address);

    for (size_t i = 0; i < length; ++i) {
        printf("%p", buffer);
    }
    printf("\n");
    return STATUS_OK;
}

status_t get_full_scale_config(uint8_t *full_scale_config) {
    uint8_t bus_address = 0xAA;
    uint8_t buffer;

    // Read contents of CTRL_REG2 (0x21). The first two bits denote full-scale selection
    if(i2c_read(bus_address, CTRL_REG2, 1, &buffer)) {
        printf("Unable to read CTRL_REG2 at address: [%x]\n", CTRL_REG2);
        return STATUS_ERROR;
    }
    buffer = (buffer & 0x60) >> 5;
    
    if (buffer == 0) {
        full_scale_config = 4;
    }
    else if (buffer == 1) {
        full_scale_config = 8;
    }
    else if (buffer == 2) {
        full_scale_config = 12;
    }
    else if (buffer == 3) {
        full_scale_config = 16;
    }
    else {
        printf("Invalid full scale config read.\n");
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

status_t get_data_rate(float *data_rate) {
    uint8_t bus_address = 0xAA;
    uint8_t buffer;
    uint8_t data_rate_bits;
    uint8_t om_bits;

    // Read bit5 from CTRL_REG3
    status_t status = i2c_read(bus_address, CTRL_REG3, 1, &buffer);
    
    if (status == STATUS_OK) {
        if (buffer & 0x20) {
            *data_rate = 0.625;
        }
        else {
            status = i2c_read(bus_address, CTRL_REG1, 1, &buffer);
            buffer = g_data_rate_buffer; //Testing variable
            if (status == STATUS_OK) {
                // Check if high-performance mode is enabled.
                if (buffer & 0x02) {
                    om_bits = (buffer & 0x60) >> 5;
                    switch (om_bits)
                    {
                    case 0:
                        *data_rate = 1000;
                        break;

                    case 1:
                        *data_rate = 560;
                        break;

                    case 2:
                        *data_rate = 300;
                        break;

                    case 3:
                        *data_rate = 155;
                        break;
                    
                    default:
                        break;
                    }
                }
                else {
                    data_rate_bits = (buffer & 0x1C) >> 2;
                    switch (data_rate_bits) {
                        case 0:
                            *data_rate = 0.625;
                            break;

                        case 1:
                            *data_rate = 1.25;
                            break;

                        case 2:
                            *data_rate = 2.5;
                            break;

                        case 3:
                            *data_rate = 5;
                            break;

                        case 4:
                            *data_rate = 10;
                            break;

                        case 5:
                            *data_rate = 20;
                            break;

                        case 6:
                            *data_rate = 40;
                            break;

                        case 7:
                            *data_rate = 80;
                            break;

                        default:
                            break;
                    }
                }
            }
            else {
                printf("Unable to read CTRL_REG1 at address: [%x]\n", CTRL_REG1);
                return STATUS_ERROR;
            }
        }
    }
    else {
        printf("Unable to read CTRL_REG3 at address: [%x]\n", CTRL_REG3);
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

status_t set_data_rate(float data_rate) {
    uint8_t bus_address = 0xAA;
    uint8_t buffer;
    if (data_rate > 80) {
        if (data_rate == 1000) {
            buffer = 0x02;
        }
        else if (data_rate == 560) {
            buffer = 0x22;
        }
        else if (data_rate == 300) {
            buffer = 0x42;
        }
        else if (data_rate == 155) {
            buffer = 0x62;
        }
        else {
            printf("The data rate [%f] is invalid.\n", data_rate);
            return STATUS_ERROR;
        }

        if (i2c_write(bus_address, CTRL_REG1, 1, &buffer)) {
            printf("Unable to write data_rate to CTRL_REG1 at [%x].\n", CTRL_REG1);
            return STATUS_ERROR;
        }
    }
    else {
        if (data_rate == 0.625) {
            buffer = 0x00;
        }
        else if (data_rate == 1.25) {
            buffer = 0x04;
        }
        else if (data_rate == 2.5) {
            buffer = 0x08;
        }
        else if (data_rate == 5) {
            buffer = 0x0C;
        }
        else if (data_rate == 10) {
            buffer = 0x10;
        }
        else if (data_rate == 20) {
            buffer = 0x14;
        }
        else if (data_rate == 40) {
            buffer = 0x18;
        }
        else if (data_rate == 80) {
            buffer = 0x1C;
        }
        else {
            printf("The data rate [%f] is invalid.\n", data_rate);
            return STATUS_ERROR;
        }

        if (i2c_write(bus_address, CTRL_REG1, 1, &buffer)) {
            printf("Unable to write data_rate to CTRL_REG1 at [%x].\n", CTRL_REG1);
            return STATUS_ERROR;
        }
    }

    g_data_rate_buffer = buffer; // Testing variable
    return STATUS_OK;
}

status_t set_interrupt_pin(uint8_t pin_status) {
    uint8_t bus_address = 0xAA;
    if (i2c_write(bus_address, INT_CFG, 1, &pin_status)) {
        printf("Unable to change interrupt pin status.\n");
        return STATUS_ERROR;
    }
    else {
        printf("Interrupt pin status set to: %d\n", pin_status);
    }

    return STATUS_OK;
}

status_t get_output_data(uint8_t axis_mask, uint16_t *axis_output) {
    uint8_t bus_address = 0xAA;
    uint8_t buffer;
    if (axis_mask & 0x1) {
        if (i2c_read(bus_address, OUT_X_L, 1, &buffer)) {
            printf("Unable to read OUT_X_L at [%x].\n", OUT_X_L);
            return STATUS_ERROR;
        }
        axis_output[0] = buffer;
        if (i2c_read(bus_address, OUT_X_H, 1, &buffer)) {
            printf("Unable to read OUT_X_H at [%x].\n", OUT_X_H);
            return STATUS_ERROR;
        }
        axis_output[0] = axis_output[0] | (buffer << 8);
    }
    else {
        axis_output[0] = 0;
    }

    if (axis_mask & 0x2) {
        if (i2c_read(bus_address, OUT_Y_L, 1, &buffer)) {
            printf("Unable to read OUT_Y_L at [%x].\n", OUT_Y_L);
            return STATUS_ERROR;
        }
        axis_output[1] = buffer;
        if (i2c_read(bus_address, OUT_Y_H, 1, &buffer)) {
            printf("Unable to read OUT_Y_H at [%x].\n", OUT_Y_H);
            return STATUS_ERROR;
        }
        axis_output[1] = axis_output[1] | (buffer << 8);
    }
    else {
        axis_output[1] = 0;
    }

    if (axis_mask & 0x4) {
        if (i2c_read(bus_address, OUT_Z_L, 1, &buffer)) {
            printf("Unable to read OUT_Z_L at [%x].\n", OUT_Z_L);
            return STATUS_ERROR;
        }
        axis_output[2] = buffer;
        if (i2c_read(bus_address, OUT_Z_H, 1, &buffer)) {
            printf("Unable to read OUT_Z_H at [%x].\n", OUT_Z_H);
            return STATUS_ERROR;
        }
        axis_output[2] = axis_output[2] | (buffer << 8);
    }
    else {
        axis_output[2] = 0;
    }

    return STATUS_OK;
}