#include "fml_can.h"


/**
  * @brief CAN发送
  * @param CAN_HandleTypeDef *hcan
	* @param uint8_t frametype  
	*           @arg @ref 0:标准帧 
	*           @arg @ref 1:扩展帧
	* @param uint32_t id
	* @param uint32_t datalen
	* @param uint8_t *txdata
  * @retval None
  */
void CAN_SendMsg(CAN_HandleTypeDef *hcan, uint8_t frametype, uint32_t id, uint32_t datalen, uint8_t *txdata)
{
	uint32_t              TxMailbox;
	uint8_t							 FreeTxMailBoxNum;
	CAN_TxHeaderTypeDef   TxHeader;	

	TxHeader.DLC = datalen;
	TxHeader.RTR = CAN_RTR_DATA;
	
	if(0 == frametype)   //标准帧定义
	{
		TxHeader.StdId = id; 
		TxHeader.IDE = CAN_ID_STD;
	}
	else if(1 == frametype)  //扩展帧定义
	{
		TxHeader.ExtId = id;
		TxHeader.IDE = CAN_ID_EXT;
	}
	
	TxHeader.TransmitGlobalTime = DISABLE;
	
  while(0 == FreeTxMailBoxNum)
  {
   FreeTxMailBoxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);//查看空闲邮箱数量
  }	
	
	if (HAL_CAN_AddTxMessage(hcan, &TxHeader, txdata, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}

}

/**
  * @brief CAN接收解析
  * @param rx_can_t rx_can
  * @param CAN_RxHeaderTypeDef *pHeader
	* @param uint8_t *buf
  * @retval rx_can_t
  */
rx_can_t CAN_RecieveMsg(rx_can_t rx_can,CAN_RxHeaderTypeDef *pHeader, uint8_t *buf)
{

	uint8_t i = 0;
	
	if(CAN_ID_STD == pHeader->IDE)
	{
		rx_can.id = pHeader->StdId;
		rx_can.frametype = 0;
	}
	else if(CAN_ID_EXT == pHeader->IDE)
	{
		rx_can.id = pHeader->ExtId;
		rx_can.frametype = 1;
	}
	
	rx_can.datalen = pHeader->DLC;
	
	for(i=0; i<8; i++)
	{
		rx_can.rx_buf[i] = buf[i];
	}
	
	return rx_can;
}
