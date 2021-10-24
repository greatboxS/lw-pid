#include "pid.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief create new pid handler structure
 * 
 * @return pid_handle_t* 
 */
pid_handle_t *pid_create_new()
{
    PID_LOG("create %d new pid handler\n", 1);
    return ((pid_handle_t *)calloc(1, sizeof(pid_handle_t)));
}

/**
 * @brief initialize a pid_handler
 * 
 * @param pid_handler   pid handler to be initialized
 * @param pid_enable    pid enable variable eg: (PID_ENABLE_P | PID_ENABLE_I) for enable PI controller
 * @return pid_result_t function result
 */
pid_result_t pid_init(pid_handle_t *pid, int pid_enable)
{
    if (!pid)
        return PID_ERROR;

    pid->parameter.enable_p = false;
    pid->parameter.enable_i = false;
    pid->parameter.enable_d = false;
    if (pid_enable & PID_ENABLE_P)
    {
        pid->parameter.enable_p = true;
    }
    if (pid_enable & PID_ENABLE_I)
    {
        pid->parameter.enable_i = true;
    }
    if (pid_enable & PID_ENABLE_D)
    {
        pid->parameter.enable_d = true;
    }
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set gain value to pid handler
 * 
 * @param pid 
 * @param gain 
 * @return pid_result_t function result
 */
pid_result_t pid_set_parameter(pid_handle_t *pid, pid_para_t *para)
{
    if (!pid)
        return PID_ERROR;
    if (!para)
        return PID_ERROR;

    memcpy(&pid->parameter, para, sizeof(pid_para_t));
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set control type for pid controller
 * 
 * @param pid 
 * @param control_type 
 * @return pid_result_t function result
 */
pid_result_t pid_set_control_type(pid_handle_t *pid, pid_control_type_e control_type)
{
    if (!pid)
        return PID_ERROR;
    pid->control.control_type = control_type;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set output high limitation for pid handler
 * 
 * @param pid 
 * @param high 
 * @return pid_result_t function result
 */
pid_result_t pid_set_limit_h(pid_handle_t *pid, pid_limit_t *high)
{
    if (!pid)
        return PID_ERROR;
    memcpy(&pid->control.out_limit_h, high, sizeof(pid_limit_t));
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set output low limitation for pid handler
 * 
 * @param pid 
 * @param low 
 * @return pid_result_t function result
 */
pid_result_t pid_set_limit_l(pid_handle_t *pid, pid_limit_t *low)
{
    if (!pid)
        return PID_ERROR;
    memcpy(&pid->control.out_limit_l, low, sizeof(pid_limit_t));
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set output gain for pid controller
 * 
 * @param pid 
 * @param gain
 * @return pid_result_t function result
 */
pid_result_t pid_set_gain(pid_handle_t *pid, pid_gain_t *gain)
{
    if (!pid)
        return PID_ERROR;
    memcpy(&pid->control.gain, gain, sizeof(pid_gain_t));
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set sv value
 * 
 * @param pid 
 * @param sv 
 * @return pid_result_t function result
 */
pid_result_t pid_set_sv_value(pid_handle_t *pid, float sv)
{
    if (!pid)
        return PID_ERROR;
    pid->control.sv = sv;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set pv max and min value
 * 
 * @param pid 
 * @param pv_max 
 * @param pv_min 
 * @return pid_result_t 
 */
pid_result_t pid_set_pv_range(pid_handle_t *pid, float pv_max, float pv_min)
{
    if (!pid)
        return PID_ERROR;
    pid->control.pv.max = pv_max;
    pid->control.pv.min = pv_min;

    // set limitaion in percent mode
    if (pid->control.control_type == PID_PERCENT_CONTROL)
    {
        if (pid->control.pv.max > MAX_OUT_PERCENT)
            pid->control.pv.max = MAX_OUT_PERCENT;

        if (pid->control.pv.min < 0)
            pid->control.pv.min = 0;
    }
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set output value, this control value use for output scale value
 * 
 * @param pid 
 * @param output_max    maximum output control value, eg: (4-20 mA) => max = 20
 * @param output_min    minimum output control value, eg: min = 4
 * @return pid_result_t 
 */
pid_result_t pid_set_output(pid_handle_t *pid, float output_max, float output_min)
{
    if (!pid)
        return PID_ERROR;
    pid->control.output.max = output_max;
    pid->control.output.min = output_min;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief configuration pid handler, run this function after setting all 
 * paraemter of pid controller. Or when pid parameter/ sample time is changed
 * 
 * @param pid 
 * @return pid_result_t 
 */
pid_result_t pid_configuration(pid_handle_t *pid)
{

    /** --------- "Tustin method"
   b0 = Kd/T = Kd.Td/T
   b1 = -(Kp + 2Kd/T) = -Kp(1 + 2Td/T)
   b2 = Kp + Ki.T + Kd/T = Kp(1 + 1.T/Ti + Td/T)  
   
   u(k) = u(k-2) + b2.e(k) + b1.e(k-1) + b0.e(k-2);   
   ------------------------------------------------------------------ 
   b2 = Kp + Ki.T/2 + 2.Kd/T = Kp(1 + T/(2.Ti) + 2Td/T)
   b1 =  Ki.T - 4Kd/T = Kp(T/Ti - 4Td/T)
   b0 = -(Kp - Ki.T/2 - 2Kd/T = -Kp(1 - T/2.Ti - 2Td/T)

   Ki = Kp/Ti
   Kd = Kp*Td;
*/
    if (!pid)
        return PID_ERROR;
    pid->parameter.b0 = -pid->parameter.kp + pid->parameter.ki * pid->control.sample_time / 2.0 + 2.0 * pid->parameter.kd / pid->control.sample_time;
    pid->parameter.b1 = pid->parameter.kp * pid->control.sample_time - 4.0 * pid->parameter.kd / pid->control.sample_time;
    pid->parameter.b2 = pid->parameter.kp + pid->parameter.ki * pid->control.sample_time / 2.0 + 2.0 * pid->parameter.kd / pid->control.sample_time;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief   on processing function
 *          call this function in the sample loop to execute pid controller
 * 
 * @param pid 
 * @param current_pv 
 * @return pid_result_t function result
 */
pid_result_t pid_on_processing(pid_handle_t *pid, float current_pv)
{
    float pv_sub = 0, limit_sub = 0, h = 0, l = 0;
    if (!pid)
        return PID_ERROR;

    pid->control.pv.value = current_pv;

    if (pid->control.control_type == PID_FLOAT_CONTROL)
    {
    }
    else if (pid->control.control_type == PID_PERCENT_CONTROL)
    {
        if (pid->control.out_limit_h.enable)
            h = pid->control.out_limit_h.value;
        else
            h = MAX_OUT_PERCENT;

        if (pid->control.out_limit_l.enable)
            l = pid->control.out_limit_h.value;
        else
            l = MIN_OUT_PERCENT;
    }

    pv_sub = pid->control.pv.max - pid->control.pv.min;
    limit_sub = h - l;

    if (pv_sub > 0 & limit_sub > 0)
    {
        pid->control.pv.percent = pid->control.pv.value/ pv_sub; 
    }
    else
        return PID_ERROR;

    /**
     * @brief 
     * u(k) = u(k-2) + b2.e(k) + b1.e(k-1) + b0.e(k-2);
     * 1. calculate the err = e(k)
     * 2. calculating the cv[0] = u(k)
     * 3. checking the output limitation
     * 4. restore old value of err and cv
     * 5. calculating the ouput value
     */

    // 1.
    if (pid->control.control_type == PID_PERCENT_CONTROL)
    {

    }
    else{
        pid->control.err[0] = pid->control.sv - pid->control.pv.value;
    }

    // 2.
    pid->control.cv[0] = pid->control.cv[2] + pid->parameter.b0 * pid->control.err[2] + pid->parameter.b1 * pid->control.err[1] + pid->parameter.b2 * pid->control.err[0];

    // 3.
    if (pid->control.out_limit_h.enable)
    {
        if ((pid->control.cv[0] > pid->control.out_limit_h.value) && (pid->control.out_limit_h.value > 0))
        {
            pid->control.cv[0] = pid->control.out_limit_h.value;
        }
    }
    if (pid->control.out_limit_l.enable)
    {
        if ((pid->control.cv[0] < pid->control.out_limit_l.value) && (pid->control.out_limit_l.value > 0))
        {
            pid->control.cv[0] = pid->control.out_limit_h.value;
        }
    }

    // 4.
    for (int i = 2; i > 0; i--)
    {
        pid->control.cv[i] = pid->control.cv[i - 1];
        pid->control.err[i] = pid->control.err[i - 1];
    }

    // 5.

    pid->err = PID_OK;
    return PID_OK;
}

static pid_result_t pid_cal_output(pid_handle_t *pid)
{
    if (!pid)
        return PID_ERROR;

    float cv_max = 0;
    float cv_min = 0;

    if (pid->control.control_type == PID_PERCENT_CONTROL)
    {
        if (pid->control.out_limit_h.enable)
        {
        }
        else
        {
            cv_max = MAX_OUT_PERCENT;
        }
    }
    else
    {
    }
    pid->err = PID_OK;
    return PID_OK;
}