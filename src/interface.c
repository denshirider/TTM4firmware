/*
 * interface.c
 *
 *  Created on: 2018/01/13
 *      Author: USER
 */

#include "interface.h"
#include "queue.h"
#include "E2ROM.h"
#include <stdarg.h>
#include <stdio.h>

unsigned int Hangar=0;//E2ROMのどの格納庫か
uint16_t Program[256]={0};

uint16_t timecnt=0;//10msタイマー割込みでカウントさせとく用


BUTTON RXbutton={NULL,0,0,0,0,0,0,0,0,0,buttonini};

void buttonini(BUTTON* button,uint32_t address,uint8_t bit,uint16_t holdtime,uint16_t clicktime){
	button->portaddress=address;
	button->bit=bit;
	button->flag=0;
	button->value=!(((*((uint8_t*)address))&(0x01<<bit))>>bit);
	button->clickvalue=0;
	button->holdvalue=0;
	button->holdtime=holdtime;
	button->clicktime=clicktime;
}

PCCOMdata PCdata={0};

int PCCOM_decode(PCCOMdata* data){
	static int cnt=-1;//-1の時は受信待ち状態
	static unsigned int sum=0;
	static uint8_t tmpPacket[8]={0};
	uint8_t tmp = 0;
	uint8_t sending[5] = {0};//送り出すデータ

	if(QueueDeque(&data->REbuf,&tmp) == -1) return -1;//データを取り出す
	if(cnt == -1 && tmp == 0x80){//受け取ったデータがパケットの先頭のとき
		cnt=0;
		sum=0;
	}
	if(cnt>=0 && cnt<7){
		sum+=tmp;
		tmpPacket[cnt++]=tmp;
	}
	else if(cnt>=0 && cnt==7){
		sum&=0xFF;//下位8ビットだけ抽出
		tmpPacket[cnt] = tmp;
		if(sum == tmpPacket[cnt]){//チェックサムを確認
			int i=0;
			//QueueEmpty(&data->REbuf);//バッファを空っぽにする
			for(i=0;i<8;i++) data->REpacket[i] = tmpPacket[i];//データ入れる
			if(data->REpacket[1] == 'B'){
				data->COM_OPEN=1;//Beginコマンドなら通信開始
				PCCOM_send(data, sending);
			}
		}
		cnt = -1;
	}
	return 1;
}

void PCCOM_process(PCCOMdata* data){
	uint8_t sending[5] = {0};//送り返すデータ
	int i=0;
	if(data->COM_OPEN == 0) return;//通信開始してなかったらreturn

	switch(data->REpacket[1]){//コマンドの解析
	case 'c'://continue
		if(data->Recv_state != data->REpacket[4]){
			data->Recv_state = data->REpacket[4];
			EN_CLK(data->Recv_state & 0x01);
			EN_RESET((data->Recv_state & 0x02)>>1);
			EN_RAM((data->Recv_state & 0x04)>>2 == 0);
			EN_DSW((data->Recv_state & 0x04)>>2 == 1);
			if((data->Recv_state & 0x08)>>3){
				EN_PC(0);
				change_IO_ADpin(1);
			}
			else if(((data->Recv_state & 0x08)>>3) == 0){
				change_IO_ADpin(0);
				EN_PC(1);
			}
			if((data->Recv_state & 0x10)>>4){
				if(INreg == 0) change_IO_INpin(1);//ショートしないように
			}
			else if(((data->Recv_state & 0x10)>>4) == 0){
				change_IO_INpin(0);
			}
		}
		TTM4_SetAddr(data->REpacket[2]);
		TTM4_SetINreg(data->REpacket[3]);

		sending[0] = PCaddress;
		sending[1] = OUTreg;
		sending[2] = data->REpacket[4];
		PCCOM_send(data, sending);
		break;
	case 'w'://write
		Program[data->REpacket[2]] = (data->REpacket[4] |data->REpacket[3]<<8);
		sending[0]=data->REpacket[2];
		sending[1]=data->REpacket[3];
		sending[2]=data->REpacket[4];
		PCCOM_send(data,sending);
		break;
	case 'r'://read
		sending[0] = data->REpacket[2];
		sending[1] = (Program[data->REpacket[2]] && 0xFF00)>>8;
		sending[2] =  Program[data->REpacket[2]] && 0x00FF;
		PCCOM_send(data, sending);
		break;
	case 's'://save
		if(SaveProgram(Hangar,Program) != FLD_OK){//ROMへ書き込み
			sending[4] |= 0x01;//saveエラーフラグを立てる
		}
		TTM4_BOOT(Program);//RAMへ書き込み
	    PCCOM_send(data, sending);
		break;
	case 'e'://erase
		PCCOM_send(data, sending);
		for(i=0;i<256;i++) Program[i] = 0;
		SaveProgram(Hangar,Program);//ROMをイレーズ
		TTM4_BOOT(Program);//RAMをイレーズ
		break;
	case 'R'://RESET
		PCCOM_send(data, sending);
		TTM4_RESET();
		break;
	case 'E'://END
		data->COM_OPEN=0;
		PCCOM_send(data, sending);
		break;
	default:
		break;
	}
	for(i=0;i<8;i++) data->REpacket[i] = 0;//処理が終わったのでクリアする

}

void PCCOM_send(PCCOMdata* data, uint8_t* sending){
	int i=0;
	uint8_t send=0;
	uint8_t sum=0;
	for(i=0;i<8;i++){
		if(i<=1) send = 0x80;
		else if(i>1 && i<7) send=sending[i-2];
		else if(i==7) send= sum & 0xFF;
		QueueEnque(&data->TRbuf, send);
		sum+=send;
	}
}

void TTM4init(void){
	EN_CLK(0);
	EN_DSW(0);
	EN_PC(0);
	EN_RAM(0);
	RAMOE=1;
	RAMWEo=1;
	RST=1;
}

void write_RAM(void){
	RAMWEo=0;
	delay_us(100);
	RAMWEo=1;
	delay_us(100);
}

void TTM4_RESET(void){
	RST=0;
	delay_ms(800);
	RST=1;
}

void TTM4_SetAddr(uint8_t addr){
	ADo0=(addr&0x01);
	ADo1=(addr&0x02)>>1;
	ADo2=(addr&0x04)>>2;
	ADo3=(addr&0x08)>>3;
	ADo4=(addr&0x10)>>4;
	ADo5=(addr&0x20)>>5;
	ADo6=(addr&0x40)>>6;
	ADo7=(addr&0x80)>>7;
}

void TTM4_SetINreg(uint8_t IN){
	IRo0=(IN&0x01);
	IRo1=(IN&0x02)>>1;
	IRo2=(IN&0x04)>>2;
	IRo3=(IN&0x08)>>3;
	IRo4=(IN&0x10)>>4;
	IRo5=(IN&0x20)>>5;
	IRo6=(IN&0x40)>>6;
	IRo7=(IN&0x80)>>7;
}
void change_IO_ADpin(int io){//0入力1出力
	PORT4.PDR.BIT.B1 = io;//AD0
	PORT4.PDR.BIT.B2 = io;//AD1
	PORT4.PDR.BIT.B3 = io;//AD2
	PORT4.PDR.BIT.B4 = io;//AD3
	PORT4.PDR.BIT.B6 = io;//AD4
	PORTE.PDR.BIT.B0 = io;//AD5
	PORTE.PDR.BIT.B1 = io;//AD6
	PORTE.PDR.BIT.B2 = io;//AD7
}
void change_IO_INpin(int io){//0入力1出力
	PORTB.PDR.BIT.B3 = io;//IR0
	PORTB.PDR.BIT.B5 = io;//IR1
	PORTB.PDR.BIT.B6 = io;//IR2
	PORTB.PDR.BIT.B7 = io;//IR3
	PORTE.PDR.BIT.B3 = io;//IR4
	PORTE.PDR.BIT.B4 = io;//IR5
	PORTE.PDR.BIT.B5 = io;//IR6
	PORTA.PDR.BIT.B0 = io;//IR7

}

void change_7seg(void){

}


int pc_printf(const char *format, ... ){
	char string[128];
	int Return;
	int i;
	va_list ap;
	va_start(ap, format);
	Return = vsprintf(string,format,ap);
	va_end(ap);
	for(i=0;i<=128;){
			if(SCI1.SSR.BIT.TEND==1){
				SCI1.TDR=string[i];
				SCI1.SPMR.BIT.CTSE=0;
				i++;
			}
			if(string[i]==0) break;
	}
	return Return;
}
int SCI5_printf(const char *format, ... ){
	char string[128];
	int Return;
	int i;
	va_list ap;
	va_start(ap, format);
	Return = vsprintf(string,format,ap);
	va_end(ap);
	for(i=0;i<=128;){
			if(SCI5.SSR.BIT.TEND==1){
				SCI5.TDR=string[i];
				SCI5.SPMR.BIT.CTSE=0;
				i++;
			}
			if(string[i]==0) break;
	}
	return Return;
}

void delay_us(unsigned int us){
	int n=5;//動作周波数に合わせて変更
	unsigned int i;
	for(i=0;i<(n*us);i++) nop();
}
void delay_ms(unsigned int ms){
	unsigned int i=0;
	for(i=0;i<ms;i++) delay_us(1000);
}

void TTM4_BOOT(uint16_t *program){
	int i=0;
	CMT.CMSTR1.BIT.STR2 = 0;				// CMT2　カウント停止
	delay_ms(1);
	TTM4init();
	change_IO_ADpin(1);
	MCP23017_0_OUTmode();
	for(i=0;i<256;i++){
		TTM4_SetAddr(i);
		MCP23017_write(0,OLATB,(uint8_t)(program[i]>>8));
		MCP23017_write(0,OLATA,(uint8_t)(program[i]&0x00FF));
		delay_ms(1);
		write_RAM();
	}
	change_IO_ADpin(0);
	MCP23017_0_INmode();
	EN_PC(1);
	EN_RAM(1);
	EN_CLK(1);
	TTM4_RESET();
	delay_ms(1);
	CMT.CMSTR1.BIT.STR2 = 1;				// CMT2　カウント開始
}
