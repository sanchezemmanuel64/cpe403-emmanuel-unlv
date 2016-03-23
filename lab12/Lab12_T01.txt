//video: http://screencast.com/t/3yrRLX019c

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

int main(void) {
	
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //setup system clock 50 MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //enable UART Port
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //enable port A

    GPIOPinConfigure(GPIO_PA0_U0RX); //configure PA0 as Rx
    GPIOPinConfigure(GPIO_PA1_U0TX); //configure PA1 as Tx
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); //configure A0 and A1 as UART

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, // Configure UART speed 115200, 8bits, 1 stop bit, no parity
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTCharPut(UART0_BASE, 'E'); //put these characters to serial port
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'T');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'x');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');

    while (1) //infinite loop
    {
    	if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));	//get char from keyboard and send through serial port
    }

}
