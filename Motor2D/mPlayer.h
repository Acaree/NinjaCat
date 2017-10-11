#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public j1Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	

	//colisions wall

	bool collW = false;
	bool collA = false;
	bool collS = false;
	bool collD = false;
	int life = 3;
public:
	Animation rectLife;
	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphicsLife = nullptr;
	Animation* current_animation = nullptr;

	Animation idleHappyRight2;
	Animation idleHappyLeft;

	Animation idleMiddleRight;
	Animation idleMiddleLeft;


	Animation idleSadRight;
	Animation idleSadLeft;
	Animation leftHappy;
	Animation leftMiddle;
	Animation leftSad;
	Animation rightHappy;
	Animation rightMiddle;
	Animation rightSad;
	Animation downHappy;
	Animation downMiddle;
	Animation downSad;
	Animation idleHappyDown;
	Animation idleMiddleDown;
	Animation idleSadDown;
	Animation idleUp;
	Animation Up;
	iPoint position;
	Collider* col;

};

#endif
