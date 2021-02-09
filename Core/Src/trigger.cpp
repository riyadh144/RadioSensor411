#include "trigger.hpp"

/*
    the function to turn off the trigger is 
    void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
    in main
    */

trigger::trigger(GPIO_TypeDef *gpiox_, PinNumber pinx, Pull pull_)
{

      /* EXTI interrupt init  on pin PB2 */
      /* These two lines are pin specific : TODO: find a way to make them general */
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

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

void trigger::SetEdge(uint8_t Rise)
{
    gpiox->MODER = Rise;
}

void trigger::SetPull(uint8_t Pull)
{
    gpiox->PUPDR = Pull;
}

