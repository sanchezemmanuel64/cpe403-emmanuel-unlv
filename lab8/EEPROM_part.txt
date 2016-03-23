#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/eeprom.h"

int main(void)
{
	uint32_t pui32Data[2];
	uint32_t pui32Read[2];
	pui32Data[0] = 0x12345678;
	pui32Data[1] = 0x56789abc;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //set clock to 40 MHz

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	//enable port F
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //set PF1, PF2, P3 as outputs
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);		//clear PF1, PF2, PF3
	SysCtlDelay(20000000);	//delay about 2 seconds

	FlashErase(0x10000);									//erase block of flash starting at 0x10000
	FlashProgram(pui32Data, 0x10000, sizeof(pui32Data));	//program the array pui32Data to beginning of block
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02);	//light red LED to indicate success
	SysCtlDelay(20000000);	//delay about 2 seconds

	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);		//turn on EEPROM peripheral
	EEPROMInit();										//perform recovery if power failed during previous operation
	EEPROMMassErase();									//erase entire PROM
	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));		//read erased values into pulRead (offset address)
	EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));	//program the data array to the beginning of the EEPROM
	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));		//read data into array puRead
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x04);	//turn off red LED and turn on blue LED

	while(1)
	{
	}
}
