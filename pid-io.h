#ifndef __PID_IO_H__
#define __PID_IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "pid-common.h"

#define ADC_16IT (16U)
#define ADC_12IT (12U)
#define ADC_10IT (10U)

    typedef enum _io_type_e
    {
        IO_0_5VDC = 0,
        IO_1_5VDC,
        IO_0_10VDC,
        IO_1_10VDC,
        IO_0_20mA,
        IO_4_20mA,
        IO_0_24VDC,
        IO_1_24VDC,
    } io_type_e;

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
    float io_get_pv_value(io_type_e type, int adc_resolution, int adc_value, float pv_max, float pv_min);

    /**
     * @brief convert control value of pid controller to output value
     * 
     * @param type              output type
     * @param adc_resolution    adc resolution, eg: 15 bits
     * @param pid               pid which contains the control value
     * @return int              output value (dac value)
     */
    int io_get_output_value(io_type_e type, int adc_resolution, pid_handle_t* pid);

#ifdef __cplusplus
}
#endif

#endif // __PID-IO_H__