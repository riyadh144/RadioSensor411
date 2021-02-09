#include "trigger.hpp"

trigger::trigger(GPIO_TypeDef * gpiox_ ,PinNumber pinx_,state PinStatus_)
{

      /* EXTI interrupt init  on pin PB2 */
      /* These two lines are pin specific : TODO: find a way to make them general */
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    gpiox = gpiox_;
      /*Configure GPIO pin : x */
    GPIO_InitStruct.Pin = pinx_;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
}

void trigger::Init()
{
    HAL_GPIO_Init(gpiox, &GPIO_InitStruct);
}

void trigger::clear_trigger(uint16_t GPIO_Pin)
{   
     /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); // Clears The Interrupt Flag
        HAL_GPIO_EXTI_Callback(GPIO_Pin);   // Calls The ISR Handler CallBack Function
    }
}

void trigger::Switch_State(state rise)
{
    PinStatus_ = rise;
}

uint8_t trigger::read_State(void)
{
    return PinStatus_;
} 
