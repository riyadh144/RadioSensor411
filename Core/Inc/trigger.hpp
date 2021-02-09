
#include "stdint.h"
#include "stm32f4xx.h"

class trigger
{
    
typedef enum T_status
{
    Fall_E,
    Rise_E
}T_status;


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
    trigger(GPIO_TypeDef * gpiox,T_status status);

    /**
     * @brief PIN Initaliser
     * 
     */
    void Init();

    /**
     * @brief a function to change the trigger status of the pin
     * 
     * @param new status
     * 
     */
    void change_rise(T_status rise);


};

trigger::~trigger()
{
}


