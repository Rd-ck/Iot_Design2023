#include "my_delay.h"

void my_delayus(uint32_t udelay){
	uint32_t startval,tickn,delays,wait;

	  startval = SysTick->VAL;
	  tickn = HAL_GetTick();
	  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
	  delays =udelay * 72; //sysc / 1000 * udelay;
	  if(delays > startval)
	    {
	      while(HAL_GetTick() == tickn)
	        {

	        }
	      wait = 72000 + startval - delays;
	      while(wait < SysTick->VAL)
	        {

	        }
	    }
	  else
	    {
	      wait = startval - delays;
	      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
	        {

	        }
	    }
}

void my_delayms(uint32_t mdelay){
	HAL_Delay(mdelay);
}
/*
 * my_delay.c
 *
 *  Created on: 2023年3月13日
 *      Author: Rd
 */


