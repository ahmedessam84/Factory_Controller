
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "inc/hw_hibernate.h"
#include "driverlib/hibernate.h"
#include "utils/ustdlib.h"

time_t rtcSeconds;	 /* date/time in unix SECONDS past 1-Jan-70 */

/* initializes the hibernation RTC module */

void Rtc_Init(void)
{
		unsigned long ui32SysClock;
		SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
		HibernateEnableExpClk(ui32SysClock);
		HibernateClockConfig(HIBERNATE_OSC_LOWDRIVE);
		HibernateRTCEnable();
}

/* gets the number of seconds from the hibernation counter register and converts it to calendar format */
/* use this function to get the current time and date*/
void Rtc_GetDate( struct tm *tm_ptr )
{
	ulocaltime( HibernateRTCGet(), tm_ptr );
}

/* converts a date/time format to seconds to be loaded into the hibernation counter register */
/* use this function to set the current time and date*/
void Rtc_SetDate( struct tm *tm_ptr )
{	
	HibernateRTCSet( umktime( tm_ptr ) );
}


bool Rtc_Compare( struct tm *Alarm_tm_ptr )
{
	struct tm temp_cal;
	
	// Store current time in a temporary time struct
	Rtc_GetDate( &temp_cal );
	
	// Compare time only, resolution 1 min
	
	// if hour is equal & minutes are equal
	if( (temp_cal.tm_hour == Alarm_tm_ptr->tm_hour) && (temp_cal.tm_min == Alarm_tm_ptr->tm_min) )
	{
		return 1;
	}
	
	else return 0;
}


/* use this code to convert time to ascii for display on terminal or LCD
  format the output for uart
  strftime((char *)buf_rz, 32, "%c\r\n", &tm2);
  UARTSend((uint8_t *)buf_rz, 32);
*/
