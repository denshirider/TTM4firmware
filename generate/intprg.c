
/************************************************************************
*
* Device     : RX/RX200/RX220
*
* File Name  : intprg.c
*
* Abstract   : Interrupt Program.
*
* History    : 1.00  (2012-11-30)  [Hardware Manual Revision : 1.00]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2012 Renesas Electronics Corporation and
* Renesas Solutions Corp. All rights reserved.
*
************************************************************************/

#include <machine.h>
#include "vect.h"
#include "stdint.h"
#include "../src/interface.h"
#include "../src/MCP23017.h"
#include "../src/SevenSegment.h"
#include "../src/E2ROM.h"
#include "../src/queue.h"
#pragma section IntPRG

// Exception(Supervisor Instruction)
void Excep_SuperVisorInst(void){/* brk(){  } */}

// Exception(Undefined Instruction)
void Excep_UndefinedInst(void){/* brk(){  } */}

// NMI
void NonMaskableInterrupt(void){/* brk(){  } */}

// Dummy
void Dummy(void){/* brk(){  } */}

// BRK
void Excep_BRK(void){ wait(); }

// BSC BUSERR
void Excep_BSC_BUSERR(void){ }

// FCU FIFERR
void Excep_FCU_FIFERR(void){ }

// FCU FRDYI
void Excep_FCU_FRDYI(void){ }

// ICU SWINT
void Excep_ICU_SWINT(void){ }

// CMT0 CMI0
void Excep_CMT0_CMI0(void){
	//1msタイマー割り込み
	timecnt++;//ただカウントしてるだけ
	//ボタンの長押し,クリック判定
	RXbutton.value=!(((*((uint8_t*)RXbutton.portaddress))&(0x01<<RXbutton.bit))>>RXbutton.bit);
	if(RXbutton.value==1) RXbutton.holdcnt++;
	else {RXbutton.holdcnt=0; RXbutton.holdvalue=0;}
	if(RXbutton.value==0) RXbutton.noholdcnt++;
	else RXbutton.noholdcnt=0;
	if(RXbutton.holdcnt>=RXbutton.holdtime) RXbutton.holdvalue=1;
	if(RXbutton.holdcnt>=RXbutton.clicktime) RXbutton.flag=1;
	if(RXbutton.flag==1 && RXbutton.value==0){
		RXbutton.flag=0;
		RXbutton.clickvalue=1;
	}
	if(RXbutton.noholdcnt>=RXbutton.clicktime) RXbutton.clickvalue=0;

	if(SCI1.SSR.BIT.TEND==1){//コントローラとの通信
		uint8_t data;
		if(QueueIsEmpty(&PCdata.TRbuf) == 0){
			LED3 = !LED3;
			QueueDeque(&PCdata.TRbuf, &data);
			SCI1.TDR = data;
		}
	}
	if(QueueIsFull(&PCdata.REbuf)) QueueEmpty(&PCdata.REbuf);//バッファがあふれた時の処置
	if(QueueIsFull(&PCdata.TRbuf)) QueueEmpty(&PCdata.TRbuf);

	if(PORT4.PDR.BIT.B1 == 1){//AD0ピンのショートチェック
		if(ADi1 != ADo1) change_IO_ADpin(0);
	}
	if(PORTB.PDR.BIT.B3 == 1){//IR0ピンのショートチェック
		if(IRi1 != IRo1) change_IO_INpin(0);
	}

}

// CMT1 CMI1
void Excep_CMT1_CMI1(void){
	//10msタイマー割り込み
	LED1=!LED1;
	PCCOM_decode(&PCdata);//コントローラとの通信

	//7セグメント表示
	update7seg();
	//setstr7seg(1,"SPD");
	//setint7segHEX2(0,IRU,IRD);
	//setint7segDEC(1,OUTreg);
	//pc_printf("%x,\n",RIIC0.ICSR2.BYTE);
}

// CMT2 CMI2
void Excep_CMT2_CMI2(void){
	//5msタイマー割込み//7セグメント表示用
	display7seg(&sevenseg);
	MCP23017_1_update();
}

// CMT3 CMI3
void Excep_CMT3_CMI3(void){ }

// CAC FERRF
void Excep_CAC_FERRF(void){ }

// CAC MENDF
void Excep_CAC_MENDF(void){ }

// CAC OVFF
void Excep_CAC_OVFF(void){ }

// RSPI0 SPEI0
void Excep_RSPI0_SPEI0(void){ }

// RSPI0 SPRI0
void Excep_RSPI0_SPRI0(void){ }

// RSPI0 SPTI0
void Excep_RSPI0_SPTI0(void){ }

// RSPI0 SPII0
void Excep_RSPI0_SPII0(void){ }

// DOC DOPCF
void Excep_DOC_DOPCF(void){ }

// CMPB CMPB0
void Excep_CMPB_CMPB0(void){ }

// CMPB CMPB1
void Excep_CMPB_CMPB1(void){ }

