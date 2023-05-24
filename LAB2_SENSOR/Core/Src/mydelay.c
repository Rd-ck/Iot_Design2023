/*
 * mydelay.c
 *
 *  Created on: May 22, 2023
 *      Author: Rd
 */
#include "mydelay.h"

typedef enum {FALSE = 0,TRUE = 1} bool;

volatile bool elapsed = FALSE; //用于判断设置的计数值是否耗尽（向下计数模式），耗尽时，在中断中奖会设置为true

void setState(bool state)
{
    elapsed = state;
}

bool getState()
{
    return elapsed;
}

void Delay_us(uint32_t time)
{

    __HAL_TIM_SetCounter(&htim2,time); //设置计数值
    setState(FALSE);
    HAL_TIM_Base_Start_IT(&htim2); //开启定时器
    while(!getState()); //判断计数值是否耗尽
    HAL_TIM_Base_Stop_IT(&htim2); //关闭定时器

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) {
		setState(TRUE);
	}

}
/* USER CODE END 1 */


