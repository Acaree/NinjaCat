#ifndef __ENEMY_WALK_H__
#define __ENEMY_WALK_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Timer.h"
enum Direction;

class Enemy_Walk : public Enemy
{
private:

	void CalculateGravity();

	Animation walkLeft;
	Animation walkRight;
	Animation deadLeft;
	Animation deadRight;
	//Animation dead;

public:
	fPoint originalpos;

	Enemy_Walk(int x, int y);
	void Move(float dt);
	int i = 0;
	bool current_in_path = false;
	p2DynArray<iPoint> enemy_path;
	bool movement[4] = { true,true,true,true };
	j1Timer timer;
	bool movingLeft = false;

	float gravity = 1.0f;
	float original_speed_jump = -20.0f;
	float speed_jump;
	bool jumping = false;
	bool gliding = false;
	
};

#endif
