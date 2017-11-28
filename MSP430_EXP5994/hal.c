/*
 * hal.c
 *
 *  Created on: Oct 30, 2017
 *      Author: Matthew Constant
 */

#include <msp430.h>
#include <hal.h>
#include "lcd_driver.h"

// Local Function Declarations
void ConfigureBoardDefaults(void);
void ConfigureSystemClocks(void);
void ConfigureLcdDisplay(void);
void ConfigureAdcModule(void);

/**
 * The following function is provided to allow a mechanism of
 * configuring the MSP430 pins to their default state for the sample.
 */
void HAL_ConfigureHardware(void) {
    ConfigureBoardDefaults();           // Set all GPIOs to known state
    ConfigureSystemClocks();            // MCLK = 8MHz, SMCLK = 1MHz, ACLK = ??
    ConfigureLcdDisplay();              // Configure LCD Display
    ConfigureAdcModule();               // Configure ADC Peripheral (Single-ended, 1.2v Vref)

    /*
     * TODO:
     *  Initialize LED Driver Module
     *  Initialize other UI components (push buttons, extra LEDs, etc)
     */
}


/**
 * This function puts each pin into a known state. Every pin
 * is set to I/O functionality as outputs with a value of '0'.
 * The interrupt flag for each pin is also cleared to avoid
 * erroneous interrupts (per datasheet).
 */
void ConfigureBoardDefaults(void) {
    PADIR   =   0xFFFF;         // Set all pins as output
    PBDIR   =   0xFFFF;
    PCDIR   =   0xFFFF;
    PDDIR   =   0xFFFF;

    PAOUT   =   0x0000;         // Clear all pins' output
    PBOUT   =   0x0000;
    PCOUT   =   0x0000;
    PDOUT   =   0x0000;

    PASELC  =   0x0000;         // Select I/O Function for each pin
    PBSELC  =   0x0000;
    PCSELC  =   0x0000;
    PDSELC  =   0x0000;

    PAIFG   &=  0x0000;         // Clear all Interrupt Flags
    PBIFG   &=  0x0000;
    PCIFG   &=  0x0000;
    PDIFG   &=  0x0000;

    PM5CTL0 &= ~LOCKLPM5;       // Clear the LOCKLPM5 bit
}

void ConfigureSystemClocks(void) {
    /**
     * Board Defaults:
     *  MCLK:   DCO 1 MHz
     *  SMCLK:  DCO 1 MHz
     *  ACLK:   REFO 32.768 kHz
     *
     *
     *
     * Clock Frequencies:
     *      MCLK:   1 MHz      (8 MHz max for FRAM I/O, wait states needed otherwise)
     *      SMCLK:  1 MHz
     *      ACLK:   10 KHz(??)
     *
     * Include External Crystal Oscillator for ACLK?
     */

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO = 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;// Set ACLK=VLO SMCLK=DCO MCLK=DCO
    CSCTL3 = DIVA__8 | DIVS__8 | DIVM__8;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers
}

void ConfigureLcdDisplay(void) {
    lcd_init();                             // Use LCD Driver functions
}

/**
 * This function configures the pins necessary for ADC
 * conversion. Currently, the ADC module is setup to
 * trigger an interrupt when a conversion result is ready.
 * According to datasheet: 12-bit res, ADC12DF = 0, ADC12DIF = 0 => Result = [0x0000, 0x0FFF]
 *              +Vref = 0x0FFF (>+Vref = 0xFFFF)
 *              -Vref = 0x0000 (<-Vref = 0x0000)
 *
 * Vref+ = 1.2v
 * Vref- = 0v
 *
 * Signal Resolution (SR) = (Vref+-Vref-)/2^n   => SR = 292.969uV/bit
 * ADC Code = Input Voltage/SR                  => ADC Code = Vin/0.000292969 OR Vin = ADC Code * 0.000292969
 *
 * TODO: Set Vref to output pin?
 */
void ConfigureAdcModule(void) {
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_2 | REFON;           // Select internal ref = 2.5V

    // Configure ADC12 Module
    ADC12CTL0 = 0x0000;                     // Reset ADC12CTL0
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sample and Hold: 16 Clock Cycles, Turn on ADC

    ADC12CTL1 = 0x0000;                     // Reset ADC12CTL1
    ADC12CTL1 |= ADC12PDIV_0;               // Predivide Clock Source by 1
    ADC12CTL1 |= ADC12SHS_0;                // ADC12SC bit (??)
    ADC12CTL1 |= ADC12SHP;                  // Sourced by sampling timer (??)
    ADC12CTL1 &= ~ADC12ISSH;                // Sample input not inverted
    ADC12CTL1 |= ADC12DIV_0;                // Divide Clock Source by 1
    ADC12CTL1 |= ADC12CONSEQ_0;             // Single Channel, Single Conversion

    ADC12CTL2 = 0x0020;                     // Reset ADC12CTL2
    ADC12CTL2 |= ADC12RES_2;                // 12-bit Resolution
    ADC12CTL2 &= ~ADC12DF;                  // Unsigned Binary Conversion Result
    ADC12CTL2 &= ~ADC12PWRMD;               // Disable low power mode (at least until sample rate is finalized)

    ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt (do not use if polling result)
    ADC12MCTL0 &= ~ADC12DIF;                // Single Ended Mode
    ADC12MCTL0 |= ADC12INCH_2;              // A2 ADC Input Select
    ADC12MCTL0 |= ADC12VRSEL_1;             // Vref+ = VREF Buffered, Vref- = AVSS
}

void HAL_StartAdcModule(void) {
    ADC12CTL0 |= ADC12ENC | ADC12SC;        // Start ADC Module
}

void HAL_StopAdcModule(void) {
    ADC12CTL0 &= ~(ADC12ENC + ADC12SC);     // ADC12_B Disabled, Conversion Stopped
    ADC12CTL0 &= ~ADC12ON;                  // Turn off ADC Module (Must be disabled first)
}

/**
 * This function configures the waveform used to drive the
 * testing circuit. Produces a 1 KHz Square Wave with a
 * Duty Cycle of 50%.
 *
 * NOTE: Assumes SMCLK is running at 1MHz
 */
void HAL_StartSquareWave(void) {
    // Configure GPIOs
    P3DIR  |= BIT7;                         // P3.7 output
    P3SEL0 |= BIT7;                         // P3.7 options select (PWM)

    // Configure Timer4_A.1
    TA4CCR0 = 1000-1;                       // PWM Period
    TA4CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA4CCR1 = 500;                          // CCR1 PWM duty cycle
    TA4CTL = TASSEL__SMCLK | MC__UP | TACLR;// SMCLK, up mode, clear TAR
}

/**
 * TODO: Verify this works
 */
void HAL_StopSquareWave(void) {
    TA4CTL = MC__STOP;
}
