/*
 * SevenSegment.h
 *
 *  Created on: 2019/04/29
 *      Author: hamakita
 */

#ifndef SEVENSEGMENT_H_
#define SEVENSEGMENT_H_


//7セグメントダイナミック表示

#define NUMofDigit 6

typedef struct {
	unsigned char data[NUMofDigit];//表示するデータdecode済み
	unsigned char digit;//表示中の桁数
}sevensegment;

extern sevensegment sevenseg;
extern int ssEditMode;//表示するデータを編集中か否か
extern int ssDisplayMode;//表示モードがDECかHEXか
extern int ssEditDigit;//表示データ編集中の桁

void update7seg(void);//表示切替用
//実際に7segに表示するこれをタイマー割込みへ
void display7seg(sevensegment* dis);

void setint7segDEC(unsigned int sel, unsigned int d);//表示するデータをセットする10進
void setint7segHEX(unsigned int sel, unsigned int d);//表示するデータをセットする16進8bitデータ
void setint7segHEX2(unsigned int sel, unsigned int d0, unsigned int d1);//表示するデータをセットする16進4bitデータを2つ
void setstr7seg(unsigned int sel, unsigned char* str);//表示するデータをセットするアルファベット大文字3文字

void SelDigit(unsigned char digit);

void setup7seg(void);


#endif /* SEVENSEGMENT_H_ */
