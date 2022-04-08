#include "fml_usart.h"


rxtx_it_usart_t RxTx_IT_Usart1 = {
   .huart = &huart1    /*1.�ṹ���Աָ����Ҫ��ʼ��*/ 
}; 

uint8_t UpdateFlag = 0;//IAP������־λ
/**
  * @brief �����ַ�������
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
  * @brief �ض���c�⺯��printf������USART1���ض�����ʹ��printf��
  * @param int ch
	* @param FILE *f
  * @retval int ch
  */
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

/**
  * @brief �ض���c�⺯��scanf������USART1����д����ʹ��scanf��getchar�Ⱥ�
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
  * @brief ����DMA�ķ���
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
  * @brief ��λ��ָ�����
  * @param 
	* @param 
  * @retval None
  */

