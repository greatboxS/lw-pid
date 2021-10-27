/**
 * @file pid-typedef.h
 * @author greatboxs (https://github.com/greatboxs/lw-pid.git)
 * @brief 
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PID_TYPEDEF_H__
#define __PID_TYPEDEF_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "pid-common.h"

#define PID_CV_MAX_POS_VALUE 1100U
#define PID_CV_MIN_VALUE 0
#define PID_CV_MAX_NAG_VALUE (-1100)

#define PID_ERR_BUFF_SIZE (3U)

    typedef enum _pid_output_ctrl_method_e
    {
        /**
         * @brief Normal control. The output control signal always be a positive 
         * signal
         */
        PID_METHOD_POSITIVE = 0,
        /**
         * @brief This method is used for potition control or drirection control
         * Control value can be nagative, and the positive or nagative indicates 
         * the direction of the output control signal
         */
        PID_METHOD_BIO,
    } pid_output_ctrl_method_e;

    typedef enum _pid_operation_mode_e
    {
        PID_MANUAL_MODE = 0,
        PID_AUTO_MODE,
    } pid_operation_mode_e;

    typedef enum _pid_operation_phase_e
    {
        PID_RUNING_PHASE = 0,
        PID_AUTO_TUNING_PHASE
    } pid_operation_phase;

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

    typedef struct _pid_io_property_t
    {
        float max;
        float min;
        float value;
        float percent;

        /**
         * @brief The io is the input or output
         * eg: 0 - 5V DC, range = 5, offset = 0, type = IO_0_5VDC
         */
        struct io_t
        {
            io_type_e type;
            float range;
            float offset;
        } io;

        struct adc_t
        {
            float value;
            int32_t resolution;
        } adc;
    } pid_io_property_t;

    typedef pid_io_property_t pv_t;
    typedef pid_io_property_t cv_output_t;

    typedef struct _pid_control_t
    {
        float sv;                     // set value
        float err[PID_ERR_BUFF_SIZE]; // difference between pv and sv; (err = pv - sv)
        float sample_time;            // sample time

        /**
         * @brief pid process value
         * @param max pv maximum value, eg: motor speed max 1000 rpm
         * @param min pv maximum value, eg: motor speed min 0 rpm
         * @param value current pv value, eg: the value within [min; max] range
         */
        pv_t pv;

        /**
         * @brief pid controller output
         * @param max output maximum value eg: 4-20 mA (max  = 20)
         * @param min output miximum value
         * @param value output current control value of pid
         */
        cv_output_t cv_output;

        struct cv_t
        {
            float max;                               // the maximum value of pid calculation (raw output of calculation)
            float min;                               // the minimum value of pid calculation
            float buff[PID_ERR_BUFF_SIZE];           // control value/ MV of pid controller (affter calculation, not scaled yet)
            pid_output_ctrl_method_e output_ctrl_mt; // control method
            pid_limit_t high_limit;                  // high limitation value   (in percent %)
            pid_limit_t low_limit;                   // low limitation value    (in percent %)
            pid_gain_t gain;                         // gain value affter calculation
        } cv;

        pid_operation_mode_e operation_mode; // pid operation mode / manual / auto mode
    } pid_control_t;

    typedef struct _pid_handle_t
    {
        pid_para_t parameter;
        pid_control_t control;
        pid_init_flag_e flag;                // indicating the pid init state
        pid_operation_phase operation_phase; // indicating the phase of pid controller
        pid_result_t err;
    } pid_handle_t;

#ifdef __cplusplus
}
#endif
#endif // __PID-TYPEDEF_H__