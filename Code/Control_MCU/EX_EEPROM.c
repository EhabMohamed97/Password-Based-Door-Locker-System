/*
 * Module:EEPROM.c
 *  Electrically Erasable Programmable Read Only Memory (EEPROM) Driver
 *   Created on:Dec 7, 2020
 *    Author:Ehab
 *     Hardware Abstraction Layer (HAL)
 */

#include "EX_EEPROM.h"
#include"TWI.h"   /* Included in this .c file to hold the abstraction */

TWI_config s_TWIConfig ={400000, 1};   /* A structure to initialize TWI with 400KB/S and take 1 as address */
/*------------------------------------->Public Functions definitions<-------------------------------*/
/* A function to initialize EEPROM */
void EEPROM_init()
{


	TWI_init(&s_TWIConfig);
}   /* End EEPROM_init() function */

/* A function to write one byte in a selected address of EEPROM */
bool EEPROM_writeByte(uint16 address, uint8 data)
{
	TWI_start();                                     /* To transmit the start condition */
	if(TWI_getStatus() != START_COND)return FALSE;   /* making sure that the start condition has been sent */

	TWI_write(((uint8)((address & 0x0700) >> 7)) | 0xA0);        /* send EEPROM Address+highest 3 bits of the word address */
	if(TWI_getStatus() != MASTER_W_ADR_SLAVE_ACK)return FALSE;   /* Making sure that the slave address Matched with EEPROM */

	TWI_write((uint8)address);                              /* send the rest of the word address (Lowest byte) */
	if(TWI_getStatus() != MASTER_W_DATA_ACK)return FALSE;   /* Making sure that the word address has been sent */

	TWI_write(data);                                        /* To send the required data */
	if(TWI_getStatus() != MASTER_W_DATA_ACK)return FALSE;   /* Making sure that the data has been sent */

	TWI_stop();   /* To send the stop condition and release the bus */

	return TRUE;
}   /* End EEPROM_writeByte() function */

/* a function to read one byte from a selected address in EEPROM
 * This function take 2 arguments:-->The address of the EEPROM Location
 *                                -->The location where to save the data
 */
uint8 EEPROM_readByte(uint16 address, uint8*a_location)
{
	TWI_start();                                      /* To transmit the start condition */
	if(TWI_getStatus() != START_COND)return FAILED;   /* making sure that the start condition has sent */

	TWI_write(((uint8)((address & 0x0700) >> 7)) | 0xA0);         /* send EEPROM Address+highest 3 bits of the word address+ write signal */
	if(TWI_getStatus() != MASTER_W_ADR_SLAVE_ACK)return FAILED;   /* Making sure that the slave address Matched with EEPROM */

	TWI_write((uint8)address);                               /* send the rest of the word address (Lowest byte) */
	if(TWI_getStatus() != MASTER_W_DATA_ACK)return FAILED;   /* Making sure that the word address has been sent */

	TWI_start();                                         /* To transmit the repeated start condition */
	if(TWI_getStatus() != RE_START_COND)return FAILED;   /* making sure that the repeated start condition has sent */

	TWI_write(((uint8)((address & 0x0700) >> 7)) | 0xA1);         /* send EEPROM Address+highest 3 bits of the word address+read signal */
	if(TWI_getStatus() != MASTER_R_ADR_SLAVE_ACK)return FAILED;   /* Making sure that the slave address Matched with EEPROM */

	*a_location = TWI_readWithNACK();                     /* Save the Word in the required address by the App */
	if(TWI_getStatus() != MASTER_R_DATA_NACK)return FAILED;   /* Making sure that the data has been read successfully */

	TWI_stop();

	return SUCCESS;   /* To send the stop condition and release the bus */
}   /* End EEPROM_readByte() function */
