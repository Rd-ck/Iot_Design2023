/*
 * Flame.c
 *
 *  Created on: May 23, 2023
 *      Author: Rd
 */
#include "Flame.h"


bool Fire(void){
	float ADC_value;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	ADC_value=HAL_ADC_GetValue(&hadc1);
	printf("Fire_value is %f\r\n",ADC_value);
	if(ADC_value>300)return TRUE;
	else return FALSE;
}

