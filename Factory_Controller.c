
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "inc/hw_hibernate.h"
#include "driverlib/hibernate.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "rtc.h"
#include "outlet.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif


//*****************************************************************************
//
// This hook is called by FreeRTOS when a stack overflow error is detected.
//
//*****************************************************************************

void
vApplicationStackOverflowHook( xTaskHandle *pxTask, char *pcTaskName )
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// Configure the UART and its pins. This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
		
}

int
main(void)
{
		struct tm calendar;
		Outlet_Schedule_t Schedule;
		char buffer[20];

		//
		// Set the clocking to run at 80 MHz from the PLL.
		//
		SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
		
    //
    // Initialize the UART.
    //
    ConfigureUART();
		Outlet_Init();
		Rtc_Init();
		
		calendar.tm_hour = 15;
		calendar.tm_min = 32;
		calendar.tm_sec = 0;
		calendar.tm_mday = 27;
		calendar.tm_mon = 3-1;
		calendar.tm_year = 2017 - 1900;
		
		//
		// preset test schedule
		//
		Schedule.hrOn = 15;
		Schedule.minOn = 32;
		Schedule.hrOff = 15;
		Schedule.minOff = 34;
		
		// load the hibernation counter register with the values set in calendar
		Rtc_SetDate( &calendar );
		
		Outlet_Set_Schedule( 0, &Schedule );
    
		while(1)
    {				
			
			Outlet_Check_Schedule();
			
			//Get date from register
			Rtc_GetDate( &calendar );
			//Convert decimal time to ascii format
			strftime( buffer, 32, "%X %x\n", &calendar );
			//Display date and time
			UARTprintf("%s", buffer);

			
			//Display outlet status
			int i;
			for(i=0;i<OUTLET_SIZE;++i)
			{
				if( Outlet_Get_Status(i) )
				{
					UARTprintf("ON ");
				}
				else
				{
					UARTprintf("OFF ");
				}
			}
			
			UARTprintf("\n\n");
						
			//Delay 1 second
			SysCtlDelay( SysCtlClockGet() / 6 );
			
    }
}

