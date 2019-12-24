/*
 * regdefine.h
 *
 *  Created on: 2018/01/20
 *      Author: USER
 */

#ifndef REGDEFINE_H_
#define REGDEFINE_H_

#include "iodefine.h"
#define SW2    !PORT0.PIDR.BIT.B5
#define SW1    !PORT4.PIDR.BIT.B0
#define ADo0   PORT4.PODR.BIT.B1
#define ADo1   PORT4.PODR.BIT.B2
#define ADo2   PORT4.PODR.BIT.B3
#define ADo3   PORT4.PODR.BIT.B4
#define ADo4   PORT4.PODR.BIT.B6
#define ADo5   PORTE.PODR.BIT.B0
#define ADo6   PORTE.PODR.BIT.B1
#define ADo7   PORTE.PODR.BIT.B2
#define ADi0   PORT4.PIDR.BIT.B1
#define ADi1   PORT4.PIDR.BIT.B2
#define ADi2   PORT4.PIDR.BIT.B3
#define ADi3   PORT4.PIDR.BIT.B4
#define ADi4   PORT4.PIDR.BIT.B6
#define ADi5   PORTE.PIDR.BIT.B0
#define ADi6   PORTE.PIDR.BIT.B1
#define ADi7   PORTE.PIDR.BIT.B2
#define IRo4   PORTE.PODR.BIT.B3
#define IRo5   PORTE.PODR.BIT.B4
#define IRo6   PORTE.PODR.BIT.B5
#define IRo7   PORTA.PODR.BIT.B0
#define IRi4   PORTE.PIDR.BIT.B3
#define IRi5   PORTE.PIDR.BIT.B4
#define IRi6   PORTE.PIDR.BIT.B5
#define IRi7   PORTA.PIDR.BIT.B0
#define WREN   PORTA.PODR.BIT.B1
#define LED3   PORTA.PODR.BIT.B3
#define LED2   PORTA.PODR.BIT.B4
#define LED1   PORTA.PODR.BIT.B6
#define IRo0   PORTB.PODR.BIT.B3
#define IRo1   PORTB.PODR.BIT.B5
#define IRo2   PORTB.PODR.BIT.B6
#define IRo3   PORTB.PODR.BIT.B7
#define IRi0   PORTB.PIDR.BIT.B3
#define IRi1   PORTB.PIDR.BIT.B5
#define IRi2   PORTB.PIDR.BIT.B6
#define IRi3   PORTB.PIDR.BIT.B7
#define CLKEN  PORTC.PODR.BIT.B4
#define RST    PORTC.PODR.BIT.B5
#define OR0    PORT5.PIDR.BIT.B4
#define OR1    PORT5.PIDR.BIT.B5
#define OR2    PORTH.PIDR.BIT.B0
#define OR3    PORTH.PIDR.BIT.B1
#define OR4    PORTH.PIDR.BIT.B2
#define OR5    PORTH.PIDR.BIT.B3
#define OR6    PORT1.PIDR.BIT.B4
#define OR7    PORT1.PIDR.BIT.B5
#define PCOPEN PORT2.PODR.BIT.B7
#define RAMOE  PORT3.PODR.BIT.B1
#define RAMWEo PORT3.PODR.BIT.B2
#define RAMWEi PORT3.PIDR.BIT.B2
#define USRBTN !PORT3.PIDR.BIT.B5
#define DSA    PORT0.PODR.BIT.B3
#define DSB    PORT3.PODR.BIT.B7
#define DSC    PORT3.PODR.BIT.B6

#define PCaddress ((ADi0)|(ADi1<<1)|(ADi2<<2)|(ADi3<<3)|(ADi4<<4)|(ADi5<<5)|(ADi6<<6)|(ADi7<<7))
#define ORD ((OR0)|(OR1<<1)|(OR2<<2)|(OR3<<3))
#define ORU ((OR4)|(OR5<<1)|(OR6<<2)|(OR7<<3))
#define OUTreg ((ORD)|(ORU<<4))
#define IRD ((IRi0)|(IRi1<<1)|(IRi2<<2)|(IRi3<<3))
#define IRU ((IRi4)|(IRi5<<1)|(IRi6<<2)|(IRi7<<3))
#define INreg ((IRD)|(IRU<<4))

#define EN_CLK(v) CLKEN=v
#define EN_DSW(v) WREN=!(v)
#define EN_PC(v)  PCOPEN=!(v)
#define EN_RAM(v) RAMOE=!(v)
#define EN_RESET(v) RST=!(v)

#define PCLKCNT (unsigned int)*((unsigned short*)(&(TMR2.TCNT)))
//#define CLKCNT (unsigned int)*((unsigned short*)(&(TMR0.TCNT)))
#define CLKCNT ((TMR0.TCNT) | (TMR1.TCNT<<8))


#endif /* REGDEFINE_H_ */
