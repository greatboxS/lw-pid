#include "pid.h"
#include <stdlib.h>

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
pid_result_t pid_set_pid_type(pid_handle_t *pid, int pid_enable)
{
    PID_RETURN_IF_NULL(pid);

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

    pid->flag |= PID_INIT_TYPE;
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
    PID_RETURN_IF_NULL(pid);
    if (!para)
        return PID_ERROR;

    memcpy(&pid->parameter, para, sizeof(pid_para_t));
    pid->flag |= PID_INIT_PARA;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set auto / manual mode
 * 
 * @param pid 
 * @param mode          PID_MANUAL_MODE/ PID_AUTO_MODE
 * @return pid_result_t 
 */
pid_result_t pid_set_operation_mode(pid_handle_t *pid, pid_operation_mode_e mode)
{
    PID_RETURN_IF_NULL(pid);
    pid->control.operation_mode = mode;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set control type for pid controller
 * 
 * @param pid 
 * @param ctrl_method   PID_METHOD_POSITIVE / PID_METHOD_BIO 
 * @return pid_result_t function result
 */
pid_result_t pid_set_output_ctrl_method(pid_handle_t *pid, pid_output_ctrl_method_e ctrl_method)
{
    PID_RETURN_IF_NULL(pid);
    pid->control.cv.output_ctrl_mt = ctrl_method;
    pid->flag |= PID_INIT_OUTPUT_CTRL;
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
pid_result_t pid_set_cv_limit_h(pid_handle_t *pid, pid_limit_t *high)
{
    PID_RETURN_IF_NULL(pid);
    memcpy(&pid->control.cv.high_limit, high, sizeof(pid_limit_t));
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
pid_result_t pid_set_cv_limit_l(pid_handle_t *pid, pid_limit_t *low)
{
    PID_RETURN_IF_NULL(pid);
    memcpy(&pid->control.cv.low_limit, low, sizeof(pid_limit_t));
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
    PID_RETURN_IF_NULL(pid);
    memcpy(&pid->control.cv.gain, gain, sizeof(pid_gain_t));
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
    PID_RETURN_IF_NULL(pid);
    pid->control.sv = sv;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set pv max and min value
 * 
 * @param pid 
 * @param pv_max        set the maximum of object to be controled, eg: 1000 rpm
 * @param pv_min        set the minimum of object to be controled, eg: 0 rpm
 * @return pid_result_t 
 */
pid_result_t pid_set_pv_range(pid_handle_t *pid, float pv_max, float pv_min)
{
    PID_RETURN_IF_NULL(pid);
    pid->control.pv.max = pv_max;
    pid->control.pv.min = pv_min;
    pid->flag |= PID_INIT_PV_MIN_MAX;
    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief set cv max, min value, this control value use for output scale value
 * 
 * @param pid 
 * @param max    maximum result of pid equation 
 * @param min    minimum result of pid equation 
 * @return pid_result_t 
 */
pid_result_t pid_set_cv_max_min(pid_handle_t *pid, float max, float min)
{
    PID_RETURN_IF_NULL(pid);
    pid->control.cv.max = max;
    pid->control.cv.min = min;
    pid->flag |= PID_INIT_CV_MIN_MAX;
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
pid_result_t pid_extend_param_cal(pid_handle_t *pid)
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
    PID_RETURN_IF_NULL(pid);

    pid->parameter.b0 = -pid->parameter.kp + pid->parameter.ki * pid->control.sample_time / 2.0 +
                        2.0 * pid->parameter.kd / pid->control.sample_time;

    pid->parameter.b1 = pid->parameter.kp * pid->control.sample_time - 4.0 * pid->parameter.kd / pid->control.sample_time;

    pid->parameter.b2 = pid->parameter.kp + pid->parameter.ki * pid->control.sample_time / 2.0 +
                        2.0 * pid->parameter.kd / pid->control.sample_time;

    pid->flag |= PID_INIT_Bx;
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
    PID_RETURN_IF_NULL(pid);

    pid->control.pv.value = current_pv;

    pv_sub = pid->control.pv.max - pid->control.pv.min;
    limit_sub = h - l;

    if (pv_sub > 0 & limit_sub > 0)
    {
        pid->control.pv.percent = pid->control.pv.value / pv_sub;
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

    // 2.
    pid->control.cv.buff[0] = pid->control.cv.buff[2] + pid->parameter.b0 * pid->control.err[2] +
                              pid->parameter.b1 * pid->control.err[1] + pid->parameter.b2 * pid->control.err[0];

    // 3.
    if (pid->control.cv.high_limit.enable)
    {
        if ((pid->control.cv.buff[0] > pid->control.cv.high_limit.value) && (pid->control.cv.high_limit.value > 0))
        {
            pid->control.cv.buff[0] = pid->control.cv.high_limit.value;
        }
    }
    if (pid->control.cv.low_limit.enable)
    {
        if ((pid->control.cv.buff[0] < pid->control.cv.low_limit.value) && (pid->control.cv.low_limit.value > 0))
        {
            pid->control.cv.buff[0] = pid->control.cv.high_limit.value;
        }
    }

    // 4.
    for (int i = 2; i > 0; i--)
    {
        pid->control.cv.buff[i] = pid->control.cv.buff[i - 1];
        pid->control.err[i] = pid->control.err[i - 1];
    }

    // 5.

    pid->err = PID_OK;
    return PID_OK;
}

/**
 * @brief create default pid instance
 * 
 * @param pid           pointer to pid pointer
 * @return pid_result_t 
 */
pid_result_t pid_create_new_default(pid_handle_t **pid)
{
    pid_result_t err = PID_ERROR;

    PID_RETURN_IF_NULL(pid);

    *pid = pid_create_new();
    pid_handle_t *new_pid = *pid;

    PID_RETURN_IF_NULL(new_pid);

    err = pid_set_pid_type(new_pid, PID_ENABLE_P | PID_ENABLE_I | PID_ENABLE_D);

    err = pid_set_operation_mode(new_pid, PID_MANUAL_MODE);

    err = pid_set_output_ctrl_method(new_pid, PID_METHOD_POSITIVE);

    // default input is 0 - 2000
    err = pid_set_pv_range(new_pid, 2000, 0);

    err = io_set_pv_input(new_pid, IO_0_5VDC, ADC_16IT);

    if (new_pid->control.cv.output_ctrl_mt == PID_METHOD_POSITIVE)
        err = pid_set_cv_max_min(new_pid, PID_DEFAULT_MAX_CV, PID_DEFAULT_MIN_CV);
    else
        err = pid_set_cv_max_min(new_pid, PID_DEFAULT_MAX_CV, -PID_DEFAULT_MAX_CV);

    err = io_set_cv_output(new_pid, IO_4_20mA, ADC_16IT);

    pid_limit_t limit = {110.0f, true};

    err = pid_set_cv_limit_h(new_pid, &limit);

    limit.value = 0;
    err = pid_set_cv_limit_l(new_pid, &limit);

    pid_gain_t gain = {PID_DEFAULT_GAIN, true};
    err = pid_set_gain(new_pid, &gain);

    pid_save_data(PID_BASE_EEPROM_ADDRESS, new_pid);

    new_pid->err = err;
    return err;
}