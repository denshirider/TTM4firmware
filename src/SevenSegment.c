/*
 * SevenSegment.c
 *
 *  Created on: 2019/04/29
 *      Author: hamakita
 */

#include "SevenSegment.h"
#include "MCP23017.h"
#include "regdefine.h"
#include "interface.h"

sevensegment sevenseg={0};
int ssEditMode = 0;//表示するデータを編集中か否か0:display,1:edit
int ssDisplayMode = 0;//表示モードがDECかHEXか0:10,1:16
int ssEditDigit = 0;//表示データ編集中の桁,0,1,2,3,4,5


const unsigned char ssd[30] = {//seven segment decode anode common
  0xC0,//0
  0xF9,//1
  0xA4,//2
  0xB0,//3
  0x99,//4
  0x92,//5
  0x82,//6
  0xD8,//7
  0x80,//8
  0x90,//9
  0x88,//A
  0x83,//b
  0xC6,//C
  0xA1,//d
  0x86,//E
  0x8E,//F
  0x82,//G
  0x8B,//h
  0xFB,//I
  0x11,//J
  0x11,//k
  0x11,//L
  0x11,//M
  0xAB,//N
  0xA3,//O
  0x8C,//P
  0x11,//Q
  0x11,//R
  0x93,//S
  0x87 //T
};

void update7seg(void){
	/*
	h　は十六進表示
	d は十進表示
	c はそれらの文字表示
	*/
	enum segmode { PCh, PCd, SPh, SPd, INh, INd, OTh, OTd,
		PChc, PCdc, SPhc, SPdc, INhc, INdc, OThc, OTdc };
	static int segnum0=PCh;//セグメント0が表示中の情報
	static int segnum1=SPh;//セグメント1が表示中の情報
	static int segcnt;//時間の調整
	static int flag=0;
	static int flagbutton=0;

	if(RXbutton.clickvalue == 0) flagbutton=0;
	if(RXbutton.clickvalue && flagbutton ==0){
		flagbutton = 1;//連続して入らないように
		if(flag==0){
			segnum1 = segnum0; //表示中のデータを下のsegに遷移する
			flag=1;
			segnum0 +=8;//文字表示にする
			segnum1 +=8;
		}
		segcnt=0;
		segnum0++; //次のデータを
		if(segnum0==OTdc+1) segnum0=8;
	}
	if(flag==1){
		segcnt++;
		if(segcnt>=300){//1s間操作時間
			flag=0;
			segcnt=0;
			segnum0 -=8;//データ表示にする
			segnum1 -=8;
		}
	}

	//7セグに表示
	switch (segnum0) {
		case PCh:
			setint7segHEX(0, PCaddress);
			break;
		case PCd:
			setint7segDEC(0, PCaddress);
			break;
		case SPh:
			setint7segHEX(0, SPaddress);
			break;
		case SPd:
			setint7segDEC(0, SPaddress);
			break;
		case INh:
			setint7segHEX(0, INreg);
			break;
		case INd:
			setint7segDEC(0, INreg);
			break;
		case OTh:
			setint7segHEX(0, OUTreg);
			break;
		case OTd:
			setint7segDEC(0, OUTreg);
			break;
		case PChc:
			setstr7seg(0, "PCH");
			break;
		case PCdc:
			setstr7seg(0, "PCD");
			break;
		case SPhc:
			setstr7seg(0, "SPH");
			break;
		case SPdc:
			setstr7seg(0, "SPD");
			break;
		case INhc:
			setstr7seg(0, "INH");
			break;
		case INdc:
			setstr7seg(0, "IND");
			break;
		case OThc:
			setstr7seg(0, "OTH");
			break;
		case OTdc:
			setstr7seg(0, "OTD");
			break;
		default:
			break;
	}
	switch (segnum1) {
		case PCh:
			setint7segHEX(1, PCaddress);
			break;
		case PCd:
			setint7segDEC(1, PCaddress);
			break;
		case SPh:
			setint7segHEX(1, SPaddress);
			break;
		case SPd:
			setint7segDEC(1, SPaddress);
			break;
		case INh:
			setint7segHEX(1, INreg);
			break;
		case INd:
			setint7segDEC(1, INreg);
			break;
		case OTh:
			setint7segHEX(1, OUTreg);
			break;
		case OTd:
			setint7segDEC(1, OUTreg);
			break;
		case PChc:
			setstr7seg(1, "PCH");
			break;
		case PCdc:
			setstr7seg(1, "PCD");
			break;
		case SPhc:
			setstr7seg(1, "SPH");
			break;
		case SPdc:
			setstr7seg(1, "SPD");
			break;
		case INhc:
			setstr7seg(1, "INH");
			break;
		case INdc:
			setstr7seg(1, "IND");
			break;
		case OThc:
			setstr7seg(1, "OTH");
			break;
		case OTdc:
			setstr7seg(1, "OTD");
			break;
		default:
			break;
	}

}

