#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#include "stdint.h"
#include "stm32f4xx.h"

class keyboard
{

keyboard();
uint8_t keyboard_poll();

};

#endif