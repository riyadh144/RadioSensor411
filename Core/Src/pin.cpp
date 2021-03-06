#include "pin.hpp"

pin::pin(GPIO_TypeDef * gpiox_, PinNumber pinx_, InOut inout_, Pull pull_,Speed speed_)
{
    
    gpiox=gpiox_;
    GPIO_InitStruct.Pin=pinx_;
    GPIO_InitStruct.Mode=inout_;
    GPIO_InitStruct.Pull=pull_;
    GPIO_InitStruct.Speed=speed_;
    HAL_GPIO_Init(gpiox, &GPIO_InitStruct);

    //TODO:figure out what is GPIO_InitStruct.Alternate
}

void pin::init()
{
  HAL_GPIO_Init(gpiox, &GPIO_InitStruct);
}

uint8_t pin::write(OnOff onoff)
{
    // if((gpiox->IDR & GPIO_InitStruct.Pin) != 0)
    //     return -1; // recheck or switch the pin state ....
    if(onoff==on)
    {
        gpiox->ODR |= (GPIO_InitStruct.Pin);
    }
    else
    {
        gpiox->ODR &= (!GPIO_InitStruct.Pin);
    }
    return 0;
}

pin::OnOff pin::read()
{
    
    if(!(gpiox->IDR & GPIO_InitStruct.Pin))
    {
        return off;
    }
    else
    {
        return on;
    }
    
}

