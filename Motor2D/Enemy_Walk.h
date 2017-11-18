#ifndef __ENEMY_WALK_H__
#define __ENEMY_WALK_H__

#include "Entity.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Timer.h"
enum Direction;

class Enemy_Walk : public Entity
{
private:

	void CalculateGravity();

	uint zombiesound;

	Animation walkLeft;
	Animation walkRight;
	Animation deadLeft;
	Animation deadRight;

public:
	fPoint originalpos;

	Enemy_Walk(int x, int y);
	void Move(float dt);
	void Dead();
	bool isDead = false;
	int i = 0; // pathcount
	bool current_in_path = false;
	p2DynArray<iPoint> enemy_path;
	j1Timer timer;
	j1Timer soundtimer;
	bool movingLeft = false;
	void NormalizeAnimations(float dt);
	
};

#endif
