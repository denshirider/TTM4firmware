
/************************************************************************
*
* Device     : RX/RX200/RX220
*
* File Name  : hwsetup.c
*
* Abstract   : Hardware Setup file.
*
* History    : 1.00  (2012-11-30)  [Hardware Manual Revision : 1.00]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2012 Renesas Electronics Corporation and
* Renesas Solutions Corp. All rights reserved.
*
************************************************************************/

#include "iodefine.h"
#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif

void CLKinit(void);
void SCIinit(void);
void RIICinit(void);
void GPIOinit(void);
void CMTinit(void);
void TMRinit(void);
void RTCinit(void);

void HardwareSetup(void)
{
	CLKinit();
	SCIinit();
	RIICinit();
	GPIOinit();
	CMTinit();
	TMRinit();
	//RTCinit();//要サブクリスタル

}


void CLKinit(void){
	//周辺クロックPCLK 32MHz
	//システムクロック 32MHz
	//FLASHIFクロック32MHz
	SYSTEM.PRCR.WORD = 0xa50b; //クロックソース選択の保護の解除
	SYSTEM.OPCCR.BIT.OPCM = 0x02; //動作モードの設定。中速
	/*クロックソースの選択*/
	//SYSTEM.SCKCR.BIT.PCKD = 0x00; //周辺クロックD//12AD用
	SYSTEM.SCKCR.BIT.PCKB = 0x00; //周辺クロックB//12AD以外用PCLK
	//SYSTEM.SCKCR.BIT.BCK = 0x00; //外部バスクロック
	SYSTEM.SCKCR.BIT.ICK = 0x00; //システムクロック
	SYSTEM.SCKCR.BIT.FCK = 0x00; //FlashIFクロック
	/*HOCO設定*/
	SYSTEM.HOCOCR2.BIT.HCFRQ=0;//32Mで動作
	SYSTEM.HOCOPCR.BIT.HOCOPCNT=0;//HOCO電源ON
	/*クロックの元栓の設定*/
	SYSTEM.SCKCR3.BIT.CKSEL=0x01;//大元のクロックに内蔵高周波クロックを使用する
	//SYSTEM.SCKCR3.WORD = 0x0200; //大元のクロックにメインクロックを使用する。
	//SYSTEM.SCKCR3.WORD = 0x0300; //大元のクロックにサブクロックを使用する。

	//SYSTEM.MOSCCR.BYTE = 0; //メインクロック発振器動作
	SYSTEM.MOSCCR.BYTE = 1; //メインクロック発振器停止
	SYSTEM.SOSCCR.BIT.SOSTP = 1; //サブクロック発振器停止//サブクリスタルつけ忘れた
	//SYSTEM.SOSCCR.BYTE = 0; //サブクロック発振器動作
	//SYSTEM.LOCOCR.BYTE = ; //LOCO 動作
	SYSTEM.HOCOCR.BYTE = 0; //HOCO 動作
	//SYSTEM.ILOCOCR.BYTE =; //IWDT 専用オシレータ停止
	SYSTEM.PRCR.WORD = 0xA500;  //プロテクト
}
void SCIinit(void){
	//SCI1//PC通信
	SYSTEM.PRCR.WORD = 0xA502;	//プロテクト解除
	MSTP(SCI1) = 0;				//SCI1起動
	SYSTEM.PRCR.WORD = 0xA500;	//プロテクト
	//SCI1割り込み動作等停止
	SCI1.SCR.BYTE  &= 0x0B;
	//IOポート設定
	MPC.PWPR.BIT.B0WI = 0;      //プロテクト解除
    MPC.PWPR.BIT.PFSWE = 1;		//プロテクト解除
    MPC.P26PFS.BIT.PSEL = 0x0A; //P26/TXD1
    MPC.P30PFS.BIT.PSEL = 0x0A; //P30/RXD1
    MPC.PWPR.BIT.PFSWE = 0;     //プロテクト
    MPC.PWPR.BIT.B0WI = 1;		//プロテクト
    PORT2.PMR.BIT.B6 = 1;       //P26/周辺
    PORT3.PMR.BIT.B0 = 1;       //P30/周辺

    SCI1.SCR.BYTE = 0x00;       //SCR初期化
    SCI1.SMR.BYTE = 0x00;		//調歩同期式，8ビットデータ，パリティ無し，ストップビット長1ビット，PCLK/1で動作
	SCI1.BRR = 8;				// 115200bps
	SCI1.SCR.BYTE = 0xF0;       //送受信割り込み許可、送受信許可
    //IPR(SCI1, TXI1) = 1;        //優先度
    //IEN(SCI1, TXI1) = 1;        //割り込み有効
    IPR(SCI1, RXI1) = 1;        //優先度
    IEN(SCI1, RXI1) = 1;        //割り込み有効

	//SCI5 TTM4_UART
	SYSTEM.PRCR.WORD = 0xA502;	//プロテクト解除
	MSTP(SCI5) = 0;				//SCI5起動
	SYSTEM.PRCR.WORD = 0xA500;	//プロテクト
	//SCI5割り込み動作等停止
	SCI5.SCR.BYTE  &= 0xB;
	//IOポート設定
	MPC.PWPR.BIT.B0WI = 0;      //プロテクト解除
    MPC.PWPR.BIT.PFSWE = 1;		//プロテクト解除
    MPC.PC3PFS.BIT.PSEL = 0x0A; //PC3/TXD5
    MPC.PC2PFS.BIT.PSEL = 0x0A; //PC2/RXD5
    MPC.PWPR.BIT.PFSWE = 0;     //プロテクト
    MPC.PWPR.BIT.B0WI = 1;		//プロテクト
    PORTC.PMR.BIT.B3 = 1;       //PC3/周辺
    PORTC.PMR.BIT.B2 = 1;       //PC2/周辺

    SCI5.SCR.BYTE = 0x00;       //SCR初期化
    SCI5.SMR.BYTE = 0x00;		//調歩同期式，8ビットデータ，パリティ無し，ストップビット長1ビット，PCLK/1で動作
	SCI5.BRR = 8;				// 115200bps
	SCI5.SCR.BYTE = 0xF0;       //送受信割り込み許可、送受信許可

}
void RIICinit(void){
	SYSTEM.PRCR.WORD = 0xA502;	//プロテクト解除
	MSTP(RIIC0) = 0;				//RIIC起動
	SYSTEM.PRCR.WORD = 0xA500;	//プロテクト
	//IOポート設定
	MPC.PWPR.BIT.B0WI = 0;      //プロテクト解除
    MPC.PWPR.BIT.PFSWE = 1;		//プロテクト解除
    MPC.P16PFS.BIT.PSEL = 0x0F; //P16/SCL
    MPC.P17PFS.BIT.PSEL = 0x0F; //P17/SDA
    MPC.PWPR.BIT.PFSWE = 0;     //プロテクト
    MPC.PWPR.BIT.B0WI = 1;		//プロテクト
    PORT1.PMR.BIT.B6 = 1;       //P16/周辺
    PORT1.PMR.BIT.B7 = 1;       //P17/周辺

    do{    RIIC0.ICCR1.BIT.ICE=0;
    }while(RIIC0.ICCR1.BIT.ICE!=0);
    RIIC0.ICCR1.BIT.IICRST=1;
    RIIC0.ICCR1.BIT.ICE=1;
    RIIC0.ICSER.BYTE =0x00;
//    RIIC0.ICMR1.BIT.CKS=0x03;//PCLK/8で動作
    RIIC0.ICMR1.BIT.CKS=0x01;//PCLK/2で動作
    RIIC0.ICBRH.BIT.BRH=0x02;//通信速度の設定
    RIIC0.ICBRL.BIT.BRL=0x03;
    //RIIC0.ICMR2.BYTE = 0x00;//保留
    //RIIC0.ICMR3.BYTE = 0x00;
    //RIIC0.ICFER.BYTE = 0x00;
    RIIC0.ICMR3.BIT.ACKWP=1;//ACKBTへのプロテクト解除
    RIIC0.ICFER.BYTE = 0x7F;//0x69;//機能設定
    RIIC0.ICCR1.BIT.IICRST=0;//内部リセット解除

    RIIC0.ICIER.BYTE = 0x1F;//割り込み許可
    IPR(RIIC0, EEI0) = 1;                      //優先度
    //IEN(RIIC0, EEI0) = 1;//3;                  //割り込み有効
    //IPR(RIIC0, RXI0) = 1;                    //優先度
    //IEN(RIIC0, RXI0) = 1;                    //割り込み有効
    //IPR(RIIC0, TXI0) = 1;                    //優先度
    //IEN(RIIC0, TXI0) = 1;                    //割り込み有効
    //IPR(RIIC0, TEI0) = 1;                    //優先度
    //IEN(RIIC0, TEI0) = 1;                    //割り込み有効
}
void GPIOinit(void){

	//PDR 0入力 1出力
	PORT0.PDR.BIT.B3 = 1;//DSA
	PORT3.PDR.BIT.B7 = 1;//DSB
	PORT3.PDR.BIT.B6 = 1;//DSC
	//PORT3.PDR.BIT.B5 = 0;//USRBTN//NMI端子を使う
	PORT3.PDR.BIT.B2 = 1;//RAMWE
	PORT3.PDR.BIT.B1 = 1;//RAMOE
	//PORT3.PDR.BIT.B0 = 0;//RXD1
	PORT2.PDR.BIT.B7 = 1;//PCOPEN
	//PORT2.PDR.BIT.B6 = 0;//TXD1

	//PORT1.PDR.BIT.B7 = 0;//SDA
	//PORT1.PDR.BIT.B6 = 0;//SCL
	PORT1.PDR.BIT.B5 = 0;//OR7
	PORT1.PDR.BIT.B4 = 0;//OR6
	PORTH.PDR.BIT.B3 = 0;//OR5
	PORTH.PDR.BIT.B2 = 0;//OR4
	PORTH.PDR.BIT.B1 = 0;//OR3
	PORTH.PDR.BIT.B0 = 0;//OR2
	PORT5.PDR.BIT.B5 = 0;//OR1
	PORT5.PDR.BIT.B4 = 0;//OR0
	//PORTC.PDR.BIT.B7 = 0;//
	//PORTC.PDR.BIT.B6 = 0;//
	PORTC.PDR.BIT.B5 = 1;//RST
	PORTC.PDR.BIT.B4 = 1;//CLKEN
	//PORTC.PDR.BIT.B3 = 0;//TXD5
	//PORTC.PDR.BIT.B2 = 0;//RXD5

	PORTB.PDR.BIT.B7 = 0;//IR3
	PORTB.PDR.BIT.B6 = 0;//IR2
	PORTB.PDR.BIT.B5 = 0;//IR1
	PORTB.PDR.BIT.B3 = 0;//IR0
	//PORTB.PDR.BIT.B1 = 0;//CLK
	//PORTB.PDR.BIT.B0 = 0;//
	PORTA.PDR.BIT.B6 = 1;//LED1
	PORTA.PDR.BIT.B4 = 1;//LED2
	PORTA.PDR.BIT.B3 = 1;//LED3
	PORTA.PDR.BIT.B1 = 1;//WREN
	PORTA.PDR.BIT.B0 = 0;//IR7
	PORTE.PDR.BIT.B5 = 0;//IR6
	PORTE.PDR.BIT.B4 = 0;//IR5
	PORTE.PDR.BIT.B3 = 0;//IR4

	PORTE.PDR.BIT.B2 = 0;//AD7
	PORTE.PDR.BIT.B1 = 0;//AD6
	PORTE.PDR.BIT.B0 = 0;//AD5
	PORT4.PDR.BIT.B6 = 0;//AD4
	PORT4.PDR.BIT.B4 = 0;//AD3
	PORT4.PDR.BIT.B3 = 0;//AD2
	PORT4.PDR.BIT.B2 = 0;//AD1
	PORT4.PDR.BIT.B1 = 0;//AD0
	PORT4.PDR.BIT.B0 = 0;//DIPSW1
	PORT0.PDR.BIT.B5 = 0;//DIPSW2

	//open drain
	PORTC.ODR1.BIT.B2 = 1;//PC5//RST
	PORT3.ODR0.BIT.B4 = 1;//P32//RAMWE

	//PMRポートモードはリセット値0なので略

/*
	//外部端子割り込み設定//CLK立ち上がり割り込み
	ICU.IER[0x08].BIT.IEN4=0;//割り込み要求禁止
    ICU.IRQFLTE0.BIT.FLTEN4=1;//デジタルフィルタ無効
	ICU.IRQFLTC0.BIT.FCLKSEL4=0x01;//デジタルフィルタサンプリング周波数
	MPC.PWPR.BIT.B0WI = 0;      //プロテクト解除
    MPC.PWPR.BIT.PFSWE = 1;		//プロテクト解除
    //MPC.PB1PFS.BIT.PSEL = 0x05; //PB1/TMCI0
    MPC.PB1PFS.BIT.ISEL =1;		//割り込み端子として使用
    MPC.PWPR.BIT.PFSWE = 0;     //プロテクト
    MPC.PWPR.BIT.B0WI = 1;		//プロテクト
    PORTB.PDR.BIT.B1 = 0;		//入力
    //PORTB.PMR.BIT.B1 = 0;       //PB1/GPIO
    ICU.IRQCR[4].BIT.IRQMD=0x02;//割り込み要因設定
    ICU.IR[68].BIT.IR=0;	//割り込みフラグを初期化
    ICU.IRQFLTE0.BIT.FLTEN4=1;//デジタルフィルタ有効
    ICU.IPR[68].BIT.IPR=1;	//割り込み優先度設定
    ICU.IER[0x08].BIT.IEN4=1;//割り込み要求有効
*/

	//外部端子割り込み設定//RAM書き込み立ち上がり割り込み
	ICU.IER[0x08].BIT.IEN2=0;//割り込み要求禁止
    ICU.IRQFLTE0.BIT.FLTEN2=1;//デジタルフィルタ無効
	ICU.IRQFLTC0.BIT.FCLKSEL2=0x01;//デジタルフィルタサンプリング周波数
	MPC.PWPR.BIT.B0WI = 0;      //プロテクト解除
    MPC.PWPR.BIT.PFSWE = 1;		//プロテクト解除
    MPC.P32PFS.BIT.ISEL =1;		//割り込み端子として使用
    MPC.PWPR.BIT.PFSWE = 0;     //プロテクト
    MPC.PWPR.BIT.B0WI = 1;		//プロテクト
    //PORT3.PDR.BIT.B2 = 0;		//入力
    //PORT3.PMR.BIT.B2 = 0;       //P32/GPIO
    ICU.IRQCR[2].BIT.IRQMD=0x02;//割り込み要因設定
    ICU.IR[66].BIT.IR=0;	//割り込みフラグを初期化
    ICU.IRQFLTE0.BIT.FLTEN2=1;//デジタルフィルタ有効
    ICU.IPR[66].BIT.IPR=3;	//割り込み優先度設定
    //ICU.IER[0x08].BIT.IEN2=1;//割り込み要求有効//mainで有効にする
}
void CMTinit(void){
	//CMT0 1ms
    SYSTEM.PRCR.WORD = 0xA502;              //プロテクト解除
    MSTP(CMT0) = 0;                         //モジュールストップ解除
    SYSTEM.PRCR.WORD = 0xA500;              //プロテクト
    CMT0.CMCR.BIT.CMIE = 1;                 //割り込み有効
    CMT0.CMCR.BIT.CKS = 0;                  //PCLK/8
    CMT0.CMCOR = 3999;                     //1ms周期
    IPR(CMT0, CMI0) = 1;                    //優先度
    IEN(CMT0, CMI0) = 1;                    //割り込み有効
    //CMT.CMSTR0.BIT.STR0 = 1;                // CMT0 カウント開始

	//CMT1 10ms
    SYSTEM.PRCR.WORD = 0xA502;              //プロテクト解除
    MSTP(CMT1) = 0;                         //モジュールストップ解除
    SYSTEM.PRCR.WORD = 0xA500;              //プロテクト
    CMT1.CMCR.BIT.CMIE = 1;                 //割り込み有効
    CMT1.CMCR.BIT.CKS = 0;                  //PCLK/8
    CMT1.CMCOR = 39999;                     //10ms周期
    IPR(CMT1, CMI1) = 1;                    //優先度
    IEN(CMT1, CMI1) = 1;                    //割り込み有効
    //CMT.CMSTR0.BIT.STR1 = 1;                // CMT1 カウント開始

    //CMT2 5ms
    SYSTEM.PRCR.WORD = 0xA502;              //プロテクト解除
    MSTP(CMT2) = 0;                         //モジュールストップ解除
    SYSTEM.PRCR.WORD = 0xA500;              //プロテクト
    CMT2.CMCR.BIT.CMIE = 1;                 //割り込み有効
    CMT2.CMCR.BIT.CKS = 0;                  //PCLK/8
    CMT2.CMCOR = 4999;//799;                       //5ms周期
    IPR(CMT2, CMI2) = 1;                    //優先度
    IEN(CMT2, CMI2) = 1;                    //割り込み有効
    //CMT.CMSTR1.BIT.STR2 = 1;                // CMT2 カウント開始

}
void TMRinit(void){
	//TTM4CLKをカウントする
	//TMR0.TCNTで8bit,TMR1.TCNT:TMR0.TCNTで16bit
	//IOポート設定
	MPC.PWPR.BIT.B0WI = 0;      //プロテクト解除
    MPC.PWPR.BIT.PFSWE = 1;		//プロテクト解除
    MPC.PB1PFS.BIT.PSEL = 0x05; //PB1/TMCI0
    MPC.PWPR.BIT.PFSWE = 0;     //プロテクト
    MPC.PWPR.BIT.B0WI = 1;		//プロテクト
    PORTB.PDR.BIT.B1 = 0;		//入力
    PORTB.PMR.BIT.B1 = 1;       //PB1/周辺

	SYSTEM.PRCR.WORD = 0xA502;  //プロテクト解除
    MSTP(TMR0) = 0;             //モジュールストップ解除
    MSTP(TMR1) = 0;             //モジュールストップ解除
    SYSTEM.PRCR.WORD = 0xA500;  //プロテクト

    IPR(TMR1, OVI1) = 1;                    //割り込み優先度設定
    IEN(TMR1, OVI1) = 1;                    //割り込み有効
    TMR0.TCORA = 0xFF;
    TMR0.TCR.BYTE= 0x00;
    TMR1.TCR.BYTE= 0x20;//オーバーフロー割り込み有効
    TMR0.TCSR.BYTE= 0x00;
    TMR1.TCSR.BYTE= 0x00;
    TMR0.TCNT = 0;
    TMR1.TCNT = 0;
    TMR0.TCCR.BYTE= 0x01;
    TMR1.TCCR.BYTE= 0x18;//コンペアマッチでカウントで16ビットカウンタとして使う

    /*16ビットカウント用設定
    TMR0.TCR.BYTE= 0x20;//16bitカウンタオーバーフロー割り込み有効
    TMR1.TCR.BYTE= 0x00;
    TMR0.TCSR.BYTE= 0x00;
    TMR1.TCSR.BYTE= 0x00;
    TMR0.TCNT = 0;
    TMR1.TCNT = 0;
    TMR0.TCCR.BYTE= 0x18;
    TMR1.TCCR.BYTE= 0x01;
    */

	//PCLK/8でカウントするdelay他用
	//TMR3.TCNTで8bit,TMR2.TCNT:TMR3.TCNTで16bit

	SYSTEM.PRCR.WORD = 0xA502;  //プロテクト解除
    MSTP(TMR2) = 0;             //モジュールストップ解除
    MSTP(TMR3) = 0;             //モジュールストップ解除
    SYSTEM.PRCR.WORD = 0xA500;  //プロテクト

    IPR(TMR2, OVI2) = 1;                    //割り込み優先度設定
    IEN(TMR2, OVI2) = 1;                    //割り込み有効
    TMR2.TCR.BYTE= 0x20;//16bitカウンタオーバーフロー割り込み有効
    TMR3.TCR.BYTE= 0x00;
    TMR2.TCSR.BYTE= 0x00;
    TMR3.TCSR.BYTE= 0x00;
    TMR2.TCNT = 0;
    TMR3.TCNT = 0;
    TMR2.TCCR.BYTE= 0x18;
    TMR3.TCCR.BYTE= 0x0A;
    }
