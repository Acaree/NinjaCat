#include "p2Defs.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "mCollision.h"
#include "mPlayer.h"
#include "j1Audio.h"
#include "j1Module.h"
#include "p2Log.h"
#include "Animation.h"
#include "j1Map.h"

ModulePlayer::ModulePlayer()
{
	idleRight.PushBack({ 0,904,231,438 });
	idleRight.PushBack({ 233,904,231,438 });
	idleRight.PushBack({ 466,904,231,438 });
	idleRight.PushBack({ 699,904,231,438 });

	idleLeft.PushBack({945,904,231,438});
	idleLeft.PushBack({ 1177,904,231,438 });
	idleLeft.PushBack({ 1409,904,231,438 });
	idleLeft.PushBack({ 1641,904,231,438 });

	walkRight.PushBack({35,0,319,446});
	walkRight.PushBack({ 395,0,319,446 });
	walkRight.PushBack({ 755,0,319,446 });
	walkRight.PushBack({ 1115,0,319,446 });
	walkRight.PushBack({ 1475,0,319,446 });
	walkRight.PushBack({ 1835,0,319,446 });
	walkRight.PushBack({2195,0,319,446 });
	walkRight.PushBack({ 2555,0,319,446 });
	walkRight.PushBack({ 2915,0,319,446 });
	walkRight.PushBack({3275,0,319,446 });

	walkLeft.PushBack({ 1,459,319,446 });
	walkLeft.PushBack({ 370,459,319,446 });
	walkLeft.PushBack({ 739,459,319,446 });
	walkLeft.PushBack({ 1108,459,319,446 });
	walkLeft.PushBack({ 1477,459,319,446 });
	walkLeft.PushBack({ 1846,459,319,446 });
	walkLeft.PushBack({ 2215,459,319,446 });
	walkLeft.PushBack({ 2584,459,319,446 });
	walkLeft.PushBack({ 2953,459,319,446 });
	walkLeft.PushBack({ 3322,459,319,446 });
	
	climb.PushBack({36,1374,219,368});
	climb.PushBack({ 270,1374,219,368 });
	climb.PushBack({ 504,1374,219,368 });
	climb.PushBack({ 738,1374,219,368 });
	climb.PushBack({ 972,1374,219,368 });
	climb.PushBack({ 1206,1374,219,368 });
	climb.PushBack({ 1440,1374,219,368 });
	climb.PushBack({ 1674,1374,219,368 });
	
	idleRight.speed = idleLeft.speed = 0.03;
	walkRight.speed = walkLeft.speed = 0.03;
	climb.speed = 0.03;

	currentAnimation = &idleRight;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	graphics = App->tex->Load("maps/spriteSheet.png");

	position.x = 100;
	position.y = 300;

	colliderPlayer = App->collision->AddCollider({position.x,position.y,230,300},COLLIDER_PLAYER,this);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	
	App->tex->UnLoad(graphics);
	
	colliderPlayer->to_delete = true;

	return true;
}

// Update: draw background
bool ModulePlayer::Update(float dt)
{
	int speed = 1;

	App->map->CollisionToWorld(colliderPlayer->rect);
	if (App->input->GetKey(SDL_SCANCODE_D)==KEY_REPEAT && movement[0] == true)
	{
		position.x += speed;
		currentAnimation = &walkRight;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && movement[1] == true)
	{
		position.x -= speed;
		currentAnimation = &walkLeft;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && movement[2]==true)
	{
		position.y -= speed;
		currentAnimation = &climb;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && movement[3] == true)
	{
		position.y += speed;
		
	}
	colliderPlayer->SetPos(position.x, position.y);
	App->render->Blit(graphics, position.x, position.y, &(currentAnimation->GetCurrentFrame()));
	
	return true;
}


void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	
}