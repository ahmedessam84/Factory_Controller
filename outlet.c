
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

#include "outlet.h"
#include "rtc.h"

static Outlet_t Outlet;

// this function initialized the 8 digiatl pins to control the relayed outlets
// port B is used as the output control port for the relays

void Outlet_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA + CURRENT_PORT);
	
	//
	// Check if the peripheral access is enabled.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA + CURRENT_PORT))
	{
	}
	
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE + (CURRENT_PORT<<12),ALL_PINS);
	
	GPIOPinWrite(GPIO_PORTA_BASE + (CURRENT_PORT<<12),ALL_PINS,0x00);
	
}

uint8_t Outlet_Read(uint8_t ui8Pins)
{
	return GPIOPinRead(GPIO_PORTA_BASE + (CURRENT_PORT<<12),ui8Pins);	
}

void Outlet_On(uint8_t ui8Pins)
{
	GPIOPinWrite(GPIO_PORTA_BASE + (CURRENT_PORT<<12), ui8Pins, 0xFF);
}

void Outlet_Off(uint8_t ui8Pins)
{
	GPIOPinWrite(GPIO_PORTA_BASE + (CURRENT_PORT<<12), ui8Pins, 0x00);
}


void Outlet_Check_Schedule()
{
	struct tm temp_cal;
	
	// Store current time in a temporary time struct
	Rtc_GetDate( &temp_cal );
	
	// check schedule for each outlet in the outlet box
	//
	int i = 0;
	for(i=0;i<OUTLET_SIZE;++i)
	{
		if( ( (temp_cal.tm_hour >= Outlet.Schedule[i].hrOn) && (temp_cal.tm_min >= Outlet.Schedule[i].minOn) ) && 
			( (temp_cal.tm_hour <= Outlet.Schedule[i].hrOff) && (temp_cal.tm_min < Outlet.Schedule[i].minOff) ) )
		{
			Outlet_On(1<<i);
			Outlet.uia8OutletStatus[i] = 1;
		}
		else
		{
			Outlet_Off(1<<i);
			Outlet.uia8OutletStatus[i] = 0;
		}		
	}
	
}

bool Outlet_Get_Status( uint8_t outlet )
{
	return Outlet.uia8OutletStatus[outlet];
}

void Outlet_Set_Schedule( uint8_t Outlet_Number, Outlet_Schedule_t * pSet_Time )
{
		Outlet.Schedule[Outlet_Number].hrOn = pSet_Time->hrOn;
		Outlet.Schedule[Outlet_Number].minOn = pSet_Time->minOn;		
		Outlet.Schedule[Outlet_Number].hrOff = pSet_Time->hrOff;
		Outlet.Schedule[Outlet_Number].minOff = pSet_Time->minOff;
}
