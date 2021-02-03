#include "keyboard.hpp"

uint8_t keyboard::keyboard_poll()
{
    int i =0;
    uint16_t pin = GPIO_PIN_11;
    //Set The KEY1(PD11) One on

    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            return 'R';
            break;
        case GPIO_PIN_10:
            return '6';
            break;
        case GPIO_PIN_9:
            return '5';
            break;
        case GPIO_PIN_8:
            return '4';
            break;
        default:
            break;
    }
    GPIOD->ODR&=pin;

    pin<<1;//PD12
    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            return 'L';
            break;
        case GPIO_PIN_10:
            return '8';
            break;
        case GPIO_PIN_9:
            return '7';
            break;
        case GPIO_PIN_8:
            return '9';
            break;
        default:
            break;

    }
    GPIOD->ODR&=pin;


    pin<<1;//PD13
    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            return 'D';
            break;
        case GPIO_PIN_10:
            return 'M';
            break;
        case GPIO_PIN_9:
            return '0';
            break;
        case GPIO_PIN_8:
            return 'B';
            break;
        default:
            break;

    }
    GPIOD->ODR&=pin;

    pin<<1;//PD14
    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            return 'U';
            break;
        case GPIO_PIN_10:
            return '3';
            break;
        case GPIO_PIN_9:
            return '2';
            break;
        case GPIO_PIN_8:
            return '1';
            break;
        default:
            break;

    }
    GPIOD->ODR&=pin;
}
