#include "pid-io.h"

/**
 * @brief set the pv or output io properties
 * 
 * @param io_prop           &pid->control.pv or &pid->control.cv_output
 */
static void io_set_io_type(pid_io_property_t *io_prop)
{
    switch (io_prop->io.type)
    {
    case IO_0_5VDC:
        io_prop->io.offset = 0;
        io_prop->io.range = 5;
        break;

    case IO_1_5VDC:
        io_prop->io.offset = 1;
        io_prop->io.range = 5;
        break;

    case IO_0_10VDC:
        io_prop->io.offset = 0;
        io_prop->io.range = 10;
        break;

    case IO_1_10VDC:
        io_prop->io.offset = 1;
        io_prop->io.range = 10;
        break;

    case IO_0_20mA:
        io_prop->io.offset = 0;
        io_prop->io.range = 20;
        break;

    case IO_4_20mA:
        io_prop->io.offset = 4;
        io_prop->io.range = 20;
        break;

    case IO_0_24VDC:
        io_prop->io.offset = 0;
        io_prop->io.range = 24;
        break;

    case IO_1_24VDC:
        io_prop->io.offset = 1;
        io_prop->io.range = 24;
        break;

    default:
        io_prop->io.offset = 0;
        io_prop->io.range = 5;
        break;
    }
}

/**
 * @brief set pv input io type and adc resolution
 * 
 * @param pid               pid handler              
 * @param type              io type
 * @param adc_resolution    adc resolution  
 * @return pid_result_t     error code
 */
pid_result_t io_set_pv_input(pid_handle_t *pid, io_type_e input_type, int adc_resolution)
{
    PID_RETURN_IF_NULL(pid);
    pid->control.pv.io.type = input_type;
    pid->control.pv.adc.resolution = adc_resolution;
    io_set_io_type(&pid->control.pv);
    return PID_OK;
}

/**
 * @brief set cv output io type and adc resolution
 * 
 * @param pid               pid handler              
 * @param type              io type
 * @param adc_resolution    adc resolution  
 * @return pid_result_t     error code
 */
pid_result_t io_set_cv_output(pid_handle_t *pid, io_type_e output_type, int adc_resolution)
{
    PID_RETURN_IF_NULL(pid);
    pid->control.cv_output.io.type = output_type;
    pid->control.cv_output.adc.resolution = adc_resolution;
    io_set_io_type(&pid->control.cv_output);
    return PID_OK;
}

/**
 * @brief get processing value from input pin
 * the result will be store in pid->pv.value
 * 
 * @param pid               pid handler
 * @param adc_resolution    adc resolution, eg:15 bit
 * @param adc_value         adc value, read from input pin
 * @return pid_result_t     error code
 */
pid_result_t io_get_pv_value(pid_handle_t *pid, int adc_value)
{
    PID_RETURN_IF_NULL(pid);
    float io_sub = pid->control.pv.io.range - pid->control.pv.io.offset;

    // get the pv in voltage or current
    // eg: [0 - 5V] <=> [0 - 32767] => 2V <=> 13,107 (ADC 16bit value)
    pid->control.pv.value = io_sub * (adc_value / pid->control.pv.adc.resolution) + pid->control.pv.io.offset;

    // get the final pv value
    // eg: [0 - 5V] <=> [0 - 1000] rpm => 2V <=> 400 rpm
    pid->control.pv.value = (pid->control.pv.max - pid->control.pv.min) * pid->control.pv.value / io_sub + pid->control.pv.min;

    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief convert control value of pid controller to output value
 * the output value will be store in pid->output.adc.value
 * 
 * @param pid               pid handler
 * @return pid_result_t     error code
 */
pid_result_t io_get_output_value(pid_handle_t *pid)
{
    PID_RETURN_IF_NULL(pid);
    float io_sub = pid->control.cv_output.io.range - pid->control.cv_output.io.offset;
    pid->err = PID_OK;
    return PID_OK;
}