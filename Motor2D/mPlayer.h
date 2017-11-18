#ifndef __Player_H__
#define __Player_H__

#include "Entity.h"
#include "mCollision.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
enum Direction;

class Player : public Entity
{
public:
	Player(int x, int y);

	void Move(float dt);
	void Draw(SDL_Texture* sprites);
	void OnCollision(Collider* collider);
	
	void CalculateGravity();
	void Respawn();

	void ResetAnimations();
	void NormalizeAnimations(float dt);
	void InputsPlayer(bool* movement, float dt);
	void Dead();
public:

	SDL_Texture* graphics = nullptr;

	Animation idleRight;
	Animation idleLeft;
	Animation walkRight;
	Animation walkLeft;
	Animation dead;
	Animation glideRight;
	Animation glideLeft;
	Animation jumpRight;
	Animation jumpLeft;
	Animation hitRight;
	Animation hitLeft;

	uint jumpsound;
	uint glidesound;


	iPoint respawnTile1;
	iPoint respawnTile2;
	
	float original_speed_jump=-20.0f;
	bool jumping = false;
	bool gliding = false;

	bool lookingleft;
	bool attacking = false;

	int backgroundspeed;

	bool godMode = false;
	bool needRespawn1;
	bool needRespawn2;
	
	bool changeLevel =false;
	bool loadRespawn = false;
	int started_attack = 0;
	Collider* attack_collider;
	float last_dt;
	int now = 0;
};

#endif
