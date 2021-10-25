#include "pid-io.h"

/**
 * @brief get processing value from input pin
 * 
 * @param type              input type
 * @param adc_resolution    adc resolution, eg:15 bit
 * @param adc_value         adc value, read from input pin
 * @param pv_max            pv maximum value
 * @param pv_min            pv miximum value
 * @return float            processing value after scale
 */
float io_get_pv_value(io_type_e type, int adc_resolution, int adc_value, float pv_max, float pv_min)
{
    float io_range = 0.f;
    float io_offset = 0.f;
    float adc_range = 0.f;
    float pv = 0.f;

    if (adc_resolution == ADC_16IT)
    {
        adc_range = 32768;
    }
    else if (adc_resolution == ADC_12IT)
    {
        adc_range = 4096;
    }
    else
    {
        adc_range = 1024;
    }

    switch (type)
    {
    case IO_0_5VDC:
        io_offset = 0;
        io_range = 5;
        break;

    case IO_1_5VDC:
        io_offset = 1;
        io_range = 5;
        break;

    case IO_0_10VDC:
        io_offset = 0;
        io_range = 10;
        break;

    case IO_1_10VDC:
        io_offset = 1;
        io_range = 10;
        break;

    case IO_0_20mA:
        io_offset = 0;
        io_range = 20;
        break;

    case IO_4_20mA:
        io_offset = 4;
        io_range = 20;
        break;

    case IO_0_24VDC:
        io_offset = 0;
        io_range = 24;
        break;

    case IO_1_24VDC:
        io_offset = 1;
        io_range = 24;
        break;

    default:
        io_offset = 0;
        io_range = 5;
        break;
    }

    // get the pv in voltage or current
    // eg: [0 - 5V] <=> [0 - 32767] => 2V <=> 13,107 (ADC 16bit value)
    pv = (io_range - io_offset) * (adc_value / adc_range) + io_offset;

    // get the final pv value
    // eg: [0 - 5V] <=> [0 - 1000] rpm => 2V <=> 400 rpm
    pv = (pv_max - pv_min) * pv / (io_range - io_offset) + pv_min;

    return pv;
}

/**
 * @brief convert control value of pid controller to output value
 * 
 * @param type              output type
 * @param adc_resolution    adc resolution, eg: 15 bits
 * @param pid               pid which contains the control value
 * @return int              output value (dac value)
 */
int io_get_output_value(io_type_e type, int adc_resolution, pid_handle_t *pid)
{
    float io_range = 0.f;
    float io_offset = 0.f;
    float adc_range = 0.f;
    float output_result = 0.f;

    if (adc_resolution == ADC_16IT)
    {
        adc_range = 32768;
    }
    else if (adc_resolution == ADC_12IT)
    {
        adc_range = 4096;
    }
    else
    {
        adc_range = 1024;
    }

    switch (type)
    {
    case IO_0_5VDC:
        io_offset = 0;
        io_range = 5;
        break;

    case IO_1_5VDC:
        io_offset = 1;
        io_range = 5;
        break;

    case IO_0_10VDC:
        io_offset = 0;
        io_range = 10;
        break;

    case IO_1_10VDC:
        io_offset = 1;
        io_range = 10;
        break;

    case IO_0_20mA:
        io_offset = 0;
        io_range = 20;
        break;

    case IO_4_20mA:
        io_offset = 4;
        io_range = 20;
        break;

    case IO_0_24VDC:
        io_offset = 0;
        io_range = 24;
        break;

    case IO_1_24VDC:
        io_offset = 1;
        io_range = 24;
        break;

    default:
        io_offset = 0;
        io_range = 5;
        break;
    }
    return 0;
}