#ifndef _SA818_H
#define _SA818_H
#include "stm32f4xx.h"
#include "pin.hpp"
#include "uart.hpp"
class sa818
{
private:

unsigned char tx_buf[50];
unsigned char rx_buf[30];
uint8_t locked; //Look on sending the next command
unsigned char tx_len;
unsigned char rx_len;

pin *pin_pd;
pin *pin_ptt;
uart *uartx;

public:

typedef enum  {
    tx=0,
    rx=1
}TxRx;

typedef enum  {
    off=0,
    on=1
}OnOff;

/**
 * @brief  Power on the module
 * @note   @ref AS it says toggles the PD pin
 * @param  on: 0: Power Down, 1:Power UP
 * @retval None
 */
void sa818_power(OnOff on);

/**
 * @brief  Set tranmission mode
 * @note   @ref controls PTT pin
 * @param  mode: 0: TX, 1:RX
 * @retval None
 */
void sa818_txrx_mode(TxRx mode);

/**
 * @brief  Passes on the UART interface handler
 * @note   @ref The UART should be initialized by the main funciton currently it is handled by HAL
 * @param  usart: USART HAL handle, specifies which UART interface to use
 * @retval None
 */
sa818 (uart* uartx_, pin* pin_pd_, pin* pin_ptt_);

/**
 * @brief  Configure the parameters of transmission.
 * @note   @ref None
 * @param  bw: Bandwidth 0: 12.5Khz 1: 25KHz
 * @param  tx_f: Transmit frequency 400-480MHz
 * @param  rx_f: Recieve Frequncy
 * @param  tx_subaudio: Tx_CTCSS or TX_CDCSS Adds a tone char array 0000 to disable
 * @param  sq: squilsh level 0-8 0 for listen mode
 * @param  rx_subaudio: rx_ctcss or rx_cdss adds a base tone to the recive 0000 to disable
 * @retval None
 */
void sa818_configure(uint8_t bw, char* tx_f, char* rx_f, char* tx_subaudio, uint8_t SQ, char* Rx_subaudio );

/**
 * @brief  Set Volume
 * @note   @ref Set the volume between 1 and 8 
 * @param  vol: 1-8
 * @retval None
 */
void sa818_set_volume(uint8_t vol);

/**
 * @brief  Read RSSI
 * @note   @ref Read Signal Strength
 * @param  None
 * @retval Value of RSSI between 0-255 in DB
 */
void sa818_get_RSSI();

/**
 * @brief  Get Module Version
 * @note   @ref gives the revision of the module
 * @param  None
 * @retval Returns a char array with the info
 */
void sa818_get_version();

void sa818_set_tail(uint8_t x);

/**
 * @brief send a message over the module.
 * 
 * @param string 
 */
void sa818_send_message(char* string);

/**
 * @brief set DMA to recieve 
 * 
 * @return char* 
 */
char* sa818_recieve_message(uint8_t count);

};

#endif