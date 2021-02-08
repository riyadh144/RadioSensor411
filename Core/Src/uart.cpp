#include "uart.hpp"

uart::uart(uart_num uartNum_, uint32_t baudrate_) //TODO:Think to add parity and HW control in the future for now we use 8n1
{
    uartNum=uartNum_;
    baudrate=baudrate_;
    switch(uartNum)
    {
        case uart1:
        uartx.Instance = USART1;
            break;
        case uart2:
        uartx.Instance = USART2;
            break;
        case uart6:
        uartx.Instance = USART6;
            break;
    }   
    uartx.Init.BaudRate = baudrate_;
    uartx.Init.WordLength = UART_WORDLENGTH_8B;
    uartx.Init.StopBits = UART_STOPBITS_1;
    uartx.Init.Parity = UART_PARITY_NONE;
    uartx.Init.Mode = UART_MODE_TX_RX;
    uartx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uartx.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&uartx) != HAL_OK)
    {
    Error_Handler();
    }
}
uint8_t uart::send_recive(char* txString, char* rxString)
{
    HAL_UART_Transmit(&uartx,(uint8_t*)txString, strlen(txString), 100);
    set_recieve(strlen(rxString)-1); //Here we want to recieve the length of the string without the null char
    HAL_Delay(100);//Give it some time to reply
    if(strcmp(rxString, rxbuffer)==0)
    {
        return 1;
    }
    else
    {
        return send_recive(txString, rxString); //In case we get a different message or we get nothing back repeat the message
    }
    
}

void uart::set_recieve(uint8_t count)
{
    HAL_UART_Receive_DMA(&uartx, (uint8_t*) rxbuffer,count);
}
void uart::rx_cplt_callback()
{
    
}
void uart::Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
