# AAC-Clydespace Avionics Software Challenge
You are given the LIS3MDLTR 3-Axis Magnetometer sensor. The data sheet is
located at `doc/lism3mdl.pdf`

## Task Description
You must use the C API defined in `i2c.h` to operate on an I2C bus.

We provide example stub implementations of this API in `i2c.c`. You are free to
extend the provided stub functions for the API.

Please write a device driver that implements the following API:
- Get the full-scale configuration
- Get and set the output data rate
- Enable or disable the interrupt pin
- Read the output data of a specified axis

## Development
You are provided this minimal repository, `aac-code-challenge`, please perform
your work on branch `<candidate_name>`

Documentation can be found in `aac-code-challenge/doc`

Once complete, please repackage and email back this repo to your interviewers

## Scope
You shouldn't need to spend more than a couple of hours on the task.

This is not a closed book assessment.

## Extra Thoughts
If you have time, answer the following questions:
- What changes you would make to this interfaces for use in an RTOS
environment?
- How might the I2C API be improved

## Extra Thoughts
- What changes you would make to this interfaces for use in an RTOS
environment?
We could designate priorities to certain get/set operations to make sure that
the important tasks are performed as quick as possible. We can also implement ISRs
for interrupts to be handled in a more efficient way as well.

## Added Functionality
> /*
>     Get full scale gauss selection
> */
> status_t get_full_scale_config(uint8_t *full_scale_config);

> /*
>     Get the currently set data_rate value
>     data_rate -> Reference to store the data_rate
> */
> status_t get_data_rate(float *data_rate);

> /*
>     Acceptable data_rate values are:
>     0.625
>     1.25
>     2.5
>     5
>     10
>     20
>     40
>     80
>     155
>     300
>     560
>    1000
> */
> status_t set_data_rate(float data_rate);

> /*
>     pin_status -> 1 = Enable interrupt
>     pin_status -> 0 = Disable interrupt
> */
> status_t set_interrupt_pin(uint8_t pin_status);

> /* 
>     axis_mask -> bit0 = X Output
>     axis_mask -> bit1 = Y Output
>     axis_mask -> bit2 = Z Output

>     axis_output -> An array of length 3 to store output data
> */
> status_t get_output_data(uint8_t axis_mask, uint16_t *axis_output);

> /*
>     This function is used to test the functionality of 
>     get_data_rate() function.
> */
> status_t test_get_data_rate(float set_data_rate);