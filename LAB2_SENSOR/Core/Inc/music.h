/*
 * music.h
 *
 *  Created on: May 22, 2023
 *      Author: Rd
 */

#ifndef INC_MUSIC_H_
#define INC_MUSIC_H_

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "mydelay.h"

extern const uint16_t tone[];

extern const uint8_t music1[];
extern const uint8_t music2[];

extern const uint8_t time1[];
extern const uint8_t time2[];

void BEEPH(void);
void BEEPL(void);
void music_import(const uint8_t* music,const uint8_t* time);


#endif /* INC_MUSIC_H_ */
