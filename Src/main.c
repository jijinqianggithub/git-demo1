/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG
#define APP
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
tx_can_t Tx_CAN1 = {
	.hcan = &hcan1      /*1.结构体成员指针需要初始化*/ 
};  
rx_can_t Rx_CAN1 = {
	.hcan = &hcan1      /*1.结构体成员指针需要初始化*/ 
};

tx_can_t Tx_CAN2 = {
	.hcan = &hcan2      /*1.结构体成员指针需要初始化*/ 
};  
rx_can_t Rx_CAN2 = {
	.hcan = &hcan2      /*1.结构体成员指针需要初始化*/ 
};

FATFS fs;                 // Work area (file system object) for logical drive
FIL fil;                  // file objects
uint32_t byteswritten;                /* File write counts */
uint32_t bytesread;                   /* File read counts */
char wtext[] = "1"; /* File write buffer */
uint8_t rtext[100];                     /* File read buffers */
char filename[] = "FirmwareUpdateFlag.txt";
uint8_t Receive_dat_buffer[100];			  	//数据接收缓存数组

uint32_t temp = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_SDIO_SD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	#ifdef APP
	SCB->VTOR = FLASH_BASE | 0x10000;//设置偏移量	
	#endif
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(RxTx_IT_Usart1.huart,&RxTx_IT_Usart1.rx_data,1);//串口接收到1个数据，接收之后调用接收完成回调函数
	
	printf("-----------APP_start---------\n");
	
	#ifdef DEBUG
//	retSD = f_mount(&fs, "0:", 1);
//	if(retSD)
//	{
//			printf(" mount error : %d \r\n",retSD);
//			Error_Handler();
//	}
//	else
//	{
//		printf(" mount sucess!!! \r\n");
//		retSD = f_open(&fil, filename, FA_OPEN_ALWAYS  | FA_READ | FA_WRITE);
//		if(retSD) printf(" open file error : %d\r\n",retSD);
//		else printf(" open file success!!!\r\n");
//	
//		retSD = f_write(&fil, wtext, sizeof(wtext), (void *)&byteswritten);
//		if(retSD) printf(" write file error : %d\r\n",retSD);
//		else printf(" write file success!!!\r\n");
//		
//		retSD = f_close(&fil);	
//		if(retSD) printf(" close file error : %d\r\n",retSD);
//		else printf(" close file success!!!\r\n");

//		printf("----------------------------------\r\n");
//		
//		retSD = f_open(&fil, filename, FA_OPEN_ALWAYS  | FA_READ | FA_WRITE);
//		if(retSD) printf(" open file error : %d\r\n",retSD);
//		else printf(" open file success!!!\r\n");
//		
//		retSD = f_read(&fil,rtext,sizeof(rtext),(void *)bytesread);
//		if(retSD) printf("read file error : %d\r\n",retSD);
//		else printf(" read date is : %s\r\n",rtext);
//			
//		retSD = f_close(&fil);	
//		if(retSD) printf(" close file error : %d\r\n",retSD);
//		else printf(" close file success!!!\r\n");
//		retSD = f_mount(NULL, "0:", 1);	
//		if(retSD) printf(" mount error : %d\r\n",retSD);	
//	}		
	#endif	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
		#ifdef DEBUG
	//	HAL_Delay(1000);
	//	LED_Operation(LED0_GPIO_Port,LED0_Pin,2);
		APP_Update_Recieve();
		if(Current_IAP_UpDate_Flag >= Update);
		else 
		{
			HAL_Delay(1000);
			Usart_DMA_SendString(RxTx_IT_Usart1.huart,(uint8_t *)"这条使用Usart_DMA输出\n");
			//printf("这条使用printf输出\n");
		}
		
		#ifdef APP
		if(Current_IAP_UpDate_Flag == JumpIAP)
		{
			printf("-----------APP_reset---------\n");
			retSD = f_mount(&fs, "0:", 1);
			if(retSD)
			{
					printf(" mount error : %d \r\n",retSD);
			}
			else
			{
				printf(" mount sucess!!! \r\n");
				retSD = f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE);
				if(retSD) printf(" open file error : %d\r\n",retSD);
				else printf(" open file success!!!\r\n");
			
				retSD = f_write(&fil, wtext, 1, (void *)&byteswritten);
				if(retSD) printf(" write file error : %d\r\n",retSD);
				else printf(" write file success!!!\r\n");
			
				retSD = f_close(&fil);	
				if(retSD) printf(" close file error : %d\r\n",retSD);
				else printf(" close file success!!!\r\n");
				retSD = f_mount(NULL, "0:", 1);	
				if(retSD) printf(" mount error : %d\r\n",retSD);	
			}		
			Current_IAP_UpDate_Flag = Wait;
			SCB->VTOR = FLASH_BASE;//设置偏移量
			NVIC_SystemReset();	
		}
		#endif
