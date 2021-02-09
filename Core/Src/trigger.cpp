#include "trigger.hpp"

trigger::trigger(GPIO_TypeDef * gpiox_)
{
      /* EXTI interrupt init  on pin PB2 */
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    gpiox = gpiox_;
      /*Configure GPIO pin : PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
}

void trigger::Init()
{
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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