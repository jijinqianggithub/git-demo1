#include "fml_led.h"

/**
  * @brief LED�Ƶ���λ����λ�ͷ�ת
  * @param GPIO_TypeDef* GPIOx
	* @param uint16_t GPIO_Pin
	* @param uint8_t Op
	*           @arg @ref 0:����
	*           @arg @ref 1:��λ
	*           @arg @ref 2:��ת
  * @retval None
  */
void LED_Operation(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t Op)
{
	if(0 == Op)
	{
		HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
	}
	else if(1 == Op)
	{
		HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
	}
	else if(2 == Op)
	{
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}
}
