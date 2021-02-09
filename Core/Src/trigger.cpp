#include "trigger.hpp"

trigger::trigger(GPIO_TypeDef * gpiox_,T_status status)
{
    gpiox = gpiox_;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    //TODO : choose the trigger pin

}

void trigger::Init()
{
    HAL_GPIO_Init(gpiox,&GPIO_InitStruct);
}

void trigger::change_rise(T_status rise)
{
    //status = rise; //TODO:FIX THIS
}