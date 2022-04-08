#ifndef __FML_CAN_H
#define __FML_CAN_H
#include "main.h"


typedef struct{
	CAN_HandleTypeDef *hcan;
	uint8_t            frametype;
	uint32_t           id;
	uint32_t           datalen;
	uint8_t						tx_buf[8];
}tx_can_t;

typedef struct{
	CAN_HandleTypeDef *hcan;
	uint8_t            frametype;
	uint32_t           id;
	uint32_t           datalen;
	uint8_t						rx_buf[8];
}rx_can_t;

void CAN_SendMsg(CAN_HandleTypeDef *hcan, uint8_t frametype, uint32_t id, uint32_t datalen, uint8_t *txdata);
rx_can_t CAN_RecieveMsg(rx_can_t rx_can,CAN_RxHeaderTypeDef *pHeader, uint8_t *buf);


#endif /* __FML_CAN_H */

