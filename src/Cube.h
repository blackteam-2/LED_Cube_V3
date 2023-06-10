#ifndef	__CUBE__
#define	__CUBE__

#include	<Arduino.h>

#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BITA                (0x0400)
#define BITB                (0x0800)
#define BITC                (0x1000)
#define BITD                (0x2000)
#define BITE                (0x4000)
#define BITF                (0x8000u)

extern	word	const	wBit[0x10];

#define ON			true
#define OFF			false
//-----------------------------------------------------------------------------
#define Axis_X 		0
#define Axis_Y		1
#define Axis_Z		2
//-----------------------------------------------------------------------------
#define PlaneFlag	8
#define Plane_XY	8
#define Plane_ZY	9
#define Plane_ZX	10
//-----------------------------------------------------------------------------
#define CUBESIZE	8
#define TEXTPATHLEN	34
//-----------------------------------------------------------------------------
extern	byte	CubeArray[8][8];
extern	int		CurrentLayer;
//-----------------------------------------------------------------------------
void	SetPixel(byte X, byte Y, byte Z, bool Level);
bool	GetPixel(byte X, byte Y, byte Z);
void	SetAxisLine(byte Axis, byte a, byte b, byte data);
byte	GetAxisLine(byte Axis, byte a, byte b);
void	SetPlane(byte Plane, byte a, bool Level);
void	SetAllPixelsOff();
void	SetAllLEDSOn();
void	ShiftPlane(byte Plane, byte NoPos, bool Dir, bool ClrLay);
void	GetCharPattern(char Ch, byte rtnCh[5]);
void	AddTextToCubeLayer(byte plane, char Ch);
void	ResetTextPath(void);
void	AddChToPath(byte inputChr, int pos);
void	IncrementPath(void);
void	AddPathToCube(void);
void 	CheckArgOrder(int in1, int in2, int *out1, int *out2);

#endif //__CUBE__