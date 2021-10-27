/**
 * @file pid-tuning.h
 * @author greatboxs (https://github.com/greatboxs/lw-pid.git)
 * @brief 
 * @version 0.1
 * @date 2021-10-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PID_TUNING_H__
#define __PID_TUNING_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "pid-typedef.h"

    /**
     * @brief auto tuning function,
     * The tuning module will calculate the pid parameter and return to pid normal mode
     * 
     * @param pid 
     * @param timeout
     * @return pid_result_t 
     */
    pid_result_t pid_tuning_run(pid_handle_t *pid, uint32_t timeout);

#ifdef __cplusplus
}
#endif
#endif // __PID-TUNING_H__