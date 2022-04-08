#ifndef APP_LOOPLIST_H_
#define APP_LOOPLIST_H_

#include "main.h"

#define IAP_UpdateMAXLen   2048  	//定义最大接收字节数(缓冲程序)
#define IAP_UpdateReadLen  128    //一次从环形队列中读取的字节数

typedef enum {Wait,Update,StartRec,EndRec,RecSucc,JumpIAP}IAP_UpDate_Flag;

extern IAP_UpDate_Flag Current_IAP_UpDate_Flag;

extern uint32_t UpdateReadCnt;
extern uint32_t UpdateIdleCnt;

int8_t APP_Update_Recieve(void);

#endif





























