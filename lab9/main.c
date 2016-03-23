//video: http://screencast.com/t/Zl5JhQxIjY

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#ifndef M_PI	//define pi if not already defined
#define M_PI                    3.14159265358979323846
#endif

#define SERIES_LENGTH 100	//length of array to hold sine values is 100

float gSeriesData[SERIES_LENGTH];	//array of floats to hold sine values

int32_t i32DataCount = 0;		//variable to store counter value for loop

int main(void)
{
    float fRadians;		//variable to store radian values

    FPULazyStackingEnable(); 	//enable lazy stacking
    FPUEnable();				//enable floating point unit (FPU)

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //set clock to 50 MHz

    fRadians = ((2 * M_PI) / SERIES_LENGTH);	//2PI*length = radians

    while(i32DataCount < SERIES_LENGTH)		//loop according to size of floating point array
    {
    	gSeriesData[i32DataCount] = sinf(fRadians * i32DataCount);	//populate array with sine values

    	i32DataCount++;			//increment count
    }

    while(1)			//infinite loop
    {
    }
}