void RTCinit(void){
	/*クロックとカウントモードの設定*/
	//RTC.RCR3.BIT.RTCDV = 0x01; //低 CL ドライブ能力 RTCDV がヘッダファイルに存在しない
	RTC.RCR3.BIT.RTCEN = 1; //サブクロック発振器動作開始
	RTC.RCR2.BIT.START = 0;//RTC カウントストップ
	while(RTC.RCR2.BIT.START != 0);
	RTC.RCR2.BYTE |= 0x00; //カレンダーカウントモード
	RTC.RCR2.BIT.RESET = 1;//RTC カウンタのリセット
	while(RTC.RCR2.BIT.RESET == 1);//リセット完了の待ち
	/*時刻の初期設定*/
	RTC.RCR2.BIT.START = 0;//RTC カウントストップ
	while(RTC.RCR2.BIT.START == 1);
	RTC.RSECCNT.BYTE = 0; //秒レジスタの初期値
	RTC.RMINCNT.BYTE = 0; //分レジスタの初期値
	RTC.RHRCNT.BYTE = 0; //時レジスタの初期値
	RTC.RWKCNT.BYTE = 0; //曜日レジスタの初期値
	RTC.RDAYCNT.BYTE = 0; //日レジスタの初期値
	RTC.RMONCNT.BYTE = 0; //月レジスタの初期値
	RTC.RYRCNT.WORD = 0; //年レジスタの初期値
	RTC.RCR2.BIT.AADJE = 0;//自動補正機能禁止
	RTC.RCR2.BIT.START = 1;//RTC カウントスタート
	while(RTC.RCR2.BIT.START == 0);
	RTC.RCR1.BYTE = 0x08;//RTC の全割り込み禁止、RTCOUT は 64Hz を出力
	//RTC.RCR2.BIT.RTCOE = 1;//RTCOUT 出力許可
}
