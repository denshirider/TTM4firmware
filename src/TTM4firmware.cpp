/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.cpp                                   */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {

#include "interface.h"
#include "typedefine.h"
#include "MCP23017.h"
#include "E2ROM.h"
#include "SevenSegment.h"

void abort(void);
}
#endif

void main(void)
{
	pc_printf("\033[;HRX220 start\n");
	TTM4init();
	MCP23017_0_INmode();//change input mode
	fld_init_pclk_notification();// peripheral clock notification
	Hangar=(SW2|SW1<<1);//DIPSWが示す格納庫番号を登録
	RXbutton.ini(&RXbutton,(uint32_t)(&PORT3.PIDR),5,1000,50);//P35 is USERBTN
	QueueAlloc(&PCdata.REbuf, 64);
	QueueAlloc(&PCdata.TRbuf, 64);
	setup7seg();


	//初期化が終わってからタイマー割り込みのカウントを開始する
    CMT.CMSTR0.BIT.STR0 = 1;                // CMT0 カウント開始
    CMT.CMSTR0.BIT.STR1 = 1;                // CMT1 カウント開始
    delay_us(100);//初期化後少し待つ

	if(RXbutton.value==1){//タクトスイッチを押しながら起動したら
		MCP23017_0_INmode();
		EN_DSW(1);
		LoadProgram(Hangar,Program);//ROMから今保存されているプログラムを読み出す
		ICU.IER[0x08].BIT.IEN2=1;//writeSW割り込み有効
		while(1){
		    CMT.CMSTR1.BIT.STR2 = 1;				// CMT2　カウント開始
			LED2=!LED2;
			delay_ms(500);
			if(RXbutton.holdvalue==1){//長押ししたら
			    CMT.CMSTR1.BIT.STR2 = 0;				// CMT2　カウント停止
			    delay_ms(10);
				LED2=0;
				SaveProgram(Hangar,Program);
				delay_ms(500);//ROMへ保存完了したことを視覚的に知らせる
				LED2=1;
				delay_ms(100);
				LED2=0;
				delay_ms(100);
				LED2=1;
				delay_ms(100);
				LED2=0;
				delay_ms(100);
				LED2=1;
				delay_ms(100);
				LED2=0;
				delay_ms(500);
			}
		}
	}
	else{
		int i=0;
		LoadProgram(Hangar,Program);
		for(i=255;i>=0;i--){
			pc_printf("%3d,%4x\n",i,Program[i]);
		}
		if(PCaddress!=0x00){//アドレススイッチがonになっている
			pc_printf("エラー　アドレススイッチをOFFにして下さい\n");
			LED2=1;
			delay_ms(500);
			LED2=0;
			delay_ms(500);
			LED2=1;
			delay_ms(500);
			LED2=0;
			delay_ms(500);
			LED2=1;
			delay_ms(500);
			LED2=0;
			delay_ms(500);
		}
		else TTM4_BOOT(Program);

	}
	int i=0;
    while(1){
		PCCOM_process(&PCdata);
		delay_us(100);
	}

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
