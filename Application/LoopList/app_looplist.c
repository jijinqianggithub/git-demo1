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
  * @brief APP������������߼����Ƚ����ڽ��յ������ݴ���ڻ��ζ����У����ϵ�д��SD���У����һ�������ڿ����жϺ�д��SD����
  * @param 
  * @param 
	* @param 
  * @retval 1����������д��ɹ�
  */
int8_t APP_Update_Recieve(void)
{
	uint8_t retSD;    /* Return value for SD */
	FATFS SDFatFS;    /* File system object for SD logical drive */
	FIL SDFile;       /* File object for SD */
	uint32_t byteswritten;                /* File write counts */
	uint32_t bytesread;                   /* File read counts */
	char HEX_FILE[] = "OBD_Diagnostic_BoxPrj.bin";

	//1.�ж���������Flag���������������������ζ��У����������ļ�
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
				rbCreate(&pRb,IAP_UpdateBuff,IAP_UpdateMAXLen);//�������ջ��ζ���
				printf("��д�Ŀռ� :%d\r\n",rbCanWrite(&pRb));
			}
		}
	}
	//2.�Ӵ��ڽ����ж����ݳ�����д��SD����
	else if(Current_IAP_UpDate_Flag == StartRec)
	{
		data_num = rbCanRead(&pRb);
		if(data_num >= IAP_UpdateReadLen)//���յ����³���Ϳ�ʼд��
		{
			rbRead(&pRb, &ReadDat, IAP_UpdateReadLen);//��ȡһ������
			f_write(&SDFile,ReadDat,IAP_UpdateReadLen,&byteswritten);
		}
	}
	//3.�������жϵ�Flag����󣬽����һ������д��SD����
	else if(Current_IAP_UpDate_Flag == EndRec)
	{
		data_num = rbCanRead(&pRb);
		if(data_num>1)//�����һ����д���ļ���
		{
			memset(ReadDat,0,sizeof(ReadDat));
			rbRead(&pRb, &ReadDat, data_num);//��ȡһ������
			
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