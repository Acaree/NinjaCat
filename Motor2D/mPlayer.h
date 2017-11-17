#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "mCollision.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
enum Direction { up=0, down ,left,right, nextLevel, death};

class ModulePlayer : public j1Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	
	void CalculateGravity();
	void Respawn();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void ResetAnimations();
	void NormalizeAnimations(float dt,float last_dt);
	void InputsPlayer(bool* movement, float dt);
	void Dead();
public:

	SDL_Texture* graphics = nullptr;

	Animation* currentAnimation = nullptr;
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

	fPoint position;
	iPoint respawnTile1;
	iPoint respawnTile2;
	Collider* colliderPlayer;

	float gravity = 1.0f;
	float original_speed_jump=-20.0f;
	float speed_jump;
	bool jumping = false;
	bool gliding = false;

	bool lookingleft;
	bool attacking;

	bool movingleft;
	bool movingright;

	bool dead_start;

	int backgroundspeed;

	bool godMode = false;
	bool needRespawn1;
	bool needRespawn2;
	bool isLevel1;
	bool movement[4] = { true,true,true,true };
	bool changeLevel =false;
	bool loadRespawn = false;
	float speed;
	int started_attack = 0;
	Collider* attack_collider;
	float last_dt;
	int now = 0;
};

#endif
