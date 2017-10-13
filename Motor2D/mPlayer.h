#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
enum Direction { up=0, down ,left,right, jumpUp};

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
	Animation glide;
	Animation jump;

	iPoint position;
	iPoint respawnTile;
	Collider* colliderPlayer;

	bool needRespawn;
	bool movement[4] = { true,true,true,true };

};

#endif
