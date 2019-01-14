#ifndef __UART485_H
#define __UART485_H

#include"stm8s.h"

#define CONTROL_CODE                    0X82
#define NORMAL_CTRL                   CONTROL_CODE
//#define UART_SEND_TIME 1111 //100ms
#define UART2_PORT    GPIOD
#define UART2_RX_PIN  GPIO_PIN_6
#define UART2_TX_PIN  GPIO_PIN_5

#define UART2_SOI                0XF0
#define UART2_EOI                0XF1

void uart2_init(void);
void uart2_received(void);
void Uart_SendData(unsigned char *p,unsigned char len);
unsigned char BlueToothUart_GetKey(void);
unsigned int GetTotalRuntime(void);

#endif
