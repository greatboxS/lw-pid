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

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define USE_DEBUG

#ifdef USE_DEBUG
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 \
                                                                                                    : __FILE__)
#define PID_LOG(...)                                                    \
    printf("[PID_LOG] --> file %s, line %d: ", __FILENAME__, __LINE__); \
    printf(__VA_ARGS__)
#else
#define PID_LOG(...)
#endif

#define MAX_OUT_PERCENT 110.0F
#define MIN_OUT_PERCENT 0.0F
#define PID_DEFAULT_KP 10.0F
#define PID_DEFAULT_KI 0.1F
#define PID_DEFAULT_MAX_CV 11000.0F
#define PID_DEFAULT_MIN_CV 0.F
#define PID_DEFAULT_GAIN 1.0F
#define PID_BASE_EEPROM_ADDRESS 0x00

#define PID_ENABLE_P (0x01U)
#define PID_ENABLE_I (0x02U)
#define PID_ENABLE_D (0x04U)

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

    typedef enum _pid_init_flag_e
    {
        PID_CREATED = 0,
        PID_INIT_TYPE = 0x01,
        PID_INIT_PV_MIN_MAX = 0x02,
        PID_INIT_CV_MIN_MAX = 0x04,
        PID_INIT_CV_IO = 0x08,
        PID_INIT_PV_IO = 0x10,
        PID_INIT_PARA = 0x20,
        PID_INIT_Bx = 0x40,
        PID_INIT_OUTPUT_CTRL = 0x80,
    } pid_init_flag_e;

#define PID_INIT_ALL ((uint32_t)(PID_INIT_TYPE | PID_INIT_PV_MIN_MAX | PID_INIT_CV_MIN_MAX | PID_INIT_CV_IO | \
                                 PID_INIT_PV_IO | PID_INIT_CV_IO | PID_INIT_PARA | PID_INIT_Bx | PID_INIT_OUTPUT_CTRL))
/**
 * @brief pid-io definition 
 * 
 */
#define ADC_16IT (32768U)
#define ADC_12IT (4096U)
#define ADC_10IT (1024U)

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

#define PID_RETURN_IF_NULL(x)                    \
    if (x == NULL)                                      \
    {                                            \
        PID_LOG("NULL variable is detected.\n"); \
        return PID_ERROR;                        \
    }

#ifdef __cplusplus
}
#endif

#endif // __PID_CONFIG_H__