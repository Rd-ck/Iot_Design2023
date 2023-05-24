/*
 * Si24R1.c
 *
 *  Created on: 2023Äê5ÔÂ9ÈÕ
 *      Author: Rd
 */

#include "Si24R1.h"
void Tx_Init(void){
	NRF24L01_Init();
	printf("\r\ninit OK!\r\n");
	NRF24L01_Check();
	TX_Mode();
}

void Rx_Init(void){
	printf("\r\ninit OK!\r\n");
	NRF24L01_Init();
	uint8_t temp = 4;
	while(NRF24L01_Check()&&(temp--));
	RX_Mode();
}

uint8_t Rx_loop(uint8_t *tmp_buf){
	if(NRF24L01_RxPacket(tmp_buf) == 0)
	{
		uint8_t i;
		for(i=0;i<32;i++)
			printf(" %02x",tmp_buf[i]);
		printf("\r\n");
		printf("%s",tmp_buf);


	}
	else {
		printf("Rx fali!\r\n");
		return 0;
	}
	printf("\r\n");
	return 1;
}

uint8_t Tx_loop(uint8_t *tmp_buf){

	if(NRF24L01_TxPacket(tmp_buf) == 0x20)
	{
		//read ack data;
		//NRF24L01_RxPacket(tmp_buf2);
    printf("send data OK!\r\n");
    uint8_t i;
		for(i=0;i<32;i++)
			printf(" %02x",tmp_buf[i]);
		printf("\r\n");
		printf("%s",tmp_buf);
		return 1;
	}
	else
	{
		printf("send error!");
		return 0;
	}
	printf("\r\n");
}
