//video: http://screencast.com/t/PuxUt1ug8

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"


int main(void)
{
	uint32_t ui32Period; //will store number to initialize counter

	//set the clock to run at 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Enable port F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2|GPIO_PIN_3); //Enable pins F1,F2,F3 as outputs

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //Enable timer 0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); //Configure timer 0 to periodic mode

	//get system clock, divide by desired blinking frequency, and divide by 2 because interrupt will happen twice per cycle
	ui32Period = (SysCtlClockGet() / 10) / 2;	//will blink at 10 Hz
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1); //load timer with value in ui32Period

	IntEnable(INT_TIMER0A); //Enable interrupts on timer 0
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //set timer 0 to interrupt at timeout
	IntMasterEnable(); //enable master interrupt

	TimerEnable(TIMER0_BASE, TIMER_A); //start the timer


	while(1) //infinite loop
	{
	}
}

void Timer0IntHandler(void) //interrupt handler for timer 0
{
	//Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Read the current state of the GPIO pin and
	//write back the opposite statee
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
	{
		//turn on pin
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	}
	else
	{
		//turn off pin
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
	}

}
