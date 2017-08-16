
#ifndef OUTLET_H
#define OUTLET_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#define PORTA					0
#define PORTB					1
#define PORTC					2
#define PORTD					3
#define ALL_PINS 			0xff
#define OUTLET0 			(1<<0)
#define OUTLET1 			(1<<1)
#define OUTLET2 			(1<<2)
#define OUTLET3 			(1<<3)
#define OUTLET4				(1<<4)
#define OUTLET5				(1<<5)
#define OUTLET6				(1<<6)
#define OUTLET7				(1<<7)
#define OUTLET_SIZE		8

#define CURRENT_PORT  PORTB
	
typedef struct{
	uint8_t uia8OutletStatus[8];
	int hrOn[OUTLET_SIZE];
	int minOn[OUTLET_SIZE];
	int hrOff[OUTLET_SIZE];
	int minOff[OUTLET_SIZE];
}Outlet_t;	
	
void Outlet_Init(void);
uint8_t Outlet_Read(uint8_t ui8Pins);
void Outlet_On(uint8_t ui8Pins);	
void Outlet_Off(uint8_t ui8Pins);
void Outlet_Check_Schedule(Outlet_t *Outlet_ptr);
	
		
	
#endif
