/*
 * E2ROM.c
 *
 *  Created on: 2018/01/27
 *      Author: USER
 */

#include "E2ROM.h"

int32_t fld_init_pclk_notification(void)
{
	// 00100000h is top address of DB00 block
	volatile uint8_t *addr_b = (uint8_t *)0x00100000;
	volatile uint16_t *addr_w = (uint16_t *)0x00100000;
	// change to P/E mode
	if( (FLASH.FENTRYR.WORD & 0x00ff) != 0x0080 ){
		FLASH.FENTRYR.WORD = 0xAA80; // AAh is key
	}
	// set peripheral clock
	FLASH.PCKAR.BIT.PCKA = 32; // PCLK = 32 MHz
	// execute the peripheral clock notification command
	*addr_b = 0xE9;
	*addr_b = 0x03;
	*addr_w = 0x0F0F;
	*addr_w = 0x0F0F;
	*addr_w = 0x0F0F;
	*addr_b = 0xD0;
	// wait for tPCKA time
	if( wait_FRDY( 120 ) == FLD_TMOUT ){ // timeout is 120us
		reset_fcu();
	}
	// error check
	if( FLASH.FSTATR0.BIT.ILGLERR == 1 ){
		return FLD_ERROR;
	}
	return FLD_OK;
}


int32_t fld_blank_check_2KB( uint32_t addr )
{
	volatile uint8_t *addr_b = (uint8_t *)addr;
	// use the blank checking command
	FLASH.FMODR.BIT.FRDMD = 1;
	// set the blank check size (2 KB)
	FLASH.DFLBCCNT.BIT.BCSIZE = 1;
	// execute the Blank checking command
	*addr_b = 0x71;
	*addr_b = 0xD0;
	// wait for tDBC2K time (timeout is 770us)
	if( wait_FRDY( 700*1.1 ) == FLD_TMOUT ){
		reset_fcu();
	}
	// error check
	if( FLASH.FSTATR0.BIT.ILGLERR == 1 ){
		return FLD_ERROR;
	}
	// get result of blank checking command
	if( FLASH.DFLBCSTAT.BIT.BCST == 0 ){
		return FLD_BLANK;
	}
	return FLD_NOBLANK;
}

int32_t fld_erase_2KB( uint32_t addr )
{
	volatile uint8_t *addr_b;
	int32_t ret = FLD_OK;
	int i=0;
	// Unprotect
	FLASH.FWEPROR.BIT.FLWE = 1;
	// enable to erase all blocks
	FLASH.DFLWE0.WORD = 0x1E0F; // 1Eh is Key
	for(i=0;i<16;i++){
		addr_b = (uint8_t *)((uint32_t)addr + (uint32_t)128*i);//消去ブロックを移動
		// execute the block erase command
		*addr_b = 0x20;
		*addr_b = 0xD0;
		// wait for tDE2K time (timeout is 275ms)
		if( wait_FRDY( 250*1000*1.1 ) == FLD_TMOUT ){
			reset_fcu();
		}
		// Error check
		if( (FLASH.FSTATR0.BIT.ILGLERR == 1) ||	(FLASH.FSTATR0.BIT.ERSERR == 1) ){
			ret = FLD_ERROR;
		}
	}
	//protect and disable to erase
	FLASH.FWEPROR.BIT.FLWE = 2;
	FLASH.DFLWE0.WORD = 0x1E00;
	return ret;
}

int32_t fld_program_2byte( uint32_t addr, uint16_t *ram )
{
	volatile uint8_t *addr_b = (uint8_t *)addr;
	volatile uint16_t *addr_w = (uint16_t *)addr;
	int32_t ret = FLD_OK;
	// Unprotect and enable to write all blocks
	FLASH.FWEPROR.BIT.FLWE = 1;
	FLASH.DFLWE0.WORD = 0x1EFF; // 1Eh is Key
	//FLASH.DFLWE1.WORD = 0xE1FF; // E1h is Key
	// execute the 2-byte programming command
	*addr_b = 0xE8;
	*addr_b = 0x01;
	*addr_w = *ram;
	*addr_b = 0xD0;
	// wait for tDP8 time (timeout is 2.2ms)
	if( wait_FRDY( 2*1000*1.1 ) == FLD_TMOUT ){
		reset_fcu();
	}
	// error check
	if( (FLASH.FSTATR0.BIT.ILGLERR == 1) ||	(FLASH.FSTATR0.BIT.PRGERR == 1) ){
		ret = FLD_ERROR;
	}
	//protect and disable to write
	FLASH.FWEPROR.BIT.FLWE = 2;
	FLASH.DFLWE0.WORD = 0x1E00;
	//FLASH.DFLWE1.WORD = 0xE100;
	return ret;
}

