/*
 * lcd_driver.h
 *
 *  Created on: Nov 8, 2017
 *      Author: Matthew Constant
 */

#ifndef MSP430_EXP5994_LCD_DRIVER_H_
#define MSP430_EXP5994_LCD_DRIVER_H_

#ifndef TRUE
#define TRUE    1==1
#endif
#ifndef FALSE
#define FALSE   1==0
#endif

/*  Common LCD Controller Instructions  */
#define DISPLAY_CLEAR   0x01
#define DISPLAY_HOME    0x02

void lcd_init(void);
void lcd_write(char *data, int n);
void lcd_command(char instruction);

#endif /* MSP430_EXP5994_LCD_DRIVER_H_ */