// RTC CUP
void Excep_RTC_CUP(void){ }

// ICU IRQ0
void Excep_ICU_IRQ0(void){ }

// ICU IRQ1
void Excep_ICU_IRQ1(void){ }

// ICU IRQ2
void Excep_ICU_IRQ2(void){
	//writeSW押下割り込み/
	uint16_t data=(MCP23017_read(0,GPIOA) | (MCP23017_read(0,GPIOB))<<8);
	Program[PCaddress]=data;

}

// ICU IRQ3
void Excep_ICU_IRQ3(void){ }

// ICU IRQ4
void Excep_ICU_IRQ4(void){
	//CLK立ち上がり割り込み
	//LED3=!LED3;

}

// ICU IRQ5
void Excep_ICU_IRQ5(void){ }

// ICU IRQ6
void Excep_ICU_IRQ6(void){ }

// ICU IRQ7
void Excep_ICU_IRQ7(void){ }

// LVD LVD1
void Excep_LVD_LVD1(void){ }

// LVD LVD2
void Excep_LVD_LVD2(void){ }

///##############################################
// CMPA CMPA1
void Excep_CMPA_CMPA1(void){ }

// CMPA CMPA2
void Excep_CMPA_CMPA2(void){ }
///##############################################

// RTC ALM
void Excep_RTC_ALM(void){ }

// RTC PRD
void Excep_RTC_PRD(void){ }

// S12AD S12ADI0
void Excep_S12AD_S12ADI0(void){ }

// S12AD GBADI
void Excep_S12AD_GBADI(void){ }

// ELC ELSR18I
void Excep_ELC_ELSR18I(void){ }

// ELC ELSR19I
void Excep_ELC_ELSR19I(void){ }

// MTU0 TGIA0
void Excep_MTU0_TGIA0(void){ }

// MTU0 TGIB0
void Excep_MTU0_TGIB0(void){ }

// MTU0 TGIC0
void Excep_MTU0_TGIC0(void){ }

// MTU0 TGID0
void Excep_MTU0_TGID0(void){ }

// MTU0 TCIV0
void Excep_MTU0_TCIV0(void){ }

// MTU0 TGIE0
void Excep_MTU0_TGIE0(void){ }

// MTU0 TGIF0
void Excep_MTU0_TGIF0(void){ }

// MTU1 TGIA1
void Excep_MTU1_TGIA1(void){ }

// MTU1 TGIB1
void Excep_MTU1_TGIB1(void){ }

// MTU1 TCIV1
void Excep_MTU1_TCIV1(void){ }

// MTU1 TCIU1
void Excep_MTU1_TCIU1(void){ }

// MTU2 TGIA2
void Excep_MTU2_TGIA2(void){ }

// MTU2 TGIB2
void Excep_MTU2_TGIB2(void){ }

// MTU2 TCIV2
void Excep_MTU2_TCIV2(void){ }

// MTU2 TCIU2
void Excep_MTU2_TCIU2(void){ }

// MTU3 TGIA3
void Excep_MTU3_TGIA3(void){ }

// MTU3 TGIB3
void Excep_MTU3_TGIB3(void){ }

// MTU3 TGIC3
void Excep_MTU3_TGIC3(void){ }

// MTU3 TGID3
void Excep_MTU3_TGID3(void){ }

// MTU3 TCIV3
void Excep_MTU3_TCIV3(void){ }

// MTU4 TGIA4
void Excep_MTU4_TGIA4(void){ }

// MTU4 TGIB4
void Excep_MTU4_TGIB4(void){ }

// MTU4 TGIC4
void Excep_MTU4_TGIC4(void){ }

// MTU4 TGID4
void Excep_MTU4_TGID4(void){ }

// MTU4 TCIV4
void Excep_MTU4_TCIV4(void){ }

// MTU5 TGIU5
void Excep_MTU5_TGIU5(void){ }

// MTU5 TGIV5
void Excep_MTU5_TGIV5(void){ }

// MTU5 TGIW5
void Excep_MTU5_TGIW5(void){ }

// POE OEI1
void Excep_POE_OEI1(void){ }

// POE OEI2
void Excep_POE_OEI2(void){ }

// TMR0 CMIA0
void Excep_TMR0_CMIA0(void){ }

// TMR0 CMIB0
void Excep_TMR0_CMIB0(void){ }

// TMR0 OVI0
void Excep_TMR0_OVI0(void){ }

// TMR1 CMIA1
void Excep_TMR1_CMIA1(void){ }

// TMR1 CMIB1
void Excep_TMR1_CMIB1(void){ }

// TMR1 OVI1
void Excep_TMR1_OVI1(void){
	//CLKCNTオーバーフロー割り込み
	//pc_printf("\033[4;Hoverflow!!");
	LED2=!LED2;

}

// TMR2 CMIA2
void Excep_TMR2_CMIA2(void){ }

