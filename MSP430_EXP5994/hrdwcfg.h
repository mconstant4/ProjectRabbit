/*
 * hrdwcfg.h
 *
 *  Created on: Oct 30, 2017
 *      Author: Matthew Constant
 */

#ifndef HRDWCFG_H_
#define HRDWCFG_H_

#include <MSP430.h>

/************ LCD Display ************/
#define LCD_RS_PORT_OUT     P5OUT           // Register Select (P5.7)
#define LCD_RS_PORT_SEL0    P5SEL0
#define LCD_RS_PORT_SEL1    P5SEL1
#define LCD_RS_PORT_DIR     P5DIR
#define LCD_RS_PIN          BIT7

#define LCD_RW_PORT_OUT     P4OUT           // Read/Write (P4.4)
#define LCD_RW_PORT_SEL0    P4SEL0
#define LCD_RW_PORT_SEL1    P4SEL1
#define LCD_RW_PORT_DIR     P4DIR
#define LCD_RW_PIN          BIT4

#define LCD_E_PORT_OUT      P5OUT           // Enable (P5.3)
#define LCD_E_PORT_SEL0     P5SEL0
#define LCD_E_PORT_SEL1     P5SEL1
#define LCD_E_PORT_DIR      P5DIR
#define LCD_E_PIN           BIT3

#define LCD_D4_PORT_OUT     P8OUT           // Digital Bit 4 (P8.3)
#define LCD_D4_PORT_SEL0    P8SEL0
#define LCD_D4_PORT_SEL1    P8SEL1
#define LCD_D4_PORT_DIR     P8DIR
#define LCD_D4_PIN          BIT3

#define LCD_D5_PORT_OUT     P8OUT           // Digital Bit 5 (P8.2)
#define LCD_D5_PORT_SEL0    P8SEL0
#define LCD_D5_PORT_SEL1    P8SEL1
#define LCD_D5_PORT_DIR     P8DIR
#define LCD_D5_PIN          BIT2

#define LCD_D6_PORT_OUT     P8OUT           // Digital Bit 6 (P8.1)
#define LCD_D6_PORT_SEL0    P8SEL0
#define LCD_D6_PORT_SEL1    P8SEL1
#define LCD_D6_PORT_DIR     P8DIR
#define LCD_D6_PIN          BIT1

#define LCD_D7_PORT_OUT     P4OUT           // Digital Bit 7 (P4.1)
#define LCD_D7_PORT_SEL0    P4SEL0
#define LCD_D7_PORT_SEL1    P4SEL1
#define LCD_D7_PORT_DIR     P4DIR
#define LCD_D7_PIN          BIT1

/*********** Testing Circuit **********/
#define SQ_WAVE_PORT_OUT    P3OUT           // 1 KHz Square Wave
#define SQ_WAVE_PORT_SEL0   P3SEL0
#define SQ_WAVE_PORT_SEL1   P3SEL1
#define SQ_WAVE_PORT_DIR    P3DIR
#define SQ_WAVE_PIN         BIT7

#define BLANK_SIG_PORT_OUT  P3OUT           // Blanking Signal
#define BLANK_SIG_PORT_SEL0 P3SEL0
#define BLANK_SIG_PORT_SEL1 P3SEL1
#define BLANK_SIG_PORT_DIR  P3DIR
#define BLANK_SIG_PIN       BIT5

#define ADC_TEST_PORT_OUT    P1OUT          // Test Circuit Output (ADC input)
#define ADC_TEST_PORT_SEL0   P1SEL0
#define ADC_TEST_PORT_SEL1   P1SEL1
#define ADC_TEST_PORT_DIR    P1DIR
#define ADC_TEST_PIN         BIT2

// TODO: MUX Pins?

#endif /* HRDWCFG_H_ */
