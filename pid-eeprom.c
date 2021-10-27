#include "pid-eeprom.h"

static struct eeprom_func_t
{
    eeprom_read_f read_f;
    eeprom_write_f write_f;
} eeprom;

/**
 * @brief set a eeprom read function 
 * 
 * @param read_f 
 */
void pid_set_eeprom_read_func(eeprom_read_f read_f)
{
    eeprom.read_f = read_f;
}

/**
 * @brief set a eeprom write function
 * 
 * @param write_f 
 */
void pid_set_eeprom_write_func(eeprom_write_f write_f)
{
    eeprom.write_f = write_f;
}

/**
 * @brief eeprom write buffer
 * 
 * @param base_addr 
 * @param buffer 
 * @param size 
 */
void eeprom_write_data(uint32_t base_addr, uint8_t *buffer, size_t size)
{
    if (buffer)
    {
        if (eeprom.write_f)
        {
            for (size_t i = 0; i < size; i++)
            {
                eeprom.write_f(base_addr + i, buffer[i]);
            }
        }
    }
}

/**
 * @brief eeprom read buffer
 * 
 * @param base_addr 
 * @param buffer 
 * @param size 
 */
void eeprom_read_data(uint32_t base_addr, uint8_t *buffer, size_t size)
{
    if (buffer)
    {
        if (eeprom.read_f)
        {
            for (size_t i = 0; i < size; i++)
            {
                buffer[i] = eeprom.read_f(i + base_addr);
            }
        }
    }
}

/**
 * @brief pid save data to eeprom
 * 
 * @param base_addr 
 * @param pid 
 */
void pid_save_data(uint32_t base_addr, pid_handle_t* pid)
{
    eeprom_write_data(base_addr, (uint8_t*)pid, sizeof(pid_handle_t));
}

/**
 * @brief pid read data from eeprom
 * 
 * @param base_addr 
 * @param pid 
 */
void pid_read_data(uint32_t base_addr, pid_handle_t* pid)
{
    eeprom_read_data(base_addr, (uint8_t*)pid, sizeof(pid_handle_t));
}