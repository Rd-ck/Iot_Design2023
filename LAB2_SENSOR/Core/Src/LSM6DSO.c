/*
 * LSM6DSO.c
 *
 *  Created on: May 23, 2023
 *      Author: Rd
 */


#include "LSM6DSO.h"

#define LSM6DSO_ADDRESS    	 0xD4

#define LSM6DSO_ID    	     0x6C

#define LSM6DSO_SDO_CRTL          0x02
#define LSM6DSO_FIFO_CTRL1        0x07
#define LSM6DSO_FIFO_CTRL2        0x08
#define LSM6DSO_FIFO_CTRL3        0x09
#define LSM6DSO_FIFO_CTRL4        0x0A
#define LSM6DSO_COUNTER_BDR_REG1  0x0B
#define LSM6DSO_COUNTER_BDR_REG2  0x0C
#define LSM6DSO_INT1_CTRL         0x0D
#define LSM6DSO_INT2_CTRL         0x0E
#define LSM6DSO_WHO_AM_I  	      0x0F
#define LSM6DSO_CTRL1_XL  	      0x10
	 //bit[7:4]
typedef enum {
			ODR_XL_OFF                  = (0<<4),
			ODR_XL_1Hz6_LOW_POWER_ONLY  = (11<<4),
			ODR_XL_12Hz5                = (1<<4),
			ODR_XL_26Hz                 = (2<<4),
			ODR_XL_52Hz                 = (3<<4),
			ODR_XL_104Hz                = (4<<4),
			ODR_XL_208Hz                = (5<<4),
			ODR_XL_416Hz                = (6<<4),
			ODR_XL_833Hz                = (7<<4),
			ODR_XL_1k66Hz               = (8<<4),
			ODR_XL_3k33Hz               = (9<<4),
			ODR_XL_6k66Hz               = (10<<4),
} lsm6dso_odr_xl_t;
		//bit[3:2]
typedef enum {
			FS_XL_2g                    = (0<<2),
			FS_XL_4g                    = (2<<2),
			FS_XL_8g                    = (3<<2),
			FS_XL_16g_SPIT_OIS_IU_FS    = (1<<2), /* check  XL_FS_MODE = ‘0’ in CTRL8_XL (17h) */
} lsm6dso_fs_xl_t;
#define ACC_LSB_2G_PER       0.061
#define ACC_LSB_4G_PER       0.122
#define ACC_LSB_8G_PER       0.244
#define ACC_LSB_16G_PER      0.488

#define LSM6DSO_CTRL2_G   	 0x11
	  //bit[7:4]
		typedef enum {
			ODR_G_OFF                  = (0<<4),
			ODR_G_12Hz5                = (1<<4),
			ODR_G_26Hz                 = (2<<4),
			ODR_G_52Hz                 = (3<<4),
			ODR_G_104Hz                = (4<<4),
			ODR_G_208Hz                = (5<<4),
			ODR_G_416Hz                = (6<<4),
			ODR_G_833Hz                = (7<<4),
			ODR_G_1k66Hz               = (8<<4),
			ODR_G_3k33Hz               = (9<<4),
			ODR_G_6k66Hz               = (10<<4),
			ODR_G_NOT_AVAILABLE        = (11<<4),
} lsm6dso_odr_g_t;
		//bit[3:2]
typedef enum {
			FS_G_250                   = (0<<2),
			FS_G_500                   = (1<<2),
			FS_G_1000                  = (2<<2),
			FS_G_2000                  = (3<<2),
} lsm6dso_fs_g_t;
#define GYR_LSB_250_PER        8.75
#define GYR_LSB_500_PER        17.50
#define GYR_LSB_1000_PER       35.0
#define GYR_LSB_2000_PER       70.0

#define LSM6DSO_CTRL3_C   	     0x12
#define LSM6DSO_CTRL4_C   	     0x13
#define LSM6DSO_CTRL5_C  	   	 0x14
#define LSM6DSO_CTRL6_C  	     0x15
#define LSM6DSO_CTRL7_G          0x16
#define LSM6DSO_CTRL8_XL         0x17
#define LSM6DSO_CTRL9_XL         0x18
#define LSM6DSO_CTRL10_C         0x19
#define LSM6DSO_ALL_INT_SRC      0x1A
#define LSM6DSO_WAKE_UP_SRC      0x1B
#define LSM6DSO_TAP_SRC          0x1C
#define LSM6DSO_D6D_SRC          0x1D
#define LSM6DSO_STATUS_REG       0x1E
#define TEM_DATA_AVAILABLE   (1<<2)
#define GYR_DATA_AVAILABLE   (1<<1)
#define ACC_DATA_AVAILABLE   (1<<0)

#define LSM6DSO_OUT_TEMP_L       0x20
#define LSM6DSO_OUT_TEMP_H       0x21
#define TEMP_LSB_PER_DEG     256.0
#define TEMP_OFFSET_DEG      25

#define LSM6DSO_OUTX_L_G     0x22
#define LSM6DSO_OUTX_H_G     0x23
#define LSM6DSO_OUTY_L_G     0x24
#define LSM6DSO_OUTY_H_G     0x25
#define LSM6DSO_OUTZ_L_G     0x26
#define LSM6DSO_OUTZ_H_G     0x27

#define LSM6DSO_OUTX_L_A  	 0x28
#define LSM6DSO_OUTX_H_A     0x29
#define LSM6DSO_OUTY_L_A     0x2A
#define LSM6DSO_OUTY_H_A     0x2B
#define LSM6DSO_OUTZ_L_A     0x2C
#define LSM6DSO_OUTZ_H_A     0x2D


void LSM6DSOInit(void){
	if(LSM6DSO_ID==LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_WHO_AM_I))//设备ID
		{
			printf("device exist\r\n");
		}
	else
		{
			printf("device not exist\r\n");
		}

	LSM6DSOWriteByteReg(LSM6DSO_ADDRESS,LSM6DSO_CTRL1_XL,(uint8_t)(ODR_XL_104Hz|FS_XL_2g));
	LSM6DSOWriteByteReg(LSM6DSO_ADDRESS,LSM6DSO_CTRL2_G,(uint8_t)(ODR_G_104Hz|FS_G_250));

}
void LSM6DSOWriteByteReg(uint8_t addr,uint8_t reg,uint8_t* data){
	HAL_I2C_Mem_Write(&hi2c1, addr, reg, sizeof(addr), data, sizeof(data), 0xff);
	//HAL_I2C_Master_Receive(&hi2c1, addr, data, sizeof(data), 0xffff);
	//HAL_I2C_Master_Transmit(&hi2c1, reg, data, sizeof(reg), 0xffff);
}
uint8_t LSM6DSOReadReg(uint8_t addr,uint8_t reg){
	//HAL_I2C_Mem_Read(&hi2c1, addr, reg, sizeof(reg), readbuf, sizeof(readbuf), 0xff);
	uint8_t readbuf=HAL_I2C_Master_Receive(&hi2c1, addr, &reg, sizeof(reg), 0xffff);
	//printf("Readbuf is %d\r\n",readbuf);
	return readbuf;
}
void LSM6DSOPolling(void)
{


	HAL_Delay(1000);//1s读取一次
	{
		uint8_t status=0;
		int16_t temp_raw;
		float temp_deg;
		int16_t acc_x_raw ,acc_y_raw, acc_z_raw, gyr_x_raw, gyr_y_raw, gyr_z_raw;
	    float acc_x ,acc_y, acc_z, gyr_x, gyr_y, gyr_z;

		status=LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_STATUS_REG);//读状态

		if(status & TEM_DATA_AVAILABLE)
		{
			temp_raw=LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUT_TEMP_L)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUT_TEMP_H)<<8);

			temp_deg=temp_raw/TEMP_LSB_PER_DEG+TEMP_OFFSET_DEG;
			printf("temp=%f\r\n",temp_deg);
		}

		if(status & GYR_DATA_AVAILABLE)
		{
		    //原始数据
			gyr_x_raw = LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTX_L_G)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTX_H_G)<<8);
			gyr_y_raw = LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTY_L_G)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTY_H_G)<<8);
			gyr_z_raw = LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTZ_L_G)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTZ_H_G)<<8);
			//原始数据乘以量程
			gyr_x = gyr_x_raw*GYR_LSB_250_PER;
			gyr_y = gyr_y_raw*GYR_LSB_250_PER;
			gyr_z = gyr_z_raw*GYR_LSB_250_PER;

			printf("gyr_x=%f mdps,gyr_y=%f mdps,gyr_z=%f mdps\r\n",gyr_x,gyr_y,gyr_z);
		}

		if(status & ACC_DATA_AVAILABLE)
		{
		   //原始数据
			acc_x_raw=LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTX_L_A)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTX_H_A)<<8);
			acc_y_raw=LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTY_L_A)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTY_H_A)<<8);
			acc_z_raw=LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTZ_L_A)|(LSM6DSOReadReg(LSM6DSO_ADDRESS,LSM6DSO_OUTZ_H_A)<<8);
			//原始数据乘以量程
			acc_x=acc_x_raw*ACC_LSB_2G_PER;
			acc_y=acc_y_raw*ACC_LSB_2G_PER;
			acc_z=acc_z_raw*ACC_LSB_2G_PER;
			printf("acc_x=%f mg,acc_y=%f mg,,acc_z=%f mg\r\n",acc_x,acc_y,acc_z);
		}


	}
}

