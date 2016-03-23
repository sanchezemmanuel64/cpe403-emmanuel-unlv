#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"

#include "inc/hw_memmap.h"		//memory map definition for Tiva C device
#include "inc/hw_types.h"		//defines common types and macros
#include "driverlib/sysctl.h"	//defines macros for System Control API of DriverLib
#include "driverlib/gpio.h"		//defines macros fro GPIO API of DriverLib

uint8_t ui8PinData=2;	//unsigned 8-bit integer to store sequence for LEDs

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); 	//set clock: 40MHz, use PLL with 16MHz xtal, and divide by 5

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);									//enable port F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);		//set output pins on port F for the LEDs
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);	//SW1

	//Unlock Pin F0 to use an interrupt on SW2
		SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
		GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
		GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
		// only PF0 needs to be unlocked, other bits can't be locked
		GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
		GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
		GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
		GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
		GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
		GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
	while(1)
	{
		if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);	//turn on next LED sequence
			SysCtlDelay(2000000);
		}
		else//delay for 2000000 cycles
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);			//turn all LEDs off
			SysCtlDelay(2000000);
		}//delay for 2000000 cycles

		//if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}				//sequence will be 0010, 0100, 1000, 0010 ...

		//button = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
	}
}
