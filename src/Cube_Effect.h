#ifndef	__CUBE_EFFECT__
#define	__CUBE_EFFECT__
//-----------------------------------------------------------------------------
#include	<Arduino.h>
//-----------------------------------------------------------------------------
typedef enum {
	EFF_ALL = 0, 		EFF_Rain = 1, 		EFF_TopDown = 2, 
	EFF_ShootText = 3,	EFF_Firework = 4,	EFF_ScrolText = 5,
	}EffectEnum;
//-----------------------------------------------------------------------------
void	EffectLoop(int Effect);
void	Effect_Rain(int iterations, unsigned long itterationDelay);
void	Effect_TopDown(byte axis, int seperation, bool direction,
			int iterations, unsigned long itterationDelay);
void	EffectShootText(byte plane, String inputStr, int iterations,
			int delayTime, int shiftDelayTime);
void	EffectFireworks(int iterations, int n, int delayTime);
void	EffectScrollText(int iterations, String inputstr, int delayTime);
void	Effect_ShootRandPixel(byte plane, int iterations,
			unsigned long delayTimeSmall, unsigned long delayTimeLarge,
			bool StartPlaneOn);
void	Effect_UpDown_Suspend(int iterations, int SmallDelayTime,
			int LongDelayTime);
void	Effect_BoxShrinkGrow(int iterations, int type, int DelayTime);

#endif	// __CUBE_EFFECT__