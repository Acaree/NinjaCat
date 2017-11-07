#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"



struct SDL_Texture;
struct Collider;

class Enemy
{
protected:
	Animation* animation = nullptr;




public:
	iPoint position;
	float vmodule;
	float hitpoints;
	bool death = false;
	uint enemy;
	Collider* collider = nullptr;


public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
};

#endif // __ENEMY_H__
