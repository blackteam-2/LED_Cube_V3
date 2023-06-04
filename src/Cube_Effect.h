#ifndef	__CUBE_EFFECT__
#define	__CUBE_EFFECT__

typedef enum {
	EFF_ALL = 0, EFF_RAIN = 1, EFF_TOPDOWN = 2
	}EffectEnum;

void	EffectLoop(int Effect);
void	Effect_Rain(int iterations, int itterationDelay);

#endif	// __CUBE_EFFECT__