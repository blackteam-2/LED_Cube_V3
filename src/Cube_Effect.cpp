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
		case EFF_Rain:
			Effect_Rain(120, 100);
			break;
		case EFF_TopDown:
			Effect_TopDown(Axis_Z, 3, true, 5, 500);
			break;
		case EFF_ShootText:
			EffectShootText(Plane_XY, "9876543210", 1, 600, 50);
			break;
		case EFF_Firework:
			break;
		case EFF_ScrolText:
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
void	EffectFireworks(int iterations, int n, int delayTime) {
	int ilk, f, e;
	float origin_x = 3;
	float origin_y = 3;
	float origin_z = 3;
	int rand_y, rand_x, rand_z;
	float slowrate, gravity;

	SetAllPixelsOff();
	// Particles and their position, x,y,z and their movement, dx, dy, dz
	float particles[n][6];
	for (ilk = 0; ilk < iterations; ilk++) {
		origin_x = rand() % 4;
		origin_y = rand() % 4;
		origin_z = rand() % 2;
		origin_z += 5;
		origin_x += 2;
		origin_y += 2;
		// shoot a particle up in the air
		for (e = 0; e < origin_z; e++) {
			SetPixel(e, origin_x, origin_y, true);
			delay(160 * e);
			SetAllPixelsOff();
			}
		// Fill particle array
		for (f = 0; f < n; f++) {
			// Position
			particles[f][0] = origin_x;
			particles[f][1] = origin_y;
			particles[f][2] = origin_z;
			rand_x = rand()%200;
			rand_y = rand()%200;
			rand_z = rand()%200;
			// Movement
			particles[f][3] = 1 - (float)rand_x / 100; // dx
			particles[f][4] = 1 - (float)rand_y / 100; // dy
			particles[f][5] = 1 - (float)rand_z / 100; // dz
			}
		// explode
		for (e = 0; e < 25; e++) {
			slowrate = 1 + tan((e + 0.1) / 20) * 10;
			gravity = tan((e + 0.1) / 20) / 2;
			for (f = 0; f < n; f++) {
				particles[f][0] += particles[f][3] / slowrate;
				particles[f][1] += particles[f][4] / slowrate;
				particles[f][2] += particles[f][5] / slowrate;
				particles[f][2] -= gravity;
				SetPixel(particles[f][2],particles[f][0],particles[f][1],true);
				}
			delay(delayTime);
			SetAllPixelsOff();
			}
		}
	}
//-----------------------------------------------------------------------------
void	EffectScrollText(int iterations, String inputstr, int delayTime) {
	int i, j, k;
	String inputString = inputstr;
	int stringLength = inputstr.length();

	SetAllPixelsOff();
	ResetTextPath();
	for (i = 0; i < iterations ; i++) {
		//
		for (j = 0 ; j < stringLength ; j++) {
			//
			char currentChr = inputString[j];
			unsigned char chrPattern[5] = {0};
			GetCharPattern(currentChr, chrPattern);
			
			//
			for (k = 0 ; k < 5 ; k++) {
				//
				AddChToPath(chrPattern[k], 0);
				AddPathToCube();
				delay(delayTime);
				IncrementPath();
				AddPathToCube();
				}
			//Add space between characters
			delay(delayTime);
			IncrementPath();
			AddPathToCube();
			}
		for (int k = 0 ; k < TEXTPATHLEN - 4 ; k++) {
			delay(delayTime);
			IncrementPath();
			AddPathToCube();
			}
		}
	}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------