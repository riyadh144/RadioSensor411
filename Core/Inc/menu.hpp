#ifndef MENU_H
#define MENU_H

#include "stdint.h"
#include "stdlib.h"
#include "oled.h"


class menu
{

private:

enum menu_state
{
    MENU_HOME=0, //Home screen
    MENU_CH=1, //Menu itself AND CH tab
    MENU_SQ=2, //MENU SQ tab
    MENU_TMO=3, //Meny Vol tab  
    MENU_MIC_REC=4, //Menu Mic tab
    MENU_MIC_PLAY = 5,
    MENU_CH_IN=1<<3,
    MENU_SQ_IN=2<<3,
    MENU_TMO_IN=3<<3,
    MENU_MIC_REC_IN = 4<<3,
    MENU_MIC_PLAY_IN = 5<<3


};

menu_state menu_value;

uint8_t value;

/**
 * @brief squash value in character form
 * 
 */
char sq[2]; //TODO: Check on the correct number of SQ channels 
uint8_t sqVal;

/**
 * @brief channel value in character form
 * 
 */
char ch[2];
uint8_t chVal;

/**
 * @brief volume value in character form
 * 
 */
char vol[2];
uint8_t volVal=0;

/**
 * @brief Display time out in seconds char form
 * 
 */
char tmo[2];
uint8_t tmoVal=30;//Default value of 30

/**
 * @brief 0:Cursor Left 1:Cursor Right
 * 
 */
uint8_t cursorPos=0;

/**
 * @brief 0:Cursor Off 1:Cursor On
 * 
 */
uint8_t cursorOn=0;

oled* oled1;


public:
/**
 * @brief Construct a new menu object  
 * 
 * @param oled_ oled to be desplayed on
 */
menu(oled *oled_);

/**
 * @brief  Print the menu updates what is going on
 * @param  None
 * @retval None
 */
void menu_print();

/**
 * @brief  Handles the press of the button menu
 * @note   enters or exits the menu
 * @param  None
 * @retval None
 */
void menu_menu();

/**
 * @brief  Handles the press of the button Next
 * @note   Goes to the next page or moves cursor
 * @param  None
 * @retval None
 */
void menu_next();

/**
 * @brief  handles the press of the button Prev
 * @note   goes to the previous page or moves the cursor
 * @param  None
 * @retval None
 */
void menu_prev();

/**
 * @brief Handles the press of a number
 * 
 * @param num:the number pressed
 */
void menu_number(char num);

/**
 * @brief  Handles the press of ok
 * @note   Ok is the only one tied to an interrupt
 * @note   It will wake the display enters a menu page or sets a value
 * @param  None
 * @retval None
 */
void menu_ok();

/**
 * @brief  Handles the press of back
 * @note   goes out of the menu
 * @param  None
 * @retval None
 */
void menu_back();

/**
 * @brief  Handles the press of up
 * @note   it increases the volume
 * @note   it increases the value in a menu page
 * @param  None
 * @retval None
 */
void menu_up();

/**
 * @brief  Handles the press of down
 * @note   it decreases the volume
 * @note   it decreases the value in a menu page
 * @param  None
 * @retval None
 */
void menu_down();

/**
 * @brief  Will be tied to an interrupt to update the menu
 * @todo   Figure this out
 * @param  None
 * @retval None
 */
void menu_update();

void keyboard_poll();

};

#endif