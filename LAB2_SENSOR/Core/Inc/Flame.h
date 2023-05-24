/*
 * Flame.h
 *
 *  Created on: May 23, 2023
 *      Author: Rd
 */

#ifndef INC_FLAME_H_
#define INC_FLAME_H_

#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "printf.h"
#include "mydelay.h"
/* USER CODE END Includes */

typedef enum {FALSE = 0,TRUE = 1} bool;

bool Fire(void);
#endif /* INC_FLAME_H_ */
