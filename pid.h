#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "pid_config.h"

#ifdef USE_DEBUG
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 \
                                                                                                    : __FILE__)
#define PID_LOG(...)                                                  \
    printf("[PID_LOG] --> file %s, line %d: ", __FILENAME__, __LINE__); \
    printf(__VA_ARGS__)
#else
#define PID_LOG(...)
#endif

#define PID_ENABLE_P (0x01U)
#define PID_ENABLE_I (0x02U)
#define PID_ENABLE_D (0x04U)

#define PID_ERR_BUFF_SIZE (3U)
    typedef enum _pid_control_type_e
    {
        PID_FLOAT_CONTROL,
        PID_PERCENT_CONTROL
    } pid_control_type_e;

    typedef enum _pid_result_t
    {
        PID_ERR_LIMIT = -6,
        PID_ERR_GAIN = -5,
        PID_ERR_S = -4,
        PID_ERR_PV = -3,
        PID_ERR_MEM = -2,
        PID_ERROR = -1,
        PID_OK = 0,
    } pid_result_t;

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
 * @param control_type 
 * @return pid_result_t function result
 */
    pid_result_t pid_set_control_type(pid_handle_t *pid, pid_control_type_e control_type);

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