/**
 * @file pid.h
 * @author greatboxs (https://github.com/greatboxs/lw-pid.git)
 * @brief 
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "pid-io.h"

    /**
     * @brief create new pid handler structure
     * 
     * @return pid_handle_t* 
     */
    pid_handle_t *pid_create_new();

    /**
     * @brief initialize a pid_handler
     * 
     * @param pid   pid handler to be initialized
     * @param pid_enable    pid enable variable eg: (PID_ENABLE_P | PID_ENABLE_I) for enable PI controller
     * @return pid_result_t function result
     */
    pid_result_t pid_init(pid_handle_t *pid, int pid_enable);

    /**
     * @brief set parameter value to pid handler
     * 
     * @param pid 
     * @param para  pid parameter for kp, ki, kd 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_parameter(pid_handle_t *pid, pid_para_t *para);

    /**
     * @brief set control type for pid controller
     * 
     * @param pid 
     * @param ctrl_method 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_control_method(pid_handle_t *pid, pid_output_ctrl_method_e ctrl_method);

    /**
     * @brief set output high limitation for pid handler
     * 
     * @param pid 
     * @param high 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_limit_h(pid_handle_t *pid, pid_limit_t *high);

    /**
     * @brief set output low limitation for pid handler
     * 
     * @param pid 
     * @param low 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_limit_l(pid_handle_t *pid, pid_limit_t *low);

    /**
     * @brief set output gain for pid controller
     * 
     * @param pid 
     * @param gain
     * @return pid_result_t function result
     */
    pid_result_t pid_set_gain(pid_handle_t *pid, pid_gain_t *gain);

    /**
     * @brief set sv value
     * 
     * @param pid 
     * @param sv 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_sv_value(pid_handle_t *pid, float sv);

    /**
     * @brief set pv max and min value
     * 
     * @param pid 
     * @param pv_max 
     * @param pv_min 
     * @return pid_result_t 
     */
    pid_result_t pid_set_pv_range(pid_handle_t *pid, float pv_max, float pv_min);

    /**
     * @brief set output value, this control value use for output scale value
     * 
     * @param pid 
     * @param output_max    maximum output control value, eg: (4-20 mA) => max = 20
     * @param output_min    minimum output control value, eg: min = 4
     * @return pid_result_t 
     */
    pid_result_t pid_set_output(pid_handle_t *pid, float output_max, float output_min);

    /**
     * @brief configuration pid handler, run this function after setting all 
     * paraemter of pid controller. Or when pid parameter/ sample time is changed
     * 
     * @param pid 
     * @return pid_result_t 
     */
    pid_result_t pid_configuration(pid_handle_t *pid);

    /**
     * @brief   on processing function
     *          call this function in the sample loop to execute pid controller
     * 
     * @param pid 
     * @param current_pv 
     * @return pid_result_t function result
     */
    pid_result_t pid_on_processing(pid_handle_t *pid, float current_pv);

#ifdef __cplusplus
}
#endif
#endif // __PID_H__