void display7seg(sevensegment* dis){
	static int flag = 0;//隣の桁に前の桁が浮き出てしまうので間に表示しない状態をつくる
	if(flag == 1){//表示する
		flag = 0;
		dis->digit++;
		if(dis->digit >= NUMofDigit) dis->digit = 0;
		SelDigit(dis->digit);
		GPIO1A = dis->data[dis->digit];
	}
	else{//表示しないターン
		flag = 1;
		GPIO1A = 0xFF;
	}
}

void setint7segDEC(unsigned int sel, unsigned int d){//表示するデータをセットする
	unsigned int data = d;
	if(sel){
		sevenseg.data[2] = ssd[data % 10]; data /= 10;// 1桁目を取り出す
		sevenseg.data[1] = ssd[data % 10]; data /= 10;// 2桁目を取り出す
		sevenseg.data[0] = ssd[data % 10]; data /= 10;// 3桁目を取り出す
	}
	else{
		sevenseg.data[5] = ssd[data % 10]; data /= 10;// 1桁目を取り出す
		sevenseg.data[4] = ssd[data % 10]; data /= 10;// 2桁目を取り出す
		sevenseg.data[3] = ssd[data % 10]; data /= 10;// 3桁目を取り出す
		}
}
void setint7segHEX(unsigned int sel, unsigned int d){
	unsigned int data = d;
	if(sel){
		sevenseg.data[2] = ssd[data % 16]; data /= 16;// 1桁目を取り出す
		sevenseg.data[1] = ssd[data % 16]; data /= 16;// 2桁目を取り出す
		sevenseg.data[0] = 0xFF;
	}
	else{
		sevenseg.data[5] = ssd[data % 16]; data /= 16;// 1桁目を取り出す
		sevenseg.data[4] = ssd[data % 16]; data /= 16;// 2桁目を取り出す
		sevenseg.data[3] = 0xFF;
		}

}
void setint7segHEX2(unsigned int sel, unsigned int d0, unsigned int d1){
	unsigned int data0 = d0;
	unsigned int data1 = d1;
	if(sel){
		sevenseg.data[2] = ssd[data0]; // データ1を表示
		sevenseg.data[1] = 0xFF;
		sevenseg.data[0] = ssd[data1]; // データ2を表示
	}
	else{
		sevenseg.data[5] = ssd[data0]; // データ1を表示
		sevenseg.data[4] = 0xFF;
		sevenseg.data[3] = ssd[data1]; // データ2を表示
			}
}

void setstr7seg(unsigned int sel, unsigned char* str){//表示するデータをセットするアルファベット大文字3文字
	if(sel){
		sevenseg.data[2] = ssd[(int)*(str+2)-55]; // データを表示
		sevenseg.data[1] = ssd[(int)*(str+1)-55];
		sevenseg.data[0] = ssd[(int)*(str+0)-55];
	}
	else{
		sevenseg.data[5] = ssd[(int)*(str+2)-55]; // データを表示
		sevenseg.data[4] = ssd[(int)*(str+1)-55];
		sevenseg.data[3] = ssd[(int)*(str+0)-55];
			}
}

void SelDigit(unsigned char digit){
	DSA = digit & 0x01;
	DSB = (digit & 0x02)>>1;
	DSC = (digit & 0x04)>>2;
}

void setup7seg(void){
	MCP23017_write(1,IODIRA,0x00);//Aポートを出力へ
	MCP23017_write(1,IODIRB,0xFF);//Bポートを入力へ
}


