/*
 * I2C.h
 *
 *  Created on: 2018/01/23
 *      Author: USER
 */

#ifndef I2C_H_
#define I2C_H_

#include "iodefine.h"
#include "typedefine.h"
#include "regdefine.h"
#include "interface.h"

int STcondtion(void);//スタートコンディション発行
int RScondtion(void);//リスタートコンディション発行
int SendBYTE(_UBYTE data);//1バイト送信
int SPcondtion(void);//ストップコンディション発行
int MasterSend(_UBYTE Sad,int len,_UBYTE *data);//マスター送信Sadの上位7ビットへアドレスを
int MasterRecieve(_UBYTE Sad,int clen,_UBYTE *cdata,int rlen,_UBYTE *rdata);

#endif /* I2C_H_ */
