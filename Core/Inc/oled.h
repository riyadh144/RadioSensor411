#ifndef OLED_H
#define OLED_H
#include "stm32f4xx.h"
#include "string.h"
#include "stdint.h"
#include "fonts.h"
#include "stdio.h"
#define W 128
#define H 64
#define COMMAND 0x00
#define DATA 0x40

class oled{

private:
I2C_HandleTypeDef *hi2c;
TIM_HandleTypeDef *htim;
uint8_t address;

uint8_t comm[2];

uint32_t tmoCounter=0;
uint32_t tmoValue=60; // max



bool Refresh_Counter = false;

uint8_t oledOn=1;

public:
uint8_t oled_buffer[W*H/8+8];

typedef enum OLED_COLOR_t
{
    OLED_BLACK= 0x00,
    OLED_WHITE= 0xFF
}OLED_COLOR;

/**
 * @brief  Construct the OLED class
 * @param  hi2c: I2C interface it should be initialized to 400Khz
 * @param  address: I2C address of the display
 * @param  htim: timer handle to be used for refresh
 * @param  Refresh_Counter : if 1 its calling for a referesh otherwise it put system to sleep
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
oled(I2C_HandleTypeDef* hi2c, uint8_t address,TIM_HandleTypeDef* htim);

/**
 * @brief  Write the initialization data to the registers.
 * @note   Should be caLLed after the initialization of the I2C port
 * @param  color: OLED_BLACk OLED_WHITE
 * @retval None
 */
void init(void);

/**
 * @brief  Refresh the OLED
 * @Note   This function will be called every 66mS via timmer intterupt
 * @param  None
 * @retval None
 */
void oled_refresh(void);

/**
 * @brief  Set Battery voltage 
 * @note   It will update the battery voltage on top of the screen
 * @param  float: voltage to 1 decimal place
 * @retval None
 */
void oled_set_battery_voltage( float volt );

/**
 * @brief  Refresh the OLED
 * @note   This function will be called every 66mS via timmer intterupt
 * @param  listing: is the menu page you want 0:menu 1:freq 2:SQ 
 * @param  param: the param of the specific item
 * @retval None
 */
void oled_menu(uint8_t listing, uint8_t param);

/**
 * @brief  Write one byte to a register 
 * @note   
 * @param  data: the data byte you want to write data[0] is the register
 * @retval 1: ACK 0:Bad
 */
uint8_t oled_write(uint8_t* data, uint16_t count);

/**
 * @brief  Fill the screen solid Either fully bright or fully dark 
 * @note   None
 * @param  color: OLED_BLACk OLED_WHITE
 * @retval None
 */
void oled_fill(OLED_COLOR_t color);

/**
 * @brief Print a line of text on a position
 * 
 * @param string: string to be printed
 * @param size  : Which font FontDef_t
 * @param x     : x-Loc of the first letter
 * @param y     : y-Loc of the first letter
 */
void oled_print(const char* string, FontDef_t size, uint16_t x , uint16_t y);
/**
 * @brief Update the battery voltage on the top right of the screen
 * 
 * @param voltage Takes the input from the ADC 
 * @todo  implement a good interrupt routine to do this
 */
void oled_update_battery(float voltage);
/**
 * @brief Put the oled to sleep and kill the timer
 * 
 */
void oled_off(); 
/**
 * @brief wake the oled and set the timer
 * 
 */
void oled_on(); 
/**
 * @brief Set the time out so the display sleep in seconds
 * 
 * @param time: Time in seconds
 */
void oled_setTimer(uint32_t time); 
/**
 * @brief Zero the timer;
 * 
 */
void oled_resetTimer();
/**
 * @brief Is the OLED on?
 * 
 * @return uint8_t 0:Off 1: Onn
 */
uint8_t oled_isOledOn();

/**
 * @brief increments the counter and checks if timeout 
 * 
 */
void Counter_increment();

/**
 * @brief switches the state of the cursor to distinguish the parts being modified
 * 
 */
void switch_cursor();

/**
 * @brief next functions provide tools for GUI presentation on the OLED
 * 
 * @param x : x location
 * @param y : y location
 * @param w : width of shape
 * @param h : hight of shape
 */

void DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void DrawPixel(uint16_t x, uint16_t y);

void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
/**
 * @brief This function takes the tracks from the SD card and displays them on the screen
 * 
 */
void Track_list();


};


#endif