int32_t fld_program_8byte( uint32_t addr, uint16_t *ram )
{
	volatile uint8_t *addr_b = (uint8_t *)addr;
	volatile uint16_t *addr_w = (uint16_t *)addr;
	int32_t i,ret = FLD_OK;
	// Unprotect and enable to write all blocks
	FLASH.FWEPROR.BIT.FLWE = 1;
	FLASH.DFLWE0.WORD = 0x1EFF; // 1Eh is Key
	//FLASH.DFLWE1.WORD = 0xE1FF; // E1h is Key
	// execute the 8-byte programming command
	*addr_b = 0xE8;
	*addr_b = 0x04;
	for(i=0; i<4; i++){ // 8-byte is 4 word size
		*addr_w = *ram++;
	}
	*addr_b = 0xD0;
	// wait for tDP8 time (timeout is 2.2ms)
	if( wait_FRDY( 2*1000*1.1 ) == FLD_TMOUT ){
		reset_fcu();
	}
	// error check
	if( (FLASH.FSTATR0.BIT.ILGLERR == 1) ||	(FLASH.FSTATR0.BIT.PRGERR == 1) ){
	//	SCI5_printf("FST0=%x\n",FLASH.FSTATR0.BYTE);
	//	SCI5_printf("F=%x\n",FLASH.FASTAT.BYTE);
		ret = FLD_ERROR;
	}
	//protect and disable to write
	FLASH.FWEPROR.BIT.FLWE = 2;
	FLASH.DFLWE0.WORD = 0x1E00;
	//FLASH.DFLWE1.WORD = 0xE100;
	return ret;
}

int32_t fld_enable_read(void)
{
	// change to read mode
	if( FLASH.FENTRYR.WORD & 0x00ff ){
		FLASH.FENTRYR.WORD = 0xAA00; // AAh is key
	}
	// enable to read all blocks
	FLASH.DFLRE0.WORD = 0x2DFF; // 2Dh is Key
	//FLASH.DFLRE1.WORD = 0xD2FF; // D2h is Key
	return FLD_OK;
}

int32_t fld_enable_PE(void)
{
	//change to PE mode
	if( (FLASH.FENTRYR.WORD & 0x00ff) != 0x0080 ){
		FLASH.FENTRYR.WORD = 0xAA80; // AAh is key
	}
	//disabled to read all blocks
	FLASH.DFLRE0.WORD = 0x2D00; // 2Dh is Key
	return FLD_OK;

}

int wait_FRDY(unsigned int wait){
	unsigned int cnt=0;
	while(FLASH.FSTATR0.BIT.FRDY==0){
		delay_us(1);
		cnt++;
		if(cnt==wait) return FLD_TMOUT;
	}
	return FLD_OK;
}

static void reset_fcu(void)
{
	volatile int32_t w;
	// FCU reset
	FLASH.FRESETR.BIT.FRESET = 1;
	// wait for tRESW2 time (tRESW2 is 35us)
	delay_us(35);
	// clear FCU reset
	FLASH.FRESETR.BIT.FRESET = 0;
}

int SaveProgram(int hangar,uint16_t *data){
	int i=0,RE=FLD_OK;
	uint32_t top_addr_hangar = (0x00100000)+(hangar*0x800);
	fld_enable_PE();//E2ROM_PEモードへ移行
	if( fld_blank_check_2KB( top_addr_hangar ) !=FLD_BLANK ){//ブランクチェック
		RE = fld_erase_2KB( top_addr_hangar );
	//	if(RE != FLD_OK)	SCI5_printf("%dE\n",RE);
	}
	for(i=0;i<64;i++){
		RE=fld_program_8byte(top_addr_hangar+(i*8),data+(i*4));
		if(RE != FLD_OK) {
	//		SCI5_printf("%d,%d\n",RE,i);
			//break;
		}
	}

	return RE;
}
int LoadProgram(int hangar,uint16_t *data){
	int i=0;
	uint32_t top_addr_hangar = (0x00100000)+(hangar*0x800);
	fld_enable_PE();//E2ROM_PEモードへ移行
	if( fld_blank_check_2KB( top_addr_hangar )!=FLD_NOBLANK ){//ブランクチェック
	return FLD_ERROR;
	}
	fld_enable_read();//E2ROM_readモードへ移行
	for(i=0;i<256;i++){
		data[i]=*((uint16_t *)(top_addr_hangar+(i*2)));
	}
	return FLD_OK;
}

