#ifndef OLED_H
#define OLED_H
#include "stm32f4xx_hal_i2c.h"
class oled{
public:
oled(I2C_HandleTypeDef hi2c, uint_8 address);

};