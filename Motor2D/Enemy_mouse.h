#ifndef __ENEMY_MOUSE_H__
#define __ENEMY_MOUSE_H__

#include "Enemy.h"


class Enemy_Mouse : public Enemy
{
private:
	iPoint originalpos;
	Animation base;
	Uint32 start_time = 0;
	Uint32 now = 0;
	uint timer = 0;
	float pos_x;
	float pos_y;
	float module;
	float v_x;
	float v_y;


public:

	Enemy_Mouse(int x, int y);

	void Move();
	void Shoot();
};

#endif