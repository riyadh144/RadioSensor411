
#include "stdint.h"
#include "stm32f4xx.h"

class trigger
{
  

private:

    GPIO_TypeDef * gpiox;
    GPIO_InitTypeDef GPIO_InitStruct;

public:
    /**
     * @brief Construct a new trigger object
     * 
     * @param gpiox port of the pin
     * @param status_ status of the pin whether its a rising or a falling edge
     * 
     * @note This class is responsible for initalising a pin as an external trigger  
     */
    trigger(GPIO_TypeDef * gpiox);

    /**
     * @brief PIN Initaliser
     * 
     */
    void Init();

    /**
     * @brief this function clears the triggers after its happening
     * 
     * @param GPIO_Pin 
     */
    void clear_trigger(uint16_t GPIO_Pin);

};

trigger::~trigger()
{
}


