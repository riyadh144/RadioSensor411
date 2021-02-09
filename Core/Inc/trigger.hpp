
#include "stdint.h"
#include "stm32f4xx.h"

class trigger
{


private:

    GPIO_TypeDef * gpiox;
    GPIO_InitTypeDef GPIO_InitStruct;

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


    typedef enum Trigger_state
    {
        Rise_T,
        Fall_T
    }state;


    /**
     * @brief Construct a new trigger object
     * 
     * @param gpiox port of the pin
     * @param status_ status of the pin whether its a rising or a falling edge
     * 
     * @note This class is responsible for initalising a pin as an external trigger  
     */
    trigger(GPIO_TypeDef * gpiox_,PinNumber pinx_,state Pinstatus_);

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

    /**
     * @brief this function switches the state of the pin
     * 
     */
    void Switch_State(state rise);

    /**
     * @brief This function reades the status of the pin and returns it
     * 
     * @return uint8_t 
     */
    uint8_t read_State(void);

    
~trigger();

};


