#include <stdio.h>
#include <stdint.h>
#include "i2c.h"

int main() {
    float set_data_ratef = 155;
    float get_data_ratef;
    set_data_rate(set_data_ratef);
    get_data_rate(&get_data_ratef);

    printf("Data rate written: %f\n", set_data_ratef);
    printf("Data rate read: %f\n", get_data_ratef);

    set_interrupt_pin(0);

    uint16_t output_data[3];
    get_output_data(3, output_data);
    printf("X Output: %x\n", output_data[0]);
    printf("Y Output: %x\n", output_data[1]);
    printf("Z Output: %x\n", output_data[2]);

    return 0;
}