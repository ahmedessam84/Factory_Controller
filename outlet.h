
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
#define OUTLET_SIZE		8

#define CURRENT_PORT  PORTB

typedef struct{
	int hrOn;
	int minOn;
	int hrOff;
	int minOff;
}Outlet_Schedule_t;
	
typedef struct{
	bool uia8OutletStatus[8];
	Outlet_Schedule_t Schedule[OUTLET_SIZE];
}Outlet_t;	
	
void Outlet_Init(void);
uint8_t Outlet_Read(uint8_t ui8Pins);
void Outlet_On(uint8_t ui8Pins);	
void Outlet_Off(uint8_t ui8Pins);
void Outlet_Check_Schedule(void);
bool Outlet_Get_Status( uint8_t outlet );
void Outlet_Set_Schedule( uint8_t Outlet_Number, Outlet_Schedule_t * pSet_Time );
	
		
	
#endif
