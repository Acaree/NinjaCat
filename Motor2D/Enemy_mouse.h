#ifndef __ENEMY_MOUSE_H__
#define __ENEMY_MOUSE_H__

#include "Entity.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Timer.h"
enum Direction;

class Enemy_Mouse : public Entity
{
private:
	
	Animation walkLeft;
	Animation walkRight;
	Animation deadRight;
	Animation deadLeft;
	
public:
	iPoint originalpos;

	Enemy_Mouse(int x, int y);
	void Move(float dt);
	void Dead();
	bool isDead = false;
	int i = 0;//pathcount
	bool current_in_path = false;
	p2DynArray<iPoint> enemy_path;
	int now = 0;
	j1Timer timer;
	bool movingLeft = false;
	void NormalizeAnimations(float dt);
};

#endif