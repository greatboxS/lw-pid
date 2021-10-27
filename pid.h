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
#include "pid-eeprom.h"
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
    pid_result_t pid_set_pid_type(pid_handle_t *pid, int pid_enable);

    /**
     * @brief set parameter value to pid handler
     * 
     * @param pid 
     * @param para  pid parameter for kp, ki, kd 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_parameter(pid_handle_t *pid, pid_para_t *para);

    /**
     * @brief set auto / manual mode
     * 
     * @param pid 
     * @param mode          PID_MANUAL_MODE / PID_AUTO_MODE
     * @return pid_result_t 
     */
    pid_result_t pid_set_operation_mode(pid_handle_t* pid, pid_operation_mode_e mode);

    /**
     * @brief set control type for pid controller
     * 
     * @param pid 
     * @param ctrl_method   PID_METHOD_POSITIVE / PID_METHOD_BIO 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_output_ctrl_method(pid_handle_t *pid, pid_output_ctrl_method_e ctrl_method);

    /**
     * @brief set output high limitation for pid handler
     * 
     * @param pid 
     * @param high 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_cv_limit_h(pid_handle_t *pid, pid_limit_t *high);

    /**
     * @brief set output low limitation for pid handler
     * 
     * @param pid 
     * @param low 
     * @return pid_result_t function result
     */
    pid_result_t pid_set_cv_limit_l(pid_handle_t *pid, pid_limit_t *low);

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
     * @param pv_max        set the maximum of object to be controled, eg: 1000 rpm
     * @param pv_min        set the minimum of object to be controled, eg: 0 rpm
     * @return pid_result_t 
     */
    pid_result_t pid_set_pv_range(pid_handle_t *pid, float pv_max, float pv_min);

    /**
     * @brief set cv max, min value, this control value use for output scale value
     * 
     * @param pid 
     * @param max    maximum result of pid equation 
     * @param min    minimum result of pid equation 
     * @return pid_result_t 
     */
    pid_result_t pid_set_cv_max_min(pid_handle_t *pid, float max, float min);

    /**
     * @brief configuration pid handler, run this function after setting all 
     * paraemter of pid controller. Or when pid parameter/ sample time is changed
     * 
     * @param pid 
     * @return pid_result_t 
     */
    pid_result_t pid_extend_param_cal(pid_handle_t *pid);

    /**
     * @brief   on processing function
     *          call this function in the sample loop to execute pid controller
     * 
     * @param pid 
     * @param current_pv 
     * @return pid_result_t function result
     */
    pid_result_t pid_on_processing(pid_handle_t *pid, float current_pv);

    /**
     * @brief create default pid instance
     * 
     * @param pid           pointer to pid pointer
     * @return pid_result_t 
     */
    pid_result_t pid_create_new_default(pid_handle_t **pid);

#ifdef __cplusplus
}
#endif
#endif // __PID_H__