// TMR2 CMIB2
void Excep_TMR2_CMIB2(void){ }

// TMR2 OVI2
void Excep_TMR2_OVI2(void){ }

// TMR3 CMIA3
void Excep_TMR3_CMIA3(void){ }

// TMR3 CMIB3
void Excep_TMR3_CMIB3(void){ }

// TMR3 OVI3
void Excep_TMR3_OVI3(void){ }

// DMAC DMAC0I
void Excep_DMAC_DMAC0I(void){ }

// DMAC DMAC1I
void Excep_DMAC_DMAC1I(void){ }

// DMAC DMAC2I
void Excep_DMAC_DMAC2I(void){ }

// DMAC DMAC3I
void Excep_DMAC_DMAC3I(void){ }

// SCI0 ERI0
void Excep_SCI0_ERI0(void){ }

// SCI0 RXI0
void Excep_SCI0_RXI0(void){ }

// SCI0 TXI0
void Excep_SCI0_TXI0(void){ }

// SCI0 TEI0
void Excep_SCI0_TEI0(void){ }

// SCI1 ERI1
void Excep_SCI1_ERI1(void){ }

// SCI1 RXI1
void Excep_SCI1_RXI1(void){
	uint8_t data=0;
	data = SCI1.RDR;
	if(QueueIsFull(&PCdata.REbuf) == 0){
		QueueEnque(&PCdata.REbuf,data);
	}

}

// SCI1 TXI1
void Excep_SCI1_TXI1(void){ }

// SCI1 TEI1
void Excep_SCI1_TEI1(void){ }

// SCI5 ERI5
void Excep_SCI5_ERI5(void){ }

// SCI5 RXI5
void Excep_SCI5_RXI5(void){ }

// SCI5 TXI5
void Excep_SCI5_TXI5(void){ }

// SCI5 TEI5
void Excep_SCI5_TEI5(void){ }

// SCI6 ERI6
void Excep_SCI6_ERI6(void){ }

// SCI6 RXI6
void Excep_SCI6_RXI6(void){ }

// SCI6 TXI6
void Excep_SCI6_TXI6(void){ }

// SCI6 TEI6
void Excep_SCI6_TEI6(void){ }

// SCI8 ERI8
void Excep_SCI8_ERI8(void){ }

// SCI8 RXI8
void Excep_SCI8_RXI8(void){ }

// SCI8 TXI8
void Excep_SCI8_TXI8(void){ }

// SCI8 TEI8
void Excep_SCI8_TEI8(void){ }

// SCI9 ERI9
void Excep_SCI9_ERI9(void){ }

// SCI9 RXI9
void Excep_SCI9_RXI9(void){ }

// SCI9 TXI9
void Excep_SCI9_TXI9(void){ }

// SCI9 TEI9
void Excep_SCI9_TEI9(void){ }

// SCI12 ERI12
void Excep_SCI12_ERI12(void){ }

// SCI12 RXI12
void Excep_SCI12_RXI12(void){ }

// SCI12 TXI12
void Excep_SCI12_TXI12(void){ }

// SCI12 TEI12
void Excep_SCI12_TEI12(void){ }

// SCI12 SCIX0
void Excep_SCI12_SCIX0(void){ }

// SCI12 SCIX1
void Excep_SCI12_SCIX1(void){ }

// SCI12 SCIX2
void Excep_SCI12_SCIX2(void){ }

// SCI12 SCIX3
void Excep_SCI12_SCIX3(void){ }

// RIIC0 EEI0
void Excep_RIIC0_EEI0(void){
	LED2 = !LED2;
	IEN(RIIC0, EEI0) = 0;                  //割り込み無効

	do{RIIC0.ICSR2.BIT.TMOF = 0;
    }while(RIIC0.ICSR2.BIT.TMOF!=0);
	do{RIIC0.ICSR2.BIT.AL = 0;
    }while(RIIC0.ICSR2.BIT.AL!=0);
	do{RIIC0.ICSR2.BIT.START = 0;
    }while(RIIC0.ICSR2.BIT.START!=0);
	do{RIIC0.ICSR2.BIT.STOP = 0;
    }while(RIIC0.ICSR2.BIT.STOP!=0);
	do{RIIC0.ICSR2.BIT.NACKF = 0;
    }while(RIIC0.ICSR2.BIT.NACKF!=0);

//	RIIC0.ICSR2.BIT.TMOF = 0;
//	RIIC0.ICSR2.BIT.AL = 0;
//	RIIC0.ICSR2.BIT.START = 0;
//	RIIC0.ICSR2.BIT.STOP = 0;
//	RIIC0.ICSR2.BIT.NACKF = 0;
}

// RIIC0 RXI0
void Excep_RIIC0_RXI0(void){ }

// RIIC0 TXI0
void Excep_RIIC0_TXI0(void){ }

// RIIC0 TEI0
void Excep_RIIC0_TEI0(void){ }

