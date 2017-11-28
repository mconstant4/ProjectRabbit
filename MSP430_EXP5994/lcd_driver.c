/*
 * lcd_driver.c
 *
 * Driver developed for the NHD-C0220AA-FSW-FTW Graphic
 * Liquid Crystal Display Module.
 *
 * http://www.newhavendisplay.com/nhdc0220aafswftw-p-3886.html
 *
 *
 *  Created on: Nov 8, 2017
 *      Author: Matthew Constant
 */

#include <MSP430.h>
#include "hrdwcfg.h"                        // Pin Assignments
#include "lcd_driver.h"
#include "hal.h"                            // Delay Function

void send(char nybble);
void function_set(void);

/**
 * This function initializes the LCD Display. First, it configures
 * the MCU pins according to the setup specified in hrdwcfg.h.
 * Next, it sends a series of commands to the display according
 * to page 25 of the LCD Controller Datasheet.
 */
void lcd_init() {
    // Configure MCU Pins
    LCD_RS_PORT_SEL0 &= ~(LCD_RS_PIN);    // Select Pins as GPIOs
    LCD_RS_PORT_SEL1 &= ~(LCD_RS_PIN);
    LCD_RW_PORT_SEL0 &= ~(LCD_RW_PIN);
    LCD_RW_PORT_SEL1 &= ~(LCD_RW_PIN);
    LCD_E_PORT_SEL0  &= ~(LCD_E_PIN);
    LCD_E_PORT_SEL1  &= ~(LCD_E_PIN);
    LCD_D7_PORT_SEL0 &= ~(LCD_D7_PIN);
    LCD_D7_PORT_SEL1 &= ~(LCD_D7_PIN);
    LCD_D6_PORT_SEL0 &= ~(LCD_D6_PIN);
    LCD_D6_PORT_SEL1 &= ~(LCD_D6_PIN);
    LCD_D5_PORT_SEL0 &= ~(LCD_D5_PIN);
    LCD_D5_PORT_SEL1 &= ~(LCD_D5_PIN);
    LCD_D4_PORT_SEL0 &= ~(LCD_D4_PIN);
    LCD_D4_PORT_SEL1 &= ~(LCD_D4_PIN);

    LCD_RS_PORT_DIR |= LCD_RS_PIN;          // Set Pins as Outputs
    LCD_RW_PORT_DIR |= LCD_RW_PIN;
    LCD_E_PORT_DIR  |= LCD_E_PIN;
    LCD_D7_PORT_DIR |= LCD_D7_PIN;
    LCD_D6_PORT_DIR |= LCD_D6_PIN;
    LCD_D5_PORT_DIR |= LCD_D5_PIN;
    LCD_D4_PORT_DIR |= LCD_D4_PIN;

    // Initialize LCD Display (P. 25 of LCD Controller Datasheet)
    P7OUT &= ~BIT0;
    __delay_cycles(240000);                 // Delay 30 ms after Vdd on (@8MHz, 1 Clock Cycle = 125ns)
    P7OUT |= BIT0;
    function_set();                         // 2-Line / 5x10 Dots (Cannot use lcd_command because requires 3 segments)
    __delay_cycles(3200);                   // Delay >= 40us
    lcd_command(0x0C);                      // Display On / Cursor Off
    __delay_cycles(3200);                   // Delay >= 40us
    lcd_command(0x01);                      // Clear Display
    __delay_cycles(13120);                  // Delay >= 1.64ms
    lcd_command(0x03);                      // Increment / Shift On
}

/**
 * This function sends an instruction to the LCD Display. This is used
 * primarily by the lcd_init() function, however, it can also be used by
 * a Caller - for example to clear the display. Some common LCD instructions
 * are defined in lcd_driver.h.
 */
void lcd_command(char instruction) {
    LCD_RS_PORT_OUT &= ~(LCD_RS_PIN);       // Register Select = 0 (instruction)
    LCD_RW_PORT_OUT &= ~(LCD_RW_PIN);       // Read/Write = 0 (write)

    send(instruction>>4);                   // Send Upper Bits First
    send(instruction);                      // send() only sends the lower 4 bits
}

/**
 * This function writes data to the LCD Display. It takes as input a
 * char array and the number of characters in the array. Because of the
 * way lcd_init() is setup, every character write to the display will
 * increment the cursor and CG data address automatically.
 */
void lcd_write(char *data, int n) {
    LCD_RS_PORT_OUT |= LCD_RS_PIN;          // Register Select = 1 (data)
    LCD_RW_PORT_OUT &= ~(LCD_RW_PIN);       // Read/Write = 0 (write)

    unsigned int i;
    for(i = 0; i < n; i++) {                // Send characters serially
        send(data[i]>>4);                   // Send Upper Bits First
        send(data[i] & 0x0F);               // send() only sends the lower 4 bits
    }
}

/**
 * This function sets the digital bits (DB7-4) and then toggles the
 * Enable pin in order to send instructions and data to the display.
 *
 * NOTE: Data should be send MSB bit first, and four bits per function
 * call.
 */
void send(char nybble) {
    if(nybble & BIT0)                       // Set Digital Bits
        LCD_D4_PORT_OUT |= LCD_D4_PIN;
    else
        LCD_D4_PORT_OUT &= ~(LCD_D4_PIN);
    if(nybble & BIT1)
        LCD_D5_PORT_OUT |= LCD_D5_PIN;
    else
        LCD_D5_PORT_OUT &= ~(LCD_D5_PIN);
    if(nybble & BIT2)
        LCD_D6_PORT_OUT |= LCD_D6_PIN;
    else
        LCD_D6_PORT_OUT &= ~(LCD_D6_PIN);
    if(nybble & BIT3)
        LCD_D7_PORT_OUT |= LCD_D7_PIN;
    else
        LCD_D7_PORT_OUT &= ~(LCD_D7_PIN);

    LCD_E_PORT_OUT |= LCD_E_PIN;            // Set Enable
    __delay_cycles(3);                      // Pulse Width ~300ns
    LCD_E_PORT_OUT &= ~(LCD_E_PIN);         // Clear Enable (LCD reads data on falling edge of Enable)
}

/**
 * This is a helper function for lcd_init() because the function
 * set call requires over a byte of data to be sent.
 */
void function_set() {
    send(0x02);
    send(0x02);
    send(0x0F);                             // 2-line mode, 5x10 dots
}
