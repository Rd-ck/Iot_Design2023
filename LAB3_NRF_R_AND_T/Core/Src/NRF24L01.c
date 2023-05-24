
#include "NRF24L01.h"
#include "main.h"
#include "spi.h"
#include "stdio.h"

//NRF24L01 ��������

unsigned char idel_mode_flag = 0;
unsigned char mode_time_counter = 0;

const unsigned char INIT_ADDR0[5]= {0x02,0x3A,0xB1,0xB1,0x01};
const unsigned char INIT_ADDR1[5]= {0x02,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR2[5]= {0x03,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR3[5]= {0x04,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR4[5]= {0x05,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR5[5]= {0x06,0x3A,0x01,0x01,0x01};

#define CH_Num	120
#define debug_out(fmt,args...)  printf(fmt,##args)
//#define debug_out(fmt,args...) 

void delay_us(uint32_t n)
{
	unsigned char i;

	while(n--)
	{
		i = 8;
		while(i--);
	}
}
//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{
	//spi init
	//gpio init
	Clr_NRF24L01_CE;    // chip enable
	Set_NRF24L01_CSN;   // Spi disable
	delay_us(100);
}

//��װspi��д����
unsigned char nRF24_SPI_Send_Byte(unsigned char txdata)
{
	unsigned char rxdata;
	HAL_SPI_TransmitReceive(&hspi2, &txdata, &rxdata, 1, 0x10);
	return(rxdata);							// return read unsigned char
}


//ͨ��SPIд�Ĵ���
unsigned char NRF24L01_Write_Reg(unsigned char regaddr,unsigned char data)
{
	unsigned char status;
	Clr_NRF24L01_CSN;                    //ʹ��SPI����
	status =nRF24_SPI_Send_Byte(regaddr); //���ͼĴ�����
	nRF24_SPI_Send_Byte(data);            //д��Ĵ�����ֵ
	Set_NRF24L01_CSN;                    //��ֹSPI����
	return(status);       		         //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
unsigned char NRF24L01_Read_Reg(unsigned char regaddr)
{
	unsigned char reg_val;
	Clr_NRF24L01_CSN;                //ʹ��SPI����
	nRF24_SPI_Send_Byte(regaddr);     //���ͼĴ�����
	reg_val=nRF24_SPI_Send_Byte(0XFF);//��ȡ�Ĵ�������
	Set_NRF24L01_CSN;                //��ֹSPI����
	return(reg_val);                 //����״ֵ̬
}
//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
unsigned char NRF24L01_Read_Buf(unsigned char regaddr,unsigned char *pBuf,unsigned char datalen)
{
	unsigned char status,u8_ctr;
	Clr_NRF24L01_CSN;                     //ʹ��SPI����
	status=nRF24_SPI_Send_Byte(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	//for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//��������
	HAL_SPI_Receive(&hspi2, pBuf, datalen, 0x10);
	Set_NRF24L01_CSN;                     //�ر�SPI����
	return status;                        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
unsigned char NRF24L01_Write_Buf(unsigned char regaddr, unsigned char *pBuf, unsigned char datalen)
{
	unsigned char status,u8_ctr;
	Clr_NRF24L01_CSN;                                    //ʹ��SPI����
	status = nRF24_SPI_Send_Byte(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	//for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI_ReadWriteByte(*pBuf++); //д������
	HAL_SPI_Transmit(&hspi2, pBuf, datalen, 0x10);
	Set_NRF24L01_CSN;                                    //�ر�SPI����
	return status;                                       //���ض�����״ֵ̬
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
unsigned char NRF24L01_TxPacket(unsigned char *txbuf)
{
	unsigned char state;
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
	Set_NRF24L01_CE;                                     //��������
	while(READ_NRF24L01_IRQ!=0);                         //�ȴ��������
	state=NRF24L01_Read_Reg(STATUS);                     //��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)                                     //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //���TX FIFO�Ĵ���
		debug_out("TX MAX_TX error!\r\n");
		return MAX_TX;
	}
	if(state&TX_OK)                                      //�������
	{
		debug_out("TX OK!\r\n");
		return TX_OK;
	}
	debug_out("TX other error!\r\n");
	return 0xff;                                         //����ԭ����ʧ��
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
unsigned char NRF24L01_RxPacket(unsigned char *rxbuf)
{
	unsigned char state;
	state=NRF24L01_Read_Reg(STATUS);                //��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&TX_OK)
	{
		debug_out("RX send ack!\r\n"); //�ɹ�����ACK
	}
	if(state&RX_OK)                                 //���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //���RX FIFO�Ĵ���
		debug_out("RX read data!\r\n");
		return 0;
	}
	return 1;                                      //û�յ��κ�����
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
void RX_Mode(void)
{
	Clr_NRF24L01_CE;
	//дRX�ڵ��ַ
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)INIT_ADDR0,RX_ADR_WIDTH);

	//ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);
	//ʹ��ͨ��0�Ľ��յ�ַ
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);
	//����RFͨ��Ƶ��
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,CH_Num);
	//ѡ��ͨ��0����Ч���ݿ��
	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
	//����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
	//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f);
	//CEΪ��,�������ģʽ
	Set_NRF24L01_CE;
}

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,
//ѡ��RFƵ��,�����ʺ�LNA HCURR PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,����������.
void TX_Mode(void)
{
	Clr_NRF24L01_CE;
	//дTX�ڵ��ַ
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(unsigned char*)INIT_ADDR0,TX_ADR_WIDTH);
	//����TX�ڵ��ַ,��ҪΪ��ʹ��ACK
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(unsigned char*)INIT_ADDR0,RX_ADR_WIDTH);

	//ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);
	//ʹ��ͨ��0�Ľ��յ�ַ
	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);
	//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);
	//����RFͨ��Ϊ40
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,CH_Num);
	//����TX�������,0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
	//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ,���������ж�
	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);
	// CEΪ��,10us����������
	Set_NRF24L01_CE;
}

//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ�
//��ͬʱ����ֵ:0����ʾ��λ;���򷵻�1����ʾ����λ
unsigned char NRF24L01_Check(void)
{
	unsigned char buf[5]= {0XA5,0XA5,0XA5,0XA5,0XA5};
	unsigned char buf1[5];
	unsigned char i;
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);              //����д��ĵ�ַ
	for(i=0; i<5; i++)if(buf1[i]!=0XA5)break;
	if(i!=5)
	{
		debug_out(("nRF24L01 TEST FAIL\r\n"));
		return 1;                               //NRF24L01����λ
	}
	debug_out(("nRF24L01 TEST OK\r\n"));
	return 0;		                           //NRF24L01��λ
}
