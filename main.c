/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "Timer.h"
#include "BlinkLed.h"

GPIO_InitTypeDef PI, SW1;
// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

void ms_delay_int_count(volatile unsigned int nTime) {
	nTime = (nTime * 14000);
	for (; nTime > 0; nTime--)
		;
}

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[]) {
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	SW1.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_12 | GPIO_PIN_13; //PB 0:up 1:down 2:center 12:left 13:right
	SW1.Mode = GPIO_MODE_INPUT;
	SW1.Pull = GPIO_NOPULL;
	SW1.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &SW1);

	PI.Pin = GPIO_PIN_15; //PortB 15:PIEZO
	PI.Mode = GPIO_MODE_OUTPUT_PP;
	PI.Pull = GPIO_NOPULL;
	PI.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &PI);
	unsigned int period, buf,flag=0;
	while(1){

		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)){
//			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
			ms_delay_int_count(300);
			flag=~flag;
		}
		if(flag){
			for (period = 0x1000; period >= 1; period--) {
					HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
					buf = period;
					while (buf--)
						;
//					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
//					buf = period;
//					while (buf--)
//						;
				}
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