//		printf("这条使用printf输出\n");
//		Usart_DMA_SendString(RxTx_IT_Usart1.huart,(uint8_t *)"这条使用Usart_DMA输出\n");
//		
//		Tx_CAN2.frametype = 1;
//		Tx_CAN2.id = 0x0CF101D0;
//		Tx_CAN2.datalen = 8;
//		Tx_CAN2.tx_buf[0] = 0;
//		Tx_CAN2.tx_buf[1] = 1;
//		Tx_CAN2.tx_buf[2] = 2;
//		CAN_SendMsg(Tx_CAN2.hcan,Tx_CAN2.frametype,Tx_CAN2.id,Tx_CAN2.datalen,Tx_CAN2.tx_buf);
//		
//		Tx_CAN1.frametype = 1;
//		Tx_CAN1.id = 0x0CF102D0;
//		Tx_CAN1.datalen = 8;
//		Tx_CAN1.tx_buf[0] = 7;
//		Tx_CAN1.tx_buf[1] = 6;
//		Tx_CAN1.tx_buf[2] = 5;
//		CAN_SendMsg(Tx_CAN1.hcan,Tx_CAN1.frametype,Tx_CAN1.id,Tx_CAN1.datalen,Tx_CAN1.tx_buf);

		#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
	CAN_FilterTypeDef sFilterConfig;
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 8;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
	
	sFilterConfig.FilterBank = 0;									// 使用过滤器0
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;				// 屏蔽位模式
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;				// 32位宽
	sFilterConfig.FilterIdHigh = 0x0000;							// ID高十六位
	sFilterConfig.FilterIdLow = 0x0000;								// ID低十六位
	sFilterConfig.FilterMaskIdHigh = 0x0000;						// ID掩码高十六位
	sFilterConfig.FilterMaskIdLow = 0x0000;							// ID掩码低十六位
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;				// 过滤器0关联到FIFO0
	sFilterConfig.FilterActivation = ENABLE;						// **过滤器0
	sFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}	
	
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
	
	if(HAL_CAN_ActivateNotification(&hcan1,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF) != HAL_OK)					// 使能CAN接收中断
	{
		Error_Handler();

	}
  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */
	CAN_FilterTypeDef sFilterConfig;
  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 8;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_16TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = ENABLE;
  hcan2.Init.AutoWakeUp = ENABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
	sFilterConfig.FilterBank = 14;									// 使用过滤器0
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;				// 屏蔽位模式
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;				// 32位宽
	sFilterConfig.FilterIdHigh = 0x0000;							// ID高十六位
	sFilterConfig.FilterIdLow = 0x0000;								// ID低十六位
	sFilterConfig.FilterMaskIdHigh = 0x0000;						// ID掩码高十六位
	sFilterConfig.FilterMaskIdLow = 0x0000;							// ID掩码低十六位
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;				// 过滤器0关联到FIFO0
	sFilterConfig.FilterActivation = ENABLE;						// **过滤器0
	sFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_CAN_Start(&hcan2) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_CAN_ActivateNotification(&hcan2,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF) != HAL_OK)		// 使能FIFO0数据中断接收
	{
		Error_Handler();
	}
  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE); 
  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED0_Pin */
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
  * @brief CAN的FIFO0的回调函数
  * @param CAN_HandleTypeDef *hcan
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t buf[8];
	CAN_RxHeaderTypeDef CAN_RX_HDR;
	
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RX_HDR, buf) == HAL_OK)		// 获得接收到的数据头和数据
	{
		#ifdef DEBUG
		LED_Operation(LED0_GPIO_Port,LED0_Pin,2);
		#endif	
		if (Rx_CAN2.hcan == hcan)
		{
			Rx_CAN2 = CAN_RecieveMsg(Rx_CAN2,&CAN_RX_HDR,buf);
		}
		else if (Rx_CAN1.hcan == hcan)
		{
			Rx_CAN1 = CAN_RecieveMsg(Rx_CAN1,&CAN_RX_HDR,buf);
		}
		HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);	// 再次使能FIFO0接收中断
	}
}
/**
  * @brief 串口接收的回调函数
  * @param UART_HandleTypeDef *huart
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (RxTx_IT_Usart1.huart == huart)
	{
		#ifdef DEBUG
			LED_Operation(LED0_GPIO_Port,LED0_Pin,2);
		#endif
		if(Current_IAP_UpDate_Flag == StartRec)
		{
			UpdateReadCnt++;
			UpdateIdleCnt = 0;
			//printf("环形队列的容量 :%d\r\n",rbCapacity(&pRb));

			if(PutData(&pRb,&RxTx_IT_Usart1.rx_data,1) == -1)//写入环形队列
			{		
				printf("能写的空间 :%d\r\n",rbCanWrite(&pRb));
				printf("环形队列溢出\r\n");
			}	
		}
		else Usart_IT_Deal(RxTx_IT_Usart1.rx_data);
		
		while(HAL_UART_Receive_IT(RxTx_IT_Usart1.huart,&RxTx_IT_Usart1.rx_data,1) != HAL_OK);//串口接收到10个数据，接收之后调用接收完成回调函
	}
	
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
