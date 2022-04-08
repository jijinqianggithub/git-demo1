#ifndef __FML_USART_H
#define __FML_USART_H
#include "main.h"

#define MAX_RECV_LEN 30


typedef struct{
	UART_HandleTypeDef *huart;
	uint8_t						rx_buf[MAX_RECV_LEN];//接收缓冲区
	uint8_t           rx_data;
	uint32_t          rx_count;
	uint32_t						rx_flag;//接收完成标志位
}rxtx_it_usart_t;

extern uint8_t UpdateFlag;

extern rxtx_it_usart_t RxTx_IT_Usart1;

void Usart_SendString(UART_HandleTypeDef *huart,uint8_t *str);
void Usart_DMA_SendString(UART_HandleTypeDef *huart,uint8_t *str);
int Usart_IT_Deal(uint8_t data);

#endif /* __FML_USART_H */

