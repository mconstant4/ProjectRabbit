/*
 * hal.h
 *
 *  Created on: Oct 30, 2017
 *      Author: Matthew Constant
 */

#ifndef HAL_H_
#define HAL_H_

#define CPU_FREQ    1000000             // MCLK Frequency (Hz)

void HAL_ConfigureHardware(void);
void HAL_StartSquareWave(void);
void HAL_StopSquareWave(void);
void HAL_StartAdcModule(void);
void HAL_StopAdcModule(void);

#endif /* HAL_H_ */
