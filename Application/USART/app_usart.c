#include "app_usart.h"


/**
  * @brief ���ڽ����жϴ���,�����Զ���Э��洢����������
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
		//��0�ֽڲ���0x22������֡ͷ
		if(RxTx_IT_Usart1.rx_buf[0] != 0x22)
		{
			RxTx_IT_Usart1.rx_count = 0;
			rx_size = 0;
		}
		//֡ͷ��2��ͬʱ��1�ֽڲ���0x22������֡ͷ
		else if((RxTx_IT_Usart1.rx_flag == 2) && ((RxTx_IT_Usart1.rx_buf[1] != 0x22)))
		{
			RxTx_IT_Usart1.rx_flag = 0;
			rx_size = 0;
		}
		//֡ͷ��5����ȡ�����а��������ֽ���
		else if(RxTx_IT_Usart1.rx_count == 5)
		{
			rx_size = (RxTx_IT_Usart1.rx_buf[2]<<8) + RxTx_IT_Usart1.rx_buf[3];
			
			//����������ֽ����������鳤���ˣ�Ҳ������
			if(rx_size >= MAX_RECV_LEN)
			{
				rx_size = 0;
				RxTx_IT_Usart1.rx_count = 0;
			//	rx_size = 0;					
			}
			else;
		}
		//֡ͷ�����ֽ�����ȣ�һ����������
		else if((rx_size == RxTx_IT_Usart1.rx_count) && (rx_size != 0))
		{
			//BCCУ�飬У��ɹ��ñ�־λ
			
			//���ɹ�������λ��־λ����֡ͷ
			
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
		memset(RxTx_IT_Usart1.rx_buf, 0, sizeof(RxTx_IT_Usart1.rx_buf));   //��ջ�����
	}	
	
}
