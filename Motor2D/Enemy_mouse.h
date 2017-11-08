#ifndef __ENEMY_MOUSE_H__
#define __ENEMY_MOUSE_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"

class Enemy_Mouse : public Enemy
{
private:

	Animation base;
	
public:

	Enemy_Mouse(int x, int y);
	void Move();
	bool movement2[4] = { true,true,true,true };
	bool play = false;
	bool player_is_up;
	bool player_is_down;
	bool player_is_left;
	bool player_is_right;
	
	int tip = 0;
	p2DynArray<iPoint> enemy_path;
};

#endif