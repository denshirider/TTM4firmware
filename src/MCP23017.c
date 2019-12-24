/*
 * MCP23017.c
 *
 *  Created on: 2018/01/30
 *      Author: USER
 */

#include "MCP23017.h"

unsigned char GPIO1A = 0;//7segつなぐところ
unsigned char GPIO1B = 0;//スタックアドレスつなぐところ


void MCP23017_write(unsigned char N, unsigned char add,unsigned char data){
	unsigned char SlaveAdd = 0x40 | (N<<1);
	unsigned char send[2]={add,data};
	MasterSend(SlaveAdd,2,send);
}

unsigned char MCP23017_read(unsigned char N, unsigned char add){
	unsigned char SlaveAdd = 0x40 | (N<<1);
	unsigned char RE=0;
	MasterRecieve(SlaveAdd,1,&add,1,&RE);
	return RE;
}

void MCP23017_1_update(void){
	MCP23017_write(1,OLATA,GPIO1A);//Aポートに出力
	GPIO1B = MCP23017_read(1,GPIOB);//Bポートから読み取り
}
