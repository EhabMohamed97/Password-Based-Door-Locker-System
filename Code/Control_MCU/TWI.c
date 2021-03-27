/*
 * Module:TWI.c
 *  Two Wire Interface (I2C) driver
 *   Created on:Dec 6, 2020
 *    Author:Ehab
 *     MicroController Abstraction Layer (MCAL)
 */

#include"TWI.h"

/*------------------------------------->Public Functions definitions<-------------------------------*/
/* A function to initialize TWI module */
void TWI_init(TWI_config*a_config)
{
	TWBR = (F_CPU / (2 * (a_config->BitRate))) - 8;   /* To select the desired Bit rate */
	TWSR = 0x00;                                      /* To select 1 PreScaler */

	/*
	 * TWI(slave) Address Register
	 * TWGCE=0                       To disable the general call recognition
	 * TWAR=a_config->SlaveAddress   To select the desired address for this MCU as a slave
	 */
	TWAR = ((a_config->SlaveAddress) << 1);

	SET_BIT(TWCR, TWEN);   /* To enable TWI communication */
}   /* End TWI_init() function */

/* A function to make the start condition in the master mode */
void TWI_start()
{
	/*
	 * TWI Control Register
	 * TWINT=1   To clear the interrupt flag to make the start condition or to make sure that it will be set again
	 *             after sending the start condition.
	 * TWSTA=1   To make the start condition to indicate that this MCU is the master
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	while(BIT_IS_CLEAR(TWCR, TWINT));   /* wait until the start condition is sent */

}   /* End TWI_start() function */

/* A function to make the  stop condition */
void TWI_stop()
{
	/*
	 * TWINT=1   To clear the interrupt flag to make the start condition or to make sure that it will be set again
	 *             after sending the stop condition.
	 * TWSTO=1   To make the stop condition to indicate the end of being the master of the bus
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}   /* End TWI_stop() function */

/* A function to make this MCU (Master) write to the bus */
void TWI_write(uint8 a_data)
{
	TWDR = a_data;                      /* To put the desired data in TWDR to send it */

	/*
	 * To clear the interrupt flag to make send data or to make sure that it will be set again
	    after sending the data.
	 */
	 TWCR = (1 << TWINT) | (1 << TWEN);

	while(BIT_IS_CLEAR(TWCR, TWINT));   /* wait until the data is sent */

}   /* End TWI_write() function */

/* A function to read data from the bus (slave mode) and send ACK to the other device */
uint8 TWI_readWithACK()
{
	/*
	 * TWINT=1   To clear the interrupt flag to make the start condition or to make sure that it will be set
	 *             again after reading the data.
	 * TWEA=1    To the Acknowledge bit
	 */
	 TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

	while(BIT_IS_CLEAR(TWCR, TWINT));   /* wait until the data is read */

	return TWDR;                        /* Return the data received in TWDR */
}   /* End TWI_readWithACK() function */

/* A function to read data from the bus (slave mode) and send NACK to the other device */
uint8 TWI_readWithNACK()
{
	/*
	 * TWINT=1   To clear the interrupt flag to make the start condition or to make sure that it will be set
	 *             again after reading the data.
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);

	while(BIT_IS_CLEAR(TWCR, TWINT));   /* wait until the data is read */

	return TWDR;                        /* Return the data received in TWDR */
}   /* End TWI_readWithNACK() function */

/* A function to get TWI operation status */
uint8 TWI_getStatus()
{
	uint8 status;

	status = TWSR & 0xF8;   /* Masking TWSR to read the highest 5 bits only that represent TWI status codes */
	return status;
}   /* End TWI_getStatus() function */
