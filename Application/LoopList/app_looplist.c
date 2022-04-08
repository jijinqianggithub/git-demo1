#include "app_looplist.h" 
#include "fml_looplist.h" 
#include "fatfs.h"

IAP_UpDate_Flag Current_IAP_UpDate_Flag = Wait;
uint8_t IAP_UpdateBuff[IAP_UpdateMAXLen];
uint32_t UpdateReadCnt = 0;
uint32_t UpdateIdleCnt = 0;
char ReadDat[IAP_UpdateReadLen];
int32_t data_num = 0;
/**
  * @brief APP升级程序接收逻辑，先将串口接收到的数据存放在环形队列中，不断的写入SD卡中，最后一段数据在空闲中断后写入SD卡中
  * @param 
  * @param 
	* @param 
  * @retval 1：程序完整写入成功
  */
int8_t APP_Update_Recieve(void)
{
	uint8_t retSD;    /* Return value for SD */
	FATFS SDFatFS;    /* File system object for SD logical drive */
	FIL SDFile;       /* File object for SD */
	uint32_t byteswritten;                /* File write counts */
	uint32_t bytesread;                   /* File read counts */
	char HEX_FILE[] = "OBD_Diagnostic_BoxPrj.bin";

	//1.判断升级下载Flag满足升级条件，创建环形队列，创建升级文件
	if(Current_IAP_UpDate_Flag == Update)
	{
		retSD = f_mount(&SDFatFS, "0:", 1);
		if(retSD) printf(" mount error : %d \r\n",retSD);
		else
		{
			printf(" mount sucess!!! \r\n");
			
			retSD = f_open(&SDFile, HEX_FILE, FA_OPEN_ALWAYS  | FA_READ | FA_WRITE);
			if(retSD) printf(" open file error : %d\r\n",retSD);
			else 
			{
				printf(" open file success!!!\r\n");
				Current_IAP_UpDate_Flag = StartRec;
				memset(IAP_UpdateBuff,0,sizeof(IAP_UpdateBuff));
				rbCreate(&pRb,IAP_UpdateBuff,IAP_UpdateMAXLen);//创建接收环形队列
				printf("能写的空间 :%d\r\n",rbCanWrite(&pRb));
			}
		}
	}
	//2.从串口接收中读数据出来，写入SD卡中
	else if(Current_IAP_UpDate_Flag == StartRec)
	{
		data_num = rbCanRead(&pRb);
		if(data_num >= IAP_UpdateReadLen)//接收到更新程序就开始写入
		{
			rbRead(&pRb, &ReadDat, IAP_UpdateReadLen);//读取一串数据
			f_write(&SDFile,ReadDat,IAP_UpdateReadLen,&byteswritten);
		}
	}
	//3.当空闲中断的Flag满足后，将最后一段数据写入SD卡中
	else if(Current_IAP_UpDate_Flag == EndRec)
	{
		data_num = rbCanRead(&pRb);
		if(data_num>1)//将最后一部分写入文件中
		{
			memset(ReadDat,0,sizeof(ReadDat));
			rbRead(&pRb, &ReadDat, data_num);//读取一串数据
			
			f_write(&SDFile,ReadDat,sizeof(ReadDat),&byteswritten);
			
			Current_IAP_UpDate_Flag = RecSucc;
		}
		retSD = f_close(&SDFile);	
		if(retSD) printf(" close file error : %d\r\n",retSD);
		else
		{
			printf(" close file success!!!\r\n");
			retSD = f_mount(NULL, "0:", 1);	
			if(retSD)
			{
				printf(" mount error : %d\r\n",retSD);	
			}
			else 
			{
				printf(" mount close sucess!!! \r\n");
				Current_IAP_UpDate_Flag = JumpIAP;
				return 1;
			}
		}
	}
	return 0;
}