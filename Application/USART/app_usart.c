#include "app_usart.h"


/**
  * @brief 串口接收中断处理,按照自定义协议存储串口数组中
  * @param uint8_t data
	* @param 
  * @retval None
  */
int Usart_IT_Deal(uint8_t data)
{
	static uint16_t rx_size;
	static uint8_t fucn = 0, cmd = 0;
	
	RxTx_IT_Usart1.rx_buf[RxTx_IT_Usart1.rx_count++] = data;
	
	if(RxTx_IT_Usart1.rx_flag == 0)
	{
		//第0字节不是0x22，重置帧头
		if(RxTx_IT_Usart1.rx_buf[0] != 0x22)
		{
			RxTx_IT_Usart1.rx_count = 0;
			rx_size = 0;
		}
		//帧头到2，同时第1字节不是0x22，重置帧头
		else if((RxTx_IT_Usart1.rx_flag == 2) && ((RxTx_IT_Usart1.rx_buf[1] != 0x22)))
		{
			RxTx_IT_Usart1.rx_flag = 0;
			rx_size = 0;
		}
		//帧头到5，提取数组中包含的总字节数
		else if(RxTx_IT_Usart1.rx_count == 5)
		{
			rx_size = (RxTx_IT_Usart1.rx_buf[2]<<8) + RxTx_IT_Usart1.rx_buf[3];
			
			//计算出的总字节数大于数组长度了，也有问题
			if(rx_size >= MAX_RECV_LEN)
			{
				rx_size = 0;
				RxTx_IT_Usart1.rx_count = 0;
			//	rx_size = 0;					
			}
			else;
		}
		//帧头和总字节数相等，一包数据完整
		else if((rx_size == RxTx_IT_Usart1.rx_count) && (rx_size != 0))
		{
			//BCC校验，校验成功置标志位
			
			//不成功，不置位标志位，清帧头
			
			RxTx_IT_Usart1.rx_flag = 1;
			
//			while(HAL_UART_Transmit_IT(RxTx_IT_Usart1.huart,RxTx_IT_Usart1.rx_buf,RxTx_IT_Usart1.rx_count)==HAL_OK);
//			RxTx_IT_Usart1.rx_flag = 0;
			
			RxTx_IT_Usart1.rx_count = 0;
		}		
		else if(RxTx_IT_Usart1.rx_count >= MAX_RECV_LEN)
		{
			RxTx_IT_Usart1.rx_count = 0;
		}
	}
	
	if(RxTx_IT_Usart1.rx_flag == 1)
	{
		fucn = RxTx_IT_Usart1.rx_buf[4];
		cmd = RxTx_IT_Usart1.rx_buf[5];
		
		if(fucn == 0x01 && cmd == 0x01)
		{
			Current_IAP_UpDate_Flag = Update;
		}
		
	}
	
	if(RxTx_IT_Usart1.rx_count >= MAX_RECV_LEN-1)
	{
		memset(RxTx_IT_Usart1.rx_buf, 0, sizeof(RxTx_IT_Usart1.rx_buf));   //清空缓存区
	}	
	
}
