/*
 * MCP23017.h
 *
 *  Created on: 2018/01/30
 *      Author: USER
 */

#ifndef MCP23017_H_
#define MCP23017_H_

#include "I2C.h"

#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA  0x12
#define GPIOB  0x13
#define OLATA  0x14
#define OLATB  0x15
#define MCP23017_0_INmode() MCP23017_write(0,IODIRA,0xFF); MCP23017_write(0,IODIRB,0xFF)
#define MCP23017_0_OUTmode() MCP23017_write(0,IODIRA,0x00); MCP23017_write(0,IODIRB,0x00)

#define SPaddress GPIO1B

extern unsigned char GPIO1A;//7segつなぐところ
extern unsigned char GPIO1B;//スタックアドレスつなぐところ


//MCP23017に設定した番号、操作対象レジスタアドレス、データ
void MCP23017_write(unsigned char N, unsigned char Radd,unsigned char data);
unsigned char MCP23017_read(unsigned char N, unsigned char Radd);

//MCP23017_1のGPIOを更新する
void MCP23017_1_update(void);

#endif /* MCP23017_H_ */
