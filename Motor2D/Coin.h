#ifndef _COIN_
#define _COIN_


#include "Entity.h"
#include "Animation.h"




class Coin : public Entity
{
public:
	Coin(int, int);
	
	void Move(float);
	void NormalizeAnimations(float);
	void Dead(float dt);

private:
	Animation rotateAnimation;
	bool isDead = false;
};


#endif