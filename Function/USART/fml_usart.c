#include "fml_usart.h"


rxtx_it_usart_t RxTx_IT_Usart1 = {
   .huart = &huart1    /*1.结构体成员指针需要初始化*/ 
}; 

uint8_t UpdateFlag = 0;//IAP升级标志位
/**
  * @brief 串口字符串发送
  * @param UART_HandleTypeDef *huart
	* @param uint8_t *str
  * @retval None
  */
void Usart_SendString(UART_HandleTypeDef *huart,uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(huart,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}

/**
  * @brief 重定向c库函数printf到串口USART1，重定向后可使用printf函
  * @param int ch
	* @param FILE *f
  * @retval int ch
  */
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

/**
  * @brief 重定向c库函数scanf到串口USART1，重写向后可使用scanf、getchar等函
  * @param FILE *f
  * @retval int ch
  */
int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/**
  * @brief 串口DMA的发送
  * @param 
	* @param 
  * @retval None
  */
void Usart_DMA_SendString(UART_HandleTypeDef *huart,uint8_t *str)
{
	static unsigned int k = 0;
	k = strlen((char *)str);
	HAL_UART_Transmit_DMA(huart,str,k);
}

/**
  * @brief 上位机指令解析
  * @param 
	* @param 
  * @retval None
  */

