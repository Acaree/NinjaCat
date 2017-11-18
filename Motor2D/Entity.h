#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"



struct SDL_Texture;
struct Collider;

enum Direction { up = 0, down, left, right, nextLevel, death };

class Entity
{
protected:
	
	float gravity = 1.0f;
	float speed_jump;
	float speed;


public:
	fPoint position;
	Animation* animation = nullptr;
	float hitpoints;
	bool death = false;
	uint enemy;
	Collider* collider = nullptr;
	bool isPlayer = false;
	bool movement[4] = { true,true,true,true };

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;
	virtual void NormalizeAnimations(float dt) {};
	virtual void CalculateGravity() {};
	virtual void Move(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
};

#endif // __ENEMY_H__
