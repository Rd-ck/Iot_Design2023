/*
 * LSM6DSO.h
 *
 *  Created on: May 23, 2023
 *      Author: Rd
 */

#ifndef INC_LSM6DSO_H_
#define INC_LSM6DSO_H_

#include "printf.h"
#include "main.h"
#include "i2c.h"

void LSM6DSOInit(void);
void LSM6DSOWriteByteReg(uint8_t addr,uint8_t reg,uint8_t* data);
uint8_t LSM6DSOReadReg(uint8_t addr,uint8_t reg);
void LSM6DSOPolling(void);


#endif /* INC_LSM6DSO_H_ */
