#ifndef	__CUBE_WORKER__
#define	__CUBE_WORKER__

#include	<Arduino.h>

typedef unsigned long dword;

extern	volatile	byte	Debounce[2];
extern	volatile	byte	PinCTemp;
extern	volatile	bool	TestPinPlag;

void	InitCube(void);
void	SetupCubeIO(void);
void	Setup_Timer1(uint16_t reloadVal);
void	LatchData(byte multiplex, byte data);
volatile	void	LatchLayer(byte layer, bool level);
void	SetData(byte data);
void	SetLayer(byte layer, bool level);
void	SetMuxAddy(byte addy);

#endif //__CUBE_WORKER__