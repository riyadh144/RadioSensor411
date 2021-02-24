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

sa818::sa818(uart* uartx_, pin* pin_pd_, pin* pin_ptt_)

{
    pin_pd=pin_pd_;
    pin_ptt=pin_ptt_;
    uartx=uartx_;

    // while(locked!=0);
    // locked=1;
    sa818_power(on);//Power on the module
    //HAL_Delay(400);//Delay here is important to give some time for the module to wake up
    //TODO: maybe use something more efficient in the future.
    sa818_txrx_mode(rx);//Force the module to transmit
    sprintf((char *)tx_buf,"AT+DMOCONNECT\r\n");
    tx_len=20;
    rx_len=16;
    uartx->send_recive((char*) tx_buf,"+DMOCONNECT:0\r\n");
    
}

void sa818::sa818_configure(uint8_t bw, const char* tx_f, const char* rx_f, uint8_t SQ)
{

     char Rx_subaudio[9];
     char tx_subaudio[9];

    if(SQ > 4)  // 5~8  squash channels
    {
        strcpy(tx_subaudio,"TX_CDCSS");
        strcpy(Rx_subaudio,"RX_CDCSS");
    }
    else if( SQ > 0) // 1~4 squash channels
    {
        strcpy(tx_subaudio,"Tx_CTCSS");
        strcpy(Rx_subaudio,"Rx_CTCSS");
    }
    else{ // ~0 sq (Listening mode)
        strcpy(tx_subaudio,strcpy(Rx_subaudio,"0000"));
    }
    // while(locked!=0);
    // locked=1;
    HAL_Delay(1000);

    sprintf((char *)tx_buf,"AT+DMOSETGROUP=%i,%s,%s,%s,%i,%s\r\n",bw,tx_f,rx_f,tx_subaudio,SQ, Rx_subaudio); //TODO: Consesistant capitalization
    tx_len=50;
    rx_len=16;
    uartx->send_recive((char*) tx_buf,"+DMOCONNECT:0\r\n");
}


void sa818::sa818_set_volume(uint8_t vol)
{
    // while(locked!=0);
    // locked=1;
    sprintf((char *)tx_buf,"AT+DMOSETVOLUME=%i\r\n",vol);
    tx_len=20;
    rx_len=16;

    uartx->send_recive((char*) tx_buf,"+DMOCONNECT:0\r\n");


}
void sa818::sa818_get_RSSI()
{

    sprintf((char *)tx_buf,"RSSI?\r\n");
    tx_len=7;
    rx_len=10;
    uartx->send_recive((char*) tx_buf,"+DMOCONNECT:0\r\n");

}
void sa818::sa818_set_tail(uint8_t x)
{
    sprintf((char *)tx_buf,"AT+SETTAIL=%i\r\n",x);
    tx_len=14;
    rx_len=16;

    uartx->send_recive((char*) tx_buf,"+DMOSETTAIL: 0\r\n");

}
//TODO implement the handle to check on the recieved message
char* sa818::sa818_recieve_message(uint8_t count)
{
    return 0;
}