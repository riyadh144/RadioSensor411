#ifndef UART_HPP
#define UART_HPP

#include "stm32f4xx.h"
#include "iostream"
#include "functional"
#include "string.h"
class uart{

    public:
    DMA_HandleTypeDef hdma_usart2_rx;
    DMA_HandleTypeDef hdma_usart2_tx;
    typedef enum 
    {
        uart1=1,
        uart2=2,
        uart6=6

    }uart_num;

    private:
    char txbuffer[100];
    char rxbuffer[100];
    uint32_t baudrate;
    uart_num uartNum;
    UART_HandleTypeDef uartx;

    public:
    uart(uart_num uartNum, uint32_t baudrate_);
    void init();
    void send(char* string);
    /**
     * @brief set the DMA to recieve
     * 
     * @param count 
     */
    void set_recieve(uint8_t count);

    /**
     * @brief send a string and wait for a return
     * 
     * @param txString 
     * @param rxString 
     * @return uint8_t 1:Done 0:Fail
     */
    uint8_t send_recive(char* txString, const char* rxString);
    
    void rx_cplt_callback();
    void Error_Handler(void);

};

#endif