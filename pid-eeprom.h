/**
 * @file pid-eeprom.h
 * @author greatboxs (https://github.com/greatboxs/lw-pid.git)
 * @brief 
 * @version 0.1
 * @date 2021-10-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PID_EEPROM_H__
#define __PID_EEPROM_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "pid-typedef.h"

    typedef void (*eeprom_write_f)(uint32_t, uint8_t);
    typedef uint8_t (*eeprom_read_f)(uint32_t);

    /**
     * @brief set a eeprom read function 
     * 
     * @param read_f 
     */
    void pid_set_eeprom_read_func(eeprom_read_f read_f);

    /**
     * @brief set a eeprom write function
     * 
     * @param write_f 
     */
    void pid_set_eeprom_write_func(eeprom_write_f write_f);

    /**
     * @brief eeprom write buffer
     * 
     * @param base_addr 
     * @param buffer 
     * @param size 
     */
    void eeprom_write_data(uint32_t base_addr, uint8_t *buffer, size_t size);

    /**
     * @brief eeprom read buffer
     * 
     * @param base_addr 
     * @param buffer 
     * @param size 
     */
    void eeprom_read_data(uint32_t base_addr, uint8_t *buffer, size_t size);

    /**
     * @brief pid save data to eeprom
     * 
     * @param base_addr 
     * @param pid 
     */
    void pid_save_data(uint32_t base_addr, pid_handle_t* pid);

    /**
     * @brief pid read data from eeprom
     * 
     * @param base_addr 
     * @param pid 
     */
    void pid_read_data(uint32_t base_addr, pid_handle_t* pid);

#ifdef __cplusplus
}
#endif
#endif // __PID-EEPROM_H__