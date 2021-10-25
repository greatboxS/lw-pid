/**
 * @file pid-common.h
 * @author greatboxs (https://github.com/greatboxs/lw-pid.git)
 * @brief 
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PID_CONFIG_H__
#define __PID_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define USE_DEBUG

#ifdef USE_DEBUG
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 \
                                                                                                    : __FILE__)
#define PID_LOG(...)                                                  \
    printf("[PID_LOG] --> file %s, line %d: ", __FILENAME__, __LINE__); \
    printf(__VA_ARGS__)
#else
#define PID_LOG(...)
#endif

#define MAX_OUT_PERCENT 110.0F
#define MIN_OUT_PERCENT 0.0F
#define DEFAULT_KP 10.0F
#define DEFAULT_KI 0.1F

#define PID_ENABLE_P (0x01U)
#define PID_ENABLE_I (0x02U)
#define PID_ENABLE_D (0x04U)

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

#ifdef __cplusplus
}
#endif

#endif // __PID_CONFIG_H__