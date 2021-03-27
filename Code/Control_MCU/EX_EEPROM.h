/*
 * Module:EEPROM.h
 *  Electrically Erasable Programmable Read Only Memory (EEPROM) Driver
 *   Created on:Dec 7, 2020
 *    Author:Ehab
 *     Hardware Abstraction Layer (HAL)
 */

#ifndef EX_EEPROM_H_
#define EX_EEPROM_H_

/*---------------------------------->Common files<----------------------------------------------*/
#include"std_types.h"
#include"common_macros.h"
#include"micro_config.h"

/*------------------------------------>Definitions<-----------------------------------------------*/
#define SUCCESS 1
#define FAILED 0

/*----------------------------------->Public functions prototypes<------------------------------*/
void EEPROM_init();
bool EEPROM_writeByte(uint16, uint8);
uint8 EEPROM_readByte(uint16, uint8*);

#endif /* EX_EEPROM_H_ */
