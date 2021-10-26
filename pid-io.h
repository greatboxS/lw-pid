/**
 * @file pid-io.h
 * @author greatboxs (https://github.com/greatboxs/lw-pid.git)
 * @brief 
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PID_IO_H__
#define __PID_IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "pid-typedef.h"

    /**
     * @brief set pv input io type and adc resolution
     * 
     * @param pid               pid handler              
     * @param type              io type
     * @param adc_resolution    adc resolution  
     * @return pid_result_t     error code
     */
    pid_result_t io_set_pv_input(pid_handle_t *pid, io_type_e input_type, int adc_resolution);

    /**
     * @brief set cv output io type and adc resolution
     * 
     * @param pid               pid handler              
     * @param type              io type
     * @param adc_resolution    adc resolution  
     * @return pid_result_t     error code
     */
    pid_result_t io_set_cv_output(pid_handle_t *pid, io_type_e output_type, int adc_resolution);

    /**
     * @brief get processing value from input pin
     * the result will be store in pid->pv.value
     * 
     * @param pid               pid handler
     * @param adc_resolution    adc resolution, eg:15 bit
     * @param adc_value         adc value, read from input pin
     * @return pid_result_t     error code
     */
    pid_result_t io_get_pv_value(pid_handle_t *pid, int adc_value);

    /**
     * @brief convert control value of pid controller to output value
     * the output value will be store in pid->output.adc.value
     * 
     * @param pid               pid handler
     * @return pid_result_t     error code
     */
    pid_result_t io_get_output_value(pid_handle_t *pid);

#ifdef __cplusplus
}
#endif

#endif // __PID-IO_H__