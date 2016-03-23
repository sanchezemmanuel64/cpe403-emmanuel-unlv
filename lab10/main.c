/*******************************************************

Project : Orbit Lab 8 ATE (Temp With UART)
Version : 1.0
Date    : 2/20/2013
Author  : Brian Zufelt / Craig Kief
Company : COSMIAC/UNM
Comments:
This lab will extend the concepts from LAB 7. This Lab
will pull data from the temperature sensor found on the
Orbit board and output the data through the UART to be
read from a terminal program.

******************************************************
Chip type               : ARM TM4C123GH6PM
Program type            : Firmware
Core Clock frequency	: 80.000000 MHz
*******************************************************/

#define TEMP_ADDR  0x4F		// Address for Temp Sensor

// Define needed for pin_map.h
#ifndef	PART_TM4C123GH6PM
#define PART_TM4C123GH6PM
#endif

#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"

unsigned char start_screen[29] = "\n\n\r ATE Lab 8 Temp Sensor \n\n\r";
unsigned char log[18] = "\n\n\r Temp reading: ";

void Print_header();					// Prints Header
void Read_temp(unsigned char *data);	// Read Temperature sensor

void main(void) {

	unsigned char temp_data[10] = "00.0 C \n\n\r";		// Temp format to be edited by read
	unsigned short int i = 0; 							// general counter

	// Setup the I2C see lab 7 **************************************************************************************************************************
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);  //setup clock

	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);		// Enable I2C hardware
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// Enable Pin hardware

	GPIOPinConfigure(GPIO_PB3_I2C0SDA);				// Configure GPIO pin for I2C Data line
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);				// Configure GPIO Pin for I2C clock line

	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);  // Set Pin Type

	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);		// SDA MUST BE STD
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);		// SCL MUST BE OPEN DRAIN
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false); 							// The False sets the controller to 100kHz communication
	I2CMasterSlaveAddrSet(I2C0_BASE, TEMP_ADDR, true);  				// false means transmit
	//****************************************************************************************************************************************************
	//configure Port F for LEDs
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);									//enable port F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);		//set output pins on port F for the LEDs

	// Setup the UART see lab 6 **************************************************************************************************************************

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);		// Enable UART hardware
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		// Enable Pin hardware

	GPIOPinConfigure(GPIO_PA0_U0RX);		// Configure GPIO pin for UART RX line
	GPIOPinConfigure(GPIO_PA1_U0TX);		// Configure GPIO Pin for UART TX line
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);     // Set Pins for UART

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,		// Configure UART to 8N1 at 115200bps
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	//****************************************************************************************************************************************************

	Print_header();			// Print Header

	while(1){

		Read_temp(temp_data);						// Read Data from Temp Sensor

		if(temp_data[0] < '3')
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);			//turn on blue LED
		else
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);			//turn on red LED


		SysCtlDelay(6000000);						// Delay

		for(i=0;i<10;i++){							// Loop to print out data string
			UARTCharPut(UART0_BASE, temp_data[i]);
		}

	}

}

void Print_header(){			// Print Header at start of program

	int i = 0; // general counter

	for(i=0;i<29;i++){	// Print Header at start of program
		UARTCharPut(UART0_BASE, start_screen[i]);
	}
}


void Read_temp(unsigned char *data){	// Read Temperature sensor

	unsigned char temp[2];				//  storage for data

	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);		// Start condition
	SysCtlDelay(20000);															// Delay
	temp[0] = I2CMasterDataGet(I2C0_BASE);								// Read first char
	SysCtlDelay(20000);															// Delay
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);		// Push second Char
	SysCtlDelay(20000);															// Delay
	temp[1] = I2CMasterDataGet(I2C0_BASE);								// Read second char
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);	// Stop Condition

	data[0] = (temp[0] / 10) + 0x30;											// convert 10 place to ASCII
	data[1] = (temp[0] - ((temp[0] / 10)*10)) + 0x30;							// Convert 1's place to ASCII
	if(temp[1] == 0x80){														// Test for .5 accuracy
		data[3] = 0x35;
	}
	else{
		data[3] = 0x30;
	}

}


