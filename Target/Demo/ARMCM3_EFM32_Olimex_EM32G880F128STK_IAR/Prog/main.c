/****************************************************************************************
|  Description: demo program application source file
|    File Name: main.c
|
|----------------------------------------------------------------------------------------
|                          C O P Y R I G H T
|----------------------------------------------------------------------------------------
|   Copyright (c) 2012  by Feaser    http://www.feaser.com    All rights reserved
|
|----------------------------------------------------------------------------------------
|                            L I C E N S E
|----------------------------------------------------------------------------------------
| This file is part of OpenBLT. OpenBLT is free software: you can redistribute it and/or
| modify it under the terms of the GNU General Public License as published by the Free
| Software Foundation, either version 3 of the License, or (at your option) any later
| version.
|
| OpenBLT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
| without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
| PURPOSE. See the GNU General Public License for more details.
|
| You should have received a copy of the GNU General Public License along with OpenBLT.
| If not, see <http://www.gnu.org/licenses/>.
|
| A special exception to the GPL is included to allow you to distribute a combined work 
| that includes OpenBLT without being obliged to provide the source code for any 
| proprietary components. The exception text is included at the bottom of the license
| file <license.html>.
| 
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "header.h"                                    /* generic header               */


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void Init(void);


/****************************************************************************************
** NAME:           main
** PARAMETER:      none
** RETURN VALUE:   none
** DESCRIPTION:    This is the entry point for the bootloader application and is called 
**                 by the reset interrupt vector after the C-startup routines executed.
**
****************************************************************************************/
void main(void)
{
  /* initialize the microcontroller */
  Init();
  /* initialize the bootloader interface */
  BootComInit();

  /* start the infinite program loop */
  while (1)
  {
    /* toggle LED with a fixed frequency */
    LedToggle();
    /* check for bootloader activation request */
    BootComCheckActivationRequest();
  }
} /*** end of main ***/


/****************************************************************************************
** NAME:           Init
** PARAMETER:      none
** RETURN VALUE:   none
** DESCRIPTION:    Initializes the microcontroller. 
**
****************************************************************************************/
static void Init(void)
{
  /* initialize the system and its clocks */
  SystemInit();
  /* handle chip errate workarounds */
  CHIP_Init();
  /* enable the low frequency crystal oscillator */
  CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
  /* turn on clocking of all the modules */
  CMU->HFCORECLKEN0 |= 0x0F;
  CMU->HFPERCLKEN0 |= 0xFFFF;
  /* disable clocking of the modules that are not in use */
  CMU_ClockEnable(cmuClock_AES, false);
  CMU_ClockEnable(cmuClock_DMA, false);
  CMU_ClockEnable(cmuClock_EBI, false);
  CMU_ClockEnable(cmuClock_PRS, false);
  CMU_ClockEnable(cmuClock_USART0, false);
  CMU_ClockEnable(cmuClock_USART1, false);
  CMU_ClockEnable(cmuClock_USART2, false);
  CMU_ClockEnable(cmuClock_UART0, false);
  CMU_ClockEnable(cmuClock_ACMP0, false);
  CMU_ClockEnable(cmuClock_ACMP1, false);
  CMU_ClockEnable(cmuClock_DAC0, false);
  CMU_ClockEnable(cmuClock_ADC0, false);
  CMU_ClockEnable(cmuClock_I2C0, false);
  CMU_ClockEnable(cmuClock_VCMP, false);
#if (BOOT_COM_UART_ENABLE > 0)
  /* enable power to U2 (RS232_PWR_E) */
  GPIO_PinModeSet(gpioPortB, 9, gpioModePushPullDrive, 1);
  /* set port B outputs to drive up to 20 mA */
  GPIO_DriveModeSet(gpioPortB, gpioDriveModeHigh);
#endif
  /* init the led driver */
  LedInit();
  /* init the timer driver */
  TimerInit();
  /* enable IRQ's, because they were initially disabled by the bootloader */
  IrqInterruptEnable();
} /*** end of Init ***/


/*********************************** end of main.c *************************************/
