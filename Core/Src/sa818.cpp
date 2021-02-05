#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "sa818.h"


const unsigned char CMD_HAND[15]={0x41,0x54,0x2B,0x44,0x4D,0x4F,0x43,0x4F,0x4E,0x4E,0x45,0x43,0x54,0x0d,0x0a};
unsigned char CMD_SET[15]={0x41,0x54,0x2b,0x44,0x4d,0x4f,0x53,0x45,0x54,0x47,0x52,0x4f,0x55,0x50,0x3d};
unsigned char CMD_VOLUME[16]={0x41,0x54,0x2B,0x44,0x4D,0x4F,0x53,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45,0x3d};
unsigned char expected_rx[30]={0};
unsigned char len_txnow;
unsigned char len_rxnow;
unsigned char status_cnt = 2;
UART_HandleTypeDef* USART;
unsigned char tx_buf[50]={0};
unsigned char rx_buf[30]={0};
uint8_t locked=0; //Look on sending the next command
unsigned char tx_len;
unsigned char rx_len;



void sa818::sa818_power(OnOff on)
{
    if(on)
        pin_pd->write(pin::on);
    else
        pin_pd->write(pin::off);
}
void sa818::sa818_txrx_mode(TxRx mode)
{
    if(mode)
        pin_ptt->write(pin::on);
    else
        pin_ptt->write(pin::off);
    
}

sa818::sa818(UART_HandleTypeDef* usart, pin* pin_pd_, pin* pin_ptt_)

{
    pin_pd=pin_pd_;
    pin_ptt=pin_ptt_;
    // while(locked!=0);
    // locked=1;
    sa818_power(on);//Power on the module
    HAL_Delay(1000);
    sa818_txrx_mode(rx);//Force the module to transmit
    USART=usart;

    sprintf((char *)tx_buf,"AT+DMOCONNECT\r\n");
    tx_len=20;
    rx_len=16;
    HAL_UART_Transmit(USART, (uint8_t*)tx_buf, tx_len, (uint8_t)100);

    sprintf((char *)expected_rx,"+DMOCONNECT:0\r\n");
    locked=1;
    HAL_UART_Receive_DMA(USART, rx_buf,rx_len);

}

void sa818::sa818_configure(uint8_t bw, char* tx_f, char* rx_f, char* tx_subaudio, uint8_t SQ, char* Rx_subaudio )
{
    // while(locked!=0);
    // locked=1;
    HAL_Delay(1000);

    sprintf((char *)tx_buf,"AT+DMOSETGROUP=%i,%s,%s,%s,%i,%s\r\n",bw,tx_f,rx_f,tx_subaudio,SQ, Rx_subaudio);
    tx_len=50;
    rx_len=16;
    HAL_UART_Transmit(USART, (uint8_t*)tx_buf, tx_len, (uint8_t)100);


    sprintf((char *)expected_rx,"+DMOCONNECT:0\r\n");
    HAL_UART_Receive_DMA(USART, rx_buf,rx_len);

}

void sa818::sa818_set_volume(uint8_t vol)
{
    // while(locked!=0);
    // locked=1;
    sprintf((char *)tx_buf,"AT+DMOSETVOLUME=%i\r\n",vol);
    tx_len=20;
    rx_len=16;
    HAL_UART_Transmit(USART, (uint8_t*)tx_buf, tx_len, (uint8_t)100);

    sprintf((char *)expected_rx,"+DMOCONNECT:0\r\n");
    HAL_UART_Receive_DMA(USART, rx_buf,rx_len);

}
void sa818::sa818_get_RSSI()
{
        // while(locked!=0);
    // locked=1;
    sprintf((char *)tx_buf,"RSSI?\r\n");
    tx_len=7;
    rx_len=10;
    HAL_UART_Transmit(USART, (uint8_t*)tx_buf, tx_len, (uint8_t)100);

    sprintf((char *)expected_rx,"+DMOCONNECT:0\r\n");
    HAL_UART_Receive_DMA(USART, rx_buf,rx_len);
}
void sa818::sa818_set_tail(uint8_t x)
{
    sprintf((char *)tx_buf,"AT+SETTAIL=%i\r\n",x);
    tx_len=14;
    rx_len=16;
    HAL_UART_Transmit(USART, (uint8_t*)tx_buf, tx_len, (uint8_t)100);

    sprintf((char *)expected_rx,"+DMOSETTAIL: 0\r\n");
    HAL_UART_Receive_DMA(USART, rx_buf,rx_len);
}