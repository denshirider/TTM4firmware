/*
 * E2ROM.h
 *
 *  Created on: 2018/01/27
 *      Author: USER
 */

#ifndef E2ROM_H_
#define E2ROM_H_

#include "iodefine.h"
#include "stdint.h"
#include "interface.h"

enum status {FLD_ERROR,FLD_OK,FLD_TMOUT,FLD_BLANK,FLD_NOBLANK};

int wait_FRDY(unsigned int wait);//処理待ち関数
static void reset_fcu(void);//fcuリセット関数
int32_t fld_init_pclk_notification(void);//クロック通知関数
int32_t fld_blank_check_2KB( uint32_t addr );//ブランクチェック関数
int32_t fld_erase_2KB( uint32_t addr );//消去関数
int32_t fld_program_2byte( uint32_t addr, uint16_t *ram );//2byte書き込み関数
int32_t fld_program_8byte( uint32_t addr, uint16_t *ram );//8byte書き込み関数
int32_t fld_enable_read(void);//読み出し許可関数
int32_t fld_enable_PE(void);//読み出し無効PE有効
int SaveProgram(int hangar,uint16_t *data);//プログラムをROMに保存//data is 256*2 byte
int LoadProgram(int hangar,uint16_t *data);//プログラムをROMから読み出し//data is 256*2 byte

#endif /* E2ROM_H_ */
