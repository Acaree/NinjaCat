#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
enum Direction { up=0, down ,left,right};

class ModulePlayer : public j1Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	

public:

	SDL_Texture* graphics = nullptr;

	Animation* currentAnimation = nullptr;
	Animation idleRight;
	Animation idleLeft;
	Animation walkRight;
	Animation walkLeft;
	Animation climb;
	Animation glideRight;
	Animation glideLeft;
	Animation jumpRight;
	Animation jumpLeft;

	iPoint position;
	iPoint respawnTile;
	Collider* colliderPlayer;

	float gravity = 1.0f;
	float original_speed_jump=-20.0f;
	float speed_jump;
	bool jumping = false;
	bool gliding = false;

	bool lookingleft;

	bool movingleft;
	bool movingright;

	int backgroundspeed;

	bool needRespawn;
	bool movement[4] = { true,true,true,true };

};

#endif
