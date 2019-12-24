/*
 * interface.hpp
 *
 *  Created on: 2018/01/19
 *      Author: USER
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "regdefine.h"
#include "MCP23017.h"
#include "queue.h"
#include "typedefine.h"
#include "stdint.h"
#include <machine.h>

#define TRUE 1
#define FALSE 0
#define PCLKfrequency 32

typedef struct button{
	uint32_t portaddress;//ioポートレジスタのアドレス
	uint8_t  bit;//レジスタの何ビット目
	uint8_t  flag;
	uint8_t  value;
	uint8_t  holdvalue;
	uint8_t  clickvalue;
	uint16_t  holdcnt;
	uint16_t  noholdcnt;
	uint16_t  holdtime;//ms
	uint16_t  clicktime;//ms
	void (*ini)(struct button*,uint32_t,uint8_t,uint16_t,uint16_t); //初期化関数
}BUTTON;

extern BUTTON RXbutton;
void buttonini(BUTTON* button,uint32_t address,uint8_t bit,uint16_t holdtime,uint16_t clicktime);

typedef struct {
	Queue REbuf;//生データのバッファ
	Queue TRbuf;
	uint8_t REpacket[8];//受信したパケット
	uint8_t TRpacket[8];//送信するパケット
	int COM_OPEN;//通信状態か否か
	uint8_t Recv_state;
	/*Recv_stateについて
	 * 0x01 CLK_EN//クロックを出力するか否か
	 * 0x02 RESET//リセットの状態
	 * 0x04 Instruction_Data//どこから命令を持ってくるか0:RAM,1:DSW
	 * 0x08 SET_Address_EN//TTM4のプログラムアドレスをコントローラ側で制御するか否か
	 * 0x10 INPUTreg_EN//input_regsterへコントローラから出力するか否か
	 */
}PCCOMdata;

int PCCOM_decode(PCCOMdata*);
void PCCOM_process(PCCOMdata*);
void PCCOM_send(PCCOMdata*, uint8_t*);

extern PCCOMdata PCdata;

extern unsigned int Hangar;//E2ROMのどの格納庫かE2の2kbyte毎に格納する
extern uint16_t Program[256];//TTM4プログラム置き場

extern uint16_t timecnt;//1msタイマー割込みでカウントさせとく用

void TTM4init(void);

void write_RAM(void);
void TTM4_RESET(void);

void TTM4_SetAddr(uint8_t addr);
void TTM4_SetINreg(uint8_t IN);

void change_IO_ADpin(int io);//0入力1出力
void change_IO_INpin(int io);//0入力1出力

void change_7seg(void);

int pc_printf(const char *format, ... );
int SCI5_printf(const char *format, ... );
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);


void TTM4_BOOT(uint16_t *program);


#endif /* INTERFACE_H_ */
