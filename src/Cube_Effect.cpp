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
			//Effect_TopDown(Axis_Z, 3, true, 5, 500);
			break;
		case EFF_ShootText:
			//EffectShootText(Plane_XY, "9876543210", 1, 600, 50);
			break;
		case EFF_Firework:
			break;
		case EFF_ScrolText:
			break;
		}
	}
//-----------------------------------------------------------------------------
void	Effect_Rain(int iterations, unsigned long itterationDelay) {
	int iterCnt, i, j;
	byte xx, yy;

	SetAllPixelsOff();
	for(iterCnt=0; iterCnt<iterations; iterCnt++) {
		ShiftPlane(Plane_XY, 1, false, false);
		SetPlane(Plane_XY, 7, false);
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
void	Effect_TopDown(byte axis, int seperation, bool direction, int iterations, unsigned long itterationDelay) {
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
					ShiftPlane(Plane_ZX, 1, direction, false);
					SetPlane(Plane_ZX, 7, false);
					break;
				case Axis_Z:
					ShiftPlane(Plane_XY, 1, !direction, false);
					SetPlane(Plane_XY, 7, false);
					break;
				}
			if(j == (seperation - 1)) {
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
			delay(itterationDelay);
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
	int				i, j, len;
	byte			k;
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
	int i, j, k;
	float origin_x = 3;
	float origin_y = 3;
	float origin_z = 3;
	int rand_y, rand_x, rand_z;
	float slowrate, gravity;

	SetAllPixelsOff();
	// Particles and their position, x,y,z and their movement, dx, dy, dz
	float particles[n][6];
	for (i=0; i<iterations; i++) {
		origin_x = rand() % 4;
		origin_y = rand() % 4;
		origin_z = rand() % 2;
		origin_z += 5;
		origin_x += 2;
		origin_y += 2;
		// shoot a particle up in the air
		for (k=0; k<origin_z; k++) {
			SetPixel(k, origin_x, origin_y, true);
			delay(160 * k);
			SetAllPixelsOff();
			}
		// Fill particle array
		for (j=0; j<n; j++) {
			// Position
			particles[j][0] = origin_x;
			particles[j][1] = origin_y;
			particles[j][2] = origin_z;
			rand_x = rand()%200;
			rand_y = rand()%200;
			rand_z = rand()%200;
			// Movement
			particles[j][3] = 1 - (float)rand_x / 100; // dx
			particles[j][4] = 1 - (float)rand_y / 100; // dy
			particles[j][5] = 1 - (float)rand_z / 100; // dz
			}
		// explode
		for (k=0; k<25; k++) {
			slowrate = 1 + tan((k + 0.1) / 20) * 10;
			gravity = tan((k + 0.1) / 20) / 2;
			for (j=0; j<n; j++) {
				particles[j][0] += particles[j][3] / slowrate;
				particles[j][1] += particles[j][4] / slowrate;
				particles[j][2] += particles[j][5] / slowrate;
				particles[j][2] -= gravity;
				SetPixel(particles[j][2],particles[j][0],particles[j][1],true);
				}
			delay(delayTime);
			SetAllPixelsOff();
			}
		}
	}
//-----------------------------------------------------------------------------
void	EffectScrollText(int iterations, String inputstr, int delayTime) {
	int i, j, k;
	String inpStr = inputstr;
	int len = inputstr.length();

	SetAllPixelsOff();
	ResetTextPath();
	for (i=0; i<iterations ; i++) {
		for (j=0 ; j<len ; j++) {
			//
			char Ch = inpStr[j];
			unsigned char ChArr[5] = {0};
			GetCharPattern(Ch, ChArr);
			
			// Add the new chrachter 
			for (k=0 ; k<5 ; k++) {
				//
				AddChToPath(ChArr[k], 0);
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
		for (k=0 ; k<TEXTPATHLEN-4 ; k++) {
			delay(delayTime);
			IncrementPath();
			AddPathToCube();
			}
		}
	}
//-----------------------------------------------------------------------------
void Effect_ShootRandPixel(byte plane, int iterations, int delayTimeSmall, int delayTimeLarge) {
	int 	i, j;
	byte 	x, y, dir;

	for(i=0; i<iterations; i++) {
		dir = random(0,2);
		x = random(0,8);
		y = random(0,8);
		if(dir)
			for(j=1; j<CUBESIZE; j++) {
				switch(plane) {
					case Plane_ZX:
						SetPixel(x, i, y, true);
						SetPixel(x, i-1, y, false);
						delay(delayTimeSmall);
						break;
					case Plane_ZY:
						SetPixel(i, y, x, true);
						SetPixel(i-1, y, x, false);
						delay(delayTimeSmall);
						break;
					case Plane_XY:
						SetPixel(x, y, i, true);
						SetPixel(x, y, i-1, false);
						delay(delayTimeSmall);
						break;
					}
				}
		else
			for(j=6; j>=0; j--) {
				switch(plane) {
					case Plane_ZX:
						SetPixel(x, i, y, true);
						SetPixel(x, i+1, y, false);
						delay(delayTimeSmall);
						break;
					case Plane_ZY:
						SetPixel(i, y, x, true);
						SetPixel(i+1, y, x, false);
						delay(delayTimeSmall);
						break;
					case Plane_XY:
						SetPixel(x, y, i, true);
						SetPixel(x, y, i+1, false);
						delay(delayTimeSmall);
						break;
					}
				}
		delay(delayTimeLarge);
		}
	}
//-----------------------------------------------------------------------------
void	Effect_UpDown_Suspend(int iterations, int SmallDelayTime, int LongDelayTime) {
	/*int itt, i, j, k;


	SetAllPixelsOff();
	for(itt=0; itt<iterations; itt++) {
		// move plane up

		// move plane bottom up

		// move plane down

		//move plain top down

		}*/
	}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------