/*
 * I2C.c
 *
 *  Created on: 2018/01/23
 *      Author: USER
 */


#include "I2C.h"


//スタートコンディション発行
int STcondtion(void){
	if(RIIC0.ICCR2.BIT.BBSY==1) return FALSE;//バスビジー状態なら送信失敗
	RIIC0.ICCR2.BIT.ST=1;//スタートコンディション発行要求
	while(RIIC0.ICCR2.BIT.MST!=1&&RIIC0.ICCR2.BIT.TRS!=1);//マスター送信モードへ移行するまで待機
	return TRUE;
}
//リスタートコンディション発行
int RScondtion(void){
	if(RIIC0.ICCR2.BIT.BBSY==1 && RIIC0.ICCR2.BIT.MST==1){
		do{RIIC0.ICCR2.BIT.RS =1;
		}while(RIIC0.ICCR2.BIT.RS !=1);
		return TRUE;
	}
	else return FALSE;
}
//1バイト送信
int SendBYTE(_UBYTE data){
	if(RIIC0.ICSR2.BIT.TDRE==0) return -1;//送信準備が出来てなかったら送信失敗
	RIIC0.ICDRT = data;
	while(RIIC0.ICSR2.BIT.TEND!=1);//送信完了するまで待機
	while(RIIC0.ICSR2.BIT.NACKF!=0);//ACK受信するまで待機あとでここにタイムアウトをいれよう！！
	//while(RIIC0.ICSR2.BIT.NACKF!=0);//ACK受信するまで待機あとでここにタイムアウトをいれよう！！
	return RIIC0.ICSR2.BIT.NACKF;
}
//ストップコンディション発行
int SPcondtion(void){
	if(RIIC0.ICSR2.BIT.TEND!=1) return FALSE;
	RIIC0.ICCR2.BIT.SP =1;//ストップコンディション発行要求
	while(RIIC0.ICSR2.BIT.STOP!=1);//ストップコンディションが発行されるまで待機
	RIIC0.ICSR2.BIT.STOP=0;//次の通信のためにフラグを折っておく
	RIIC0.ICSR2.BIT.NACKF=0;
	return TRUE;
}

int MasterSend(_UBYTE Sad,int len,_UBYTE *data){
	int i=0,cnt=0;
	while(STcondtion()==FALSE);//スタートコンディション発行
	SendBYTE(Sad);
	for(i=0;i<len;i++){
		if(SendBYTE(*(data+i))) break;//ACK返ってこなかったら送信中止
		cnt++;
	}
	while(SPcondtion()==FALSE);//ストップコンディション発行
	return cnt;
}

int MasterRecieve(_UBYTE Sad,int clen,_UBYTE *cdata,int rlen,_UBYTE *rdata){
	/*int i=0,rcnt=0,RE=TRUE;
	volatile char dummy;
	while(STcondtion()==FALSE);//スタートコンディション発行
	SendBYTE(Sad&0xFE);
	for(i=0;i<clen;i++){
		if(SendBYTE(*(cdata+i))) break;//ACK返ってこなかったら送信中止
	}
	RIIC0.ICSR2.BIT.START=0;//フラグを折っておく
	do{RScondtion();//リスタートコンディション発行
	}while(RIIC0.ICSR2.BIT.START!=1);

	SendBYTE(Sad|0x01);

	while(RIIC0.ICSR2.BIT.RDRF!=1);//マスター受信モードに移行するまで待機
	//while(RIIC0.ICCR2.BIT.TRS!=0);//マスター受信モードへ移行するまで待機
	LED1=1;
*/
	int RE=-1;
	int ccnt=0;
	int rcnt=0;
	_UBYTE send;
	volatile _UBYTE dummy;
	while(RIIC0.ICCR2.BIT.BBSY!=0);
	RIIC0.ICCR2.BIT.ST=1;
	if(0<clen){
		while(RIIC0.ICSR2.BIT.NACKF==0){
			if(RIIC0.ICSR2.BIT.TDRE==0){
				continue;
			}
			if(ccnt==0){
				send=Sad&0xFE;
			}
			else{
				send=cdata[ccnt-1];
			}
			RIIC0.ICDRT=send;
			if(ccnt==clen){
				while(RIIC0.ICSR2.BIT.TEND!=1);
				RE=0;
				break;
			}
			ccnt++;
		}
		RIIC0.ICCR2.BIT.RS=1;
		while(RIIC0.ICCR2.BIT.RS!=0);
	}
	while(RIIC0.ICSR2.BIT.TDRE!=1);
	RIIC0.ICDRT=Sad|0x01;
	while(RIIC0.ICSR2.BIT.RDRF!=1);
	////
	if(RIIC0.ICSR2.BIT.NACKF==0){//0ならスレーブが認識されている
		if(rlen == 1){
			RIIC0.ICMR3.BIT.ACKBT=1;//NACKを送信する
		}
		else if(rlen==2){
			RIIC0.ICMR3.BIT.WAIT=1;
		}
		dummy = RIIC0.ICDRR;
		while(rcnt<rlen-1){
			while(RIIC0.ICSR2.BIT.RDRF!=1);
			if(rcnt+1==rlen-2){
				RIIC0.ICMR3.BIT.WAIT=1;
			}
			else if(rcnt+1==rlen-1){
				RIIC0.ICMR3.BIT.ACKBT=1;//NACKを送信する
			}
			rdata[rcnt]=RIIC0.ICDRR;
			rcnt++;
		}
		while(RIIC0.ICSR2.BIT.RDRF!=1);
		RIIC0.ICSR2.BIT.STOP=0;
		RIIC0.ICCR2.BIT.SP=1;
		rdata[rcnt]=RIIC0.ICDRR;
		RIIC0.ICMR3.BIT.WAIT=0;
	}
	else{
		RE=-1;
		RIIC0.ICSR2.BIT.STOP=0;
		RIIC0.ICCR2.BIT.SP=1;
		dummy=RIIC0.ICDRR;
	}
	while(RIIC0.ICSR2.BIT.STOP!=1);
	RIIC0.ICSR2.BIT.NACKF=0;
	RIIC0.ICSR2.BIT.STOP=0;
	return RE;

}
