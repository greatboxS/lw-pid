#ifndef __PID_TYPEDEF_H__
#define __PID_TYPEDEF_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "pid-common.h"

#define PID_ERR_BUFF_SIZE (3U)

    typedef struct _pid_para_t
    {
        float kp; //
        float ki;
        float ti;
        float kd;

        float b0; // decrease pid controller variable
        float b1; // decrease pid controller variable
        float b2; // decrease pid controller variable

        bool enable_p; // enable p in controller
        bool enable_i; // enable i in controller
        bool enable_d; // enable d in controller
    } pid_para_t;

    typedef struct _pid_control_property_t
    {
        float value;
        bool enable;
    } pid_control_property_t;

    typedef pid_control_property_t pid_limit_t;
    typedef pid_control_property_t pid_gain_t;

    typedef struct _pid_control_t
    {
        float sv;                     // set value
        float cv[PID_ERR_BUFF_SIZE];  // control value/ MV of pid controller (affter calculation, not scaled yet)
        float err[PID_ERR_BUFF_SIZE]; // difference between pv and sv; (err = pv - sv)
        float sample_time;            // sample time

        /**
     * @brief pid process value
     * @param max pv maximum value, eg: motor speed max 1000 rpm
     * @param min pv maximum value, eg: motor speed min 0 rpm
     * @param value current pv value, eg: the value within [min; max] range
     */
        struct pv_t
        {
            float max;
            float min;
            float value;
            float percent;
        } pv;

        /**
     * @brief pid controller output
     * @param max output maximum value eg: 4-20 mA (max  = 20)
     * @param min output miximum value
     * @param value output current control value of pid
     */
        struct output_t
        {
            float max;
            float min;
            float value;
        } output;

        pid_limit_t out_limit_h;         // high limitation value
        pid_limit_t out_limit_l;         // low limitation value
        pid_gain_t gain;                 // gain value affter calculation
        pid_control_type_e control_type; // indicating what type of controller is
    } pid_control_t;

    typedef struct _pid_handle_t
    {
        pid_para_t parameter;
        pid_control_t control;
        pid_result_t err;
    } pid_handle_t;

#ifdef __cplusplus
}
#endif
#endif // __PID-TYPEDEF_H__