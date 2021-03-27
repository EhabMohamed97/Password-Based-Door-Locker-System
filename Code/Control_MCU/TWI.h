/*
 * TWI.h
 *  Module:Two Wire Interface (I2C) driver
 *   Created on:Dec 6, 2020
 *    Author:Ehab
 *     MicroController Abstraction Layer (MCAL)
 */

#ifndef TWI_H_
#define TWI_H_

/*---------------------------------->Common files<----------------------------------------------*/
#include"std_types.h"
#include"common_macros.h"
#include"micro_config.h"

/*-------------------------------------->Structures definitions<----------------------------------*/
/* TWI  configuration structure  */
typedef struct
{
	uint32 BitRate;        /* To select the TWI speed */
	//uint8 PreScaler;     /* To select the TWI PreScaler(TWPS0 and TWPS1 in TWSR) */
	uint8 SlaveAddress;    /* To give MCU an address */
}TWI_config;

/*------------------------------------------>Definitions<--------------------------------------------*/
/* Status codes for master transmitter definitions */
/* Status codes for master transmitter definitions */
#define START_COND 0x08
#define RE_START_COND 0x10
#define MASTER_W_ADR_SLAVE_ACK 0x18   /* Master transmit slave address+write signal+ACK received */
#define MASTER_R_ADR_SLAVE_ACK 0x40   /* Master transmit slave address+read signal+ACK received */
#define MASTER_W_DATA_ACK 0x28        /* Master transmit data+ACK received */
#define MASTER_W_DATA_NACK 0x30       /* Master transmit data+NACK received */
#define MASTER_R_DATA_ACK 0x50        /* Master read data+send ACK */
#define MASTER_R_DATA_NACK 0x58       /* master read data+send NACK */

/*----------------------------------->Public functions prototypes<------------------------------*/
void TWI_init(TWI_config*);
void TWI_start();
void TWI_stop();
void TWI_write(uint8);
uint8 TWI_readWithACK();
uint8 TWI_readWithNACK();
uint8 TWI_getStatus();
#endif /* TWI_H_ */
