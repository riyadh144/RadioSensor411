#ifndef PIN_HPP
#define PIN_HPP
#include "stdint.h"
#include "stm32f4xx.h"

class pin
{
public:
typedef enum
{
    PIN0=0x0001,
    PIN1=0x0001<<1,
    PIN2=0x0001<<2,
    PIN3=0x0001<<3,
    PIN4=0x0001<<4,
    PIN5=0x0001<<5,
    PIN6=0x0001<<6,
    PIN7=0x0001<<7,
    PIN8=0x0001<<8,
    PIN9=0x0001<<9,
    PIN10=0x0001<<10,
    PIN11=0x0001<<11,
    PIN12=0x0001<<12,
    PIN13=0x0001<<13,
    PIN14=0x0001<<14,
    PIN15=0x0001<<15
}PinNumber;

typedef enum InOut
{
    in,
    out
}InOut;

typedef enum Pull
{
    NoPull,
    PullUp,
    PullDown
}Pull;

typedef enum Speed
{
    SPEED_LOW,
    SPEED_MEDIUM,
    SPEED_HIGH,
    SPEED_VERYHIGH
}Speed;

typedef enum OnOff
{
    off,
    on
}OnOff;

private:
GPIO_TypeDef * gpiox;

GPIO_InitTypeDef GPIO_InitStruct;

public:

/**
 * @brief Construct a new pin object
 * 
 * @param gpiox port of the pin
 * @param pinx  pin number
 * @param inout Pin status In or Out
 * @param pull  PullUp, PullDown, NoPull
 */
pin(GPIO_TypeDef * gpiox_, PinNumber pinx_, InOut inout_, Pull pull_, Speed speed_);
/**
 * @brief 
 * 
 */
void init();

/**
 * @brief Read the state of the pin
 * 
 * @return uint8_t 0, 1
 */
OnOff read();

/**
 * @brief Write to the pin if output
 * 
 * @return uint8_t 
 */
uint8_t write(OnOff onoff);

    

};



#endif