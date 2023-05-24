/*
 * Si24R1.h
 *
 *  Created on: 2023Äê5ÔÂ9ÈÕ
 *      Author: Rd
 */

#ifndef INC_SI24R1_H_
#define INC_SI24R1_H_

#include "NRF24l01.h"
#include "my_delay.h"
#include "printf.h"

uint8_t Rx_loop(uint8_t *tmp_buf);
uint8_t Tx_loop(uint8_t *tmp_buf);

void Rx_Init(void);
void Tx_Init(void);

#endif /* INC_SI24R1_H_ */
