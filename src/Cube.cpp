#include	"Cube.h"
#include	"Font.h"

word	const	wBit[0x10] = {
	BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
	BIT8, BIT9, BITA, BITB, BITC, BITD, BITE, BITF};

/*
This is the main cube data, as wel as the basic functions to interact
with the array and set, clear, and read data. This also contains the
data/functions for handling text as well as the scrollint text
*/

// ====== Main Cube Data Array ======
// 8 bits in byte is Y, [X][Z]
// X - Right	->	Left
// Y - Front	->	Back
// Z - Bottom	->	Top
/*
---Top View---																								 		___________
{7}[7][Z]	{7}[6][Z]	{7}[5][Z]	{7}[4][Z]	{7}[3][Z]	{7}[2][Z]	{7}[1][Z]	{7}[0][Z]			|			|			|
{6}[7][Z]	{6}[6][Z]	{6}[5][Z]	{6}[4][Z]	{6}[3][Z]	{6}[2][Z]	{6}[1][Z]	{6}[0][Z]			|			|			|
{5}[7][Z]	{5}[6][Z]	{5}[5][Z]	{5}[4][Z]	{5}[3][Z]	{5}[2][Z]	{5}[1][Z]	{5}[0][Z]			|			|			|
{4}[7][Z]	{4}[6][Z]	{4}[5][Z]	{4}[4][Z]	{4}[3][Z]	{4}[2][Z]	{4}[1][Z]	{4}[0][Z]			|ZY Plane	|  XY Plane	|
{3}[7][Z]	{3}[6][Z]	{3}[5][Z]	{3}[4][Z]	{3}[3][Z]	{3}[2][Z]	{3}[1][Z]	{3}[0][Z]	^		|<---		|	(UP)	|
{2}[7][Z]	{2}[6][Z]	{2}[5][Z]	{2}[4][Z]	{2}[3][Z]	{2}[2][Z]	{2}[1][Z]	{2}[0][Z]	|		|			|			|
{1}[7][Z]	{1}[6][Z]	{1}[5][Z]	{1}[4][Z]	{1}[3][Z]	{1}[2][Z]	{1}[1][Z]	{1}[0][Z]	| Y		|			|			|
{0}[7][Z]	{0}[6][Z]	{0}[5][Z]	{0}[4][Z]	{0}[3][Z]	{0}[2][Z]	{0}[1][Z]	{0}[0][Z]	|		|			|___________|

																				X	<--------	_| (Z Up)
_____________________________________________________________________________________________
										^	ZX Plane 	^
										|				|

									  ^^^---Front---^^^

---Front View---																				 ___________
{0}[7][7]	{0}[6][7]	{0}[5][7]	{0}[4][7]	{0}[3][7]	{0}[2][7]	{0}[1][7]	{0}[0][7]	|			|	|
{0}[7][6]	{0}[6][6]	{0}[5][6]	{0}[4][6]	{0}[3][6]	{0}[2][6]	{0}[1][6]	{0}[0][6]	|			|	|
{0}[7][5]	{0}[6][5]	{0}[5][5]	{0}[4][5]	{0}[3][5]	{0}[2][5]	{0}[1][5]	{0}[0][5]	|			|	|
{0}[7][4]	{0}[6][4]	{0}[5][4]	{0}[4][4]	{0}[3][4]	{0}[2][4]	{0}[1][4]	{0}[0][4]	| ZX Plane	|	|<--- ZY Plane
{0}[7][3]	{0}[6][3]	{0}[5][3]	{0}[4][3]	{0}[3][3]	{0}[2][3]	{0}[1][3]	{0}[0][3]	|			|	|
{0}[7][2]	{0}[6][2]	{0}[5][2]	{0}[4][2]	{0}[3][2]	{0}[2][2]	{0}[1][2]	{0}[0][2]	|			|	|						 ^
{0}[7][1]	{0}[6][1]	{0}[5][1]	{0}[4][1]	{0}[3][1]	{0}[2][1]	{0}[1][1]	{0}[0][1]	|			|	|		XY Plane		 |	
{0}[7][0]	{0}[6][0]	{0}[5][0]	{0}[4][0]	{0}[3][0]	{0}[2][0]	{0}[1][0]	{0}[0][0]	|___________|	|	____________________ |
									 ^^^---Bottom---^^^
*/
byte	CubeArray[8][8];
int		CurrentLayer = 0;
// Data Array to hold the text info for itterating around the path
volatile unsigned char textPath[TEXTPATHLEN] = {0};

//-----------------------------------------------------------------------------
// Set the value of a LED at the specefied position in the array
void	SetPixel(byte X, byte Y, byte Z, bool Level) {
	if(Level)
		CubeArray[X][Z] |= wBit[Y];
	else
		CubeArray[X][Z] &= ~wBit[Y];
	}
//-----------------------------------------------------------------------------
// Get the value of a LED at the specefied position in the array
bool	GetPixel(byte X, byte Y, byte Z) {
	return (CubeArray[X][Z] & wBit[Y]);
	}
//-----------------------------------------------------------------------------
// Set all the LED's along a axis at the specefied position
// Axis = X, a=Y, b=Z
// Axis = Y, a=X, b=Z
// Axis = Z, a=X, b=Y
// eg. Set Z at a=0, b=0 will set all LEDs at {0}[0][Z] and produce a Vertical
// line at X,Y = 0,0 (front right Corner)
void	SetAxisLine(byte Axis, byte a, byte b, byte data) {
	int 	i;

	switch(Axis) {
		case Axis_X:
			for(i=0; i<CUBESIZE; i++) {
				SetPixel(i, a, b, data&0x01);
				data >>= 1;
				}
			break;
		case Axis_Y:
			CubeArray[a][b] = data;
			break;
		case Axis_Z:
			for(i=0; i<CUBESIZE; i++) {
				SetPixel(a, b, i, data&0x01);
				data >>= 1;
				}
			break;
		}
	}
//-----------------------------------------------------------------------------
// Get all the LED's along a axis at the specefied position
// Axis = X, a=Y, b=Z
// Axis = Y, a=X, b=Z
// Axis = Z, a=X, b=Y
// eg. Set Z at a=0, b=0 will set all LEDs at {0}[0][Z] and produce a Vertical
// line at X,Y = 0,0 (front right Corner)
byte	GetAxisLine(byte Axis, byte a, byte b) {
	int 	i;
	byte	rtn = 0;

	switch(Axis) {
		case Axis_X:
			for(i=0; i<CUBESIZE; i++) {
				rtn <<= 1;
				rtn += GetPixel(i, a, b);
				//SetArrayLED(i, a, b, Level);
				}
			break;
		case Axis_Y:
			rtn  = CubeArray[a][b];
			break;
		case Axis_Z:
			for(i=0; i<CUBESIZE; i++) {
				rtn <<= 1;
				rtn += (int)GetPixel(a, b, i);
				//SetArrayLED(a, b, i, Level);
				}
			break;
		default:
			rtn = -1;
			break;
		}
	return rtn;
	}
//-----------------------------------------------------------------------------
// Set all LED's in an Axis plane at a selected level on that axis
// Axis = ZX, a=Y
// Axis = ZY, a=X
// Axis = XY, a=Z
void	SetPlane(byte Plane, byte a, bool Level) {
	int 	i;

	switch(Plane) {
		case Plane_ZX:
			for(i=0; i<CUBESIZE; i++)
				SetAxisLine(Axis_Z, a, i, (Level)?0xFF:0x00);
			break;
		case Plane_ZY:
			for(i=0; i<CUBESIZE; i++)
				SetAxisLine(Axis_Z, i, a, (Level)?0xFF:0x00);
			break;
		case Plane_XY:
			for(i=0; i<CUBESIZE; i++)
				SetAxisLine(Axis_Y, i, a, (Level)?0xFF:0x00);
			break;
		}
	}
//-----------------------------------------------------------------------------
// Get Plane ???
//-----------------------------------------------------------------------------
void	SetAllLEDS(bool Level) {
	int 	i, j;

	for(i=0; i < CUBESIZE; i++)
		for(j=0; j<CUBESIZE; j++)
			CubeArray[i][j] = (Level)?0xFF:0x00;
	}
//-----------------------------------------------------------------------------
void	SetAllPixelsOff() {
	SetAllLEDS(OFF);
	}
//-----------------------------------------------------------------------------
void	SetAllLEDSOn() {
	SetAllLEDS(ON);
	}
//-----------------------------------------------------------------------------
// shift all the LEDS along the selected plane by the number of positions sel
// Plane 	- The plane to shift the LED's along
// NoPos 	- The number of positions to shift the data
// Dir		- True = 0 -> CUBESIZE, false = CUBESIZE -> 0
// ClrLay	- True = Clear the shifted layers that have shifted data 
void	ShiftPlane(byte Plane, byte NoPos, bool Dir, bool ClrLay) { 
	int 	i, j;

	if(NoPos==0) return;
	if(NoPos>CUBESIZE) NoPos = CUBESIZE;

	switch(Plane) {
		case Plane_ZX:
			if(Dir) {
				for(i=(CUBESIZE-1); i>=NoPos; i--)
					for(j=0; j<CUBESIZE; j++)
						SetAxisLine(Axis_Z, j, i, GetAxisLine(Axis_Z, j, i-NoPos));
				}
			else {
				for(i=0; i<=CUBESIZE-NoPos; i++)
					for(j=0; j<CUBESIZE; j++)
						SetAxisLine(Axis_Z, j, i, GetAxisLine(Axis_Z, j, i+NoPos));
				}
			break;
		case Plane_ZY:
			if(Dir) {
				for(i=(CUBESIZE-1); i>=NoPos; i--)
					for(j=0; j<CUBESIZE; j++)
						SetAxisLine(Axis_Z, i, j, GetAxisLine(Axis_Z, i-NoPos, j));
				}
			else {
				for(i=0; i<=CUBESIZE-NoPos; i++)
					for(j=0; j<CUBESIZE; j++)
						SetAxisLine(Axis_Z, i, j, GetAxisLine(Axis_Z, i+NoPos, j));
				}
			break;
		case Plane_XY:
			if(Dir) {
				for(i=(CUBESIZE-1); i>=NoPos; i--)
					for(j=0; j<CUBESIZE; j++)
						SetAxisLine(Axis_Y, j, i, GetAxisLine(Axis_Z, j, i-NoPos));
				}
			else {
				for(i=0; i<=CUBESIZE-NoPos; i++)
					for(j=0; j<CUBESIZE; j++)
						SetAxisLine(Axis_Y, j, i, GetAxisLine(Axis_Z, j, i+NoPos));
				}
			break;
		}

	if(ClrLay) {
		if(Dir) {
			for(i=0; i<NoPos; i++)
				SetPlane(Plane, i, false);
			}
		else {
			j = ((CUBESIZE-1)-NoPos);
			for(i=CUBESIZE-1; i>j; i++)
				SetPlane(Plane, i, false);
			}		
		}	
	}
//-----------------------------------------------------------------------------
void	GetCharPattern(char Ch, byte rtnCh[5]) {
	Ch -= 32;
	int addr = (int)Ch * 5;
	int i, j;

	for(i=addr; i<addr+5; i++) {
		rtnCh[j++] = chracterStorageArray[i];
		}
	}
//-----------------------------------------------------------------------------
void	AddTextToCubeLayer(byte plane, char Ch) {
	byte ChData[5] = {0};
	int i;

	GetCharPattern(Ch, ChData);
	for(i = 0; i<5; i++) {
		switch(plane) {
			case Plane_ZX:
				SetAxisLine(Axis_Z, i+1, 0, ChData[i]);
				break;
			case Plane_ZY:
				SetAxisLine(Axis_Z, 0, i+1, ChData[i]);
				break;
			case Plane_XY:
				SetAxisLine(Axis_Y, i+1, 0, ChData[i]);
				break;
			}
		}
	}
//-----------------------------------------------------------------------------
void	ResetTextPath(void) {
	int i; 

	for(i=0; i<TEXTPATHLEN; i++) {
		textPath[i] = 0;
		}
	}
//-----------------------------------------------------------------------------
void	AddChToPath(byte inputChr, int pos) {
	textPath[pos] = inputChr;
	}
//-----------------------------------------------------------------------------
void	IncrementPath(void) {
	int i;

	for(i=TEXTPATHLEN; i>=0; i--) {
		textPath[i+1] = textPath[i];
		}
	textPath[0] = 0;
	}
//-----------------------------------------------------------------------------
// Start Path in back right corner, scrol in clockwise looking top down
void	AddPathToCube(void) {
	//			Axis  , X, Y, Data
	// Y axis, back to front
	SetAxisLine(Axis_Z, 0, 7, textPath[0]);
	SetAxisLine(Axis_Z, 0, 6, textPath[1]);
	SetAxisLine(Axis_Z, 0, 5, textPath[2]);
	SetAxisLine(Axis_Z, 0, 4, textPath[3]);
	SetAxisLine(Axis_Z, 0, 3, textPath[4]);
	SetAxisLine(Axis_Z, 0, 2, textPath[5]);
	SetAxisLine(Axis_Z, 0, 1, textPath[6]);
	SetAxisLine(Axis_Z, 0, 0, textPath[7]);
	// X axis, front cube, right to left
	SetAxisLine(Axis_Z, 1, 0, textPath[8]);
	SetAxisLine(Axis_Z, 2, 0, textPath[9]);
	SetAxisLine(Axis_Z, 3, 0, textPath[10]);
	SetAxisLine(Axis_Z, 4, 0, textPath[11]);
	SetAxisLine(Axis_Z, 5, 0, textPath[12]);
	SetAxisLine(Axis_Z, 6, 0, textPath[13]);
	SetAxisLine(Axis_Z, 7, 0, textPath[14]);
	// Y axis, front to back
	SetAxisLine(Axis_Z, 7, 1, textPath[15]);
	SetAxisLine(Axis_Z, 7, 2, textPath[16]);
	SetAxisLine(Axis_Z, 7, 3, textPath[17]);
	SetAxisLine(Axis_Z, 7, 4, textPath[18]);
	SetAxisLine(Axis_Z, 7, 5, textPath[19]);
	SetAxisLine(Axis_Z, 7, 6, textPath[20]);
	SetAxisLine(Axis_Z, 7, 7, textPath[21]);
	// x axis, cube back, left to right
	SetAxisLine(Axis_Z, 6, 7, textPath[22]);
	SetAxisLine(Axis_Z, 6, 6, textPath[23]);
	SetAxisLine(Axis_Z, 6, 5, textPath[24]);
	SetAxisLine(Axis_Z, 6, 4, textPath[25]);
	SetAxisLine(Axis_Z, 6, 3, textPath[26]);
	SetAxisLine(Axis_Z, 6, 2, textPath[27]);
	SetAxisLine(Axis_Z, 6, 1, textPath[28]);
	}
//-----------------------------------------------------------------------------