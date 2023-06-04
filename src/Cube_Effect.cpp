#include	"Cube_Effect.h"
#include	"Cube.h"

//-----------------------------------------------------------------------------
void	EffectLoop(int Effect) {
	static int EffAllPos = 0;
	int SelEffect = 0;

	if(Effect==EFF_ALL) {
		if(++EffAllPos > 2) EffAllPos = 0;
		SelEffect = EffAllPos;
		}
	else
		SelEffect = Effect;

	switch(SelEffect) {
		case EFF_ALL:	// Fall through as this is not a valid state to be in
		case EFF_RAIN:
			Effect_Rain(120, 100);
			break;
		case EFF_TOPDOWN:
			break;
		}
	}
//-----------------------------------------------------------------------------
void	Effect_Rain(int iterations, int itterationDelay) {
	int iterCnt, i, j;
	byte xx, yy;

	SetAllPixelsOff();

	for(iterCnt=0; iterCnt<iterations; iterCnt++) {
		ShiftPlane(Plane_XY, 1, false, true);
		j = random(10, 20);
		for(i=0; i<j; i++) {
			xx = random(0, 8);
			yy = random(0, 8);
			SetPixel(xx, yy, 7, true);
			}
		delay(itterationDelay);
		}
	}
//-----------------------------------------------------------------------------
void	Effect_TopDown(byte axis, int seperation, bool direction, int iterations, int itterationDelay) {
	int i, j;

	SetAllPixelsOff();

	switch(axis) {
		case Axis_X:
			SetPlane(Plane_ZY, 7, true);
			break;
		case Axis_Y:
			SetPlane(Plane_ZX, 7, true);
			break;
		case Axis_Z:
			SetPlane(Plane_XY, 7, true);
			break;
		}
	delay(itterationDelay);

	for(i=0; i<iterations; i++) {
		for(j=0; j<CUBESIZE; j++) {
			switch(axis) {
				case Axis_X:
					ShiftPlane(Plane_ZY, 1, direction, false);
					SetPlane(Plane_ZY, 7, false);
					break;
				case Axis_Y:
					SetPlane(Plane_ZX, 7, false);
					break;
				case Axis_Z:
					SetPlane(Plane_XY, 7, false);
					break;
				}
			if(i == (seperation - 1)) {
				switch(axis) {
					case Axis_X:
						SetPlane(Plane_ZY, 7, true);
						break;
					case Axis_Y:
						SetPlane(Plane_ZX, 7, true);
						break;
					case Axis_Z:
						SetPlane(Plane_XY, 7, true);
						break;
					}
				}
			}
		}
	
	/*
	if (seperation < 8)
	{
		for (i = 8 - seperation ; i < cubeSize ; i++)
		{
			//y-axis
			if (axis == 0)
			{
				shift(axis,1);
			}
			
			//x-axis
			if (axis == 1)
			{
				shift(axis,1);
			}
			
			//z-axis
			if (axis == 2)
			{
				shift(axis,1);
			}
		}
	}
	*/
	}
//-----------------------------------------------------------------------------
void	EffectShootText(byte plane, String inputStr, int iterations, int delayTime, int shiftDelayTime) {
	unsigned int	i, j, k, len;
	String			Str = inputStr;
	char			Ch;

	SetAllPixelsOff();
	len = Str.length();
	for(i=0; i<iterations; i++) {
		for(j=0; j<len; j++) {
			Ch = Str[j];
			SetPlane(plane, 7, false);
			AddTextToCubeLayer(plane, Ch);
			delay(shiftDelayTime);
			for(k=0; k<(CUBESIZE-1); k++) {
				ShiftPlane(plane, 1, true, false);
				SetPlane(plane, k-1, false);
				delay(shiftDelayTime);
				}
			delay(delayTime);
			}
		}
	}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------