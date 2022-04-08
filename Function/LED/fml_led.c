#include "fml_led.h"

/**
  * @brief LED灯的置位，复位和翻转
  * @param GPIO_TypeDef* GPIOx
	* @param uint16_t GPIO_Pin
	* @param uint8_t Op
	*           @arg @ref 0:清零
	*           @arg @ref 1:置位
	*           @arg @ref 2:翻转
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
