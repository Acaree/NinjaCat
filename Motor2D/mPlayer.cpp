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
	name.create("player");
	idleRight.PushBack({ 0,227,57,109 });
	idleRight.PushBack({ 59,227,57,109 });
	idleRight.PushBack({ 117,227,57,109 });
	idleRight.PushBack({ 175,227,57,109 });
	idleRight.speed = 0.1f;
	
	idleLeft.PushBack({ 237,227,57,109 });
	idleLeft.PushBack({ 295,227,57,109 });
	idleLeft.PushBack({ 353,227,57,109 });
	idleLeft.PushBack({ 411,227,57,109 });
	idleLeft.speed = idleRight.speed;

	walkRight.PushBack({ 9,3,81,107});
	walkRight.PushBack({ 99,2,80,110 });
	walkRight.PushBack({ 189,0,79,111 });
	//walkRight.PushBack({ 273,1,87,105 });
	//walkRight.PushBack({ 365,3,88,106 });
	walkRight.PushBack({ 463,3,81,105 });
	walkRight.PushBack({ 553,1,79,110 });
	walkRight.PushBack({ 643,0,78,107 });
	walkRight.PushBack({ 734,1,78,100 });
	walkRight.PushBack({ 826,3,81,104 });
	walkRight.speed = 0.2f;

	walkLeft.PushBack({ 1,117,80,107 });
	walkLeft.PushBack({ 93,115,80,112 });
	walkLeft.PushBack({ 186,115,79,110 });
	walkLeft.PushBack({ 275,115,87,104 });
	walkLeft.PushBack({ 364,117,87,106 });
	walkLeft.PushBack({ 455,117,80,105 });
	walkLeft.PushBack({ 547,115,80,109 });
	walkLeft.PushBack({ 640,114,78,107 });
	walkLeft.PushBack({ 730,115,78,100 });
	walkLeft.PushBack({ 818,117,80,104 });
	walkLeft.speed = walkRight.speed;

	climb.PushBack({ 9,344,52,94});
	climb.PushBack({ 68,344,52,94 });
	climb.PushBack({ 127,344,52,94 });
	climb.PushBack({ 184,344,52,94 });
	climb.PushBack({ 241,344,52,94 });
	climb.PushBack({ 303,344,52,94 });
	climb.PushBack({ 363,344,52,94 });
	climb.PushBack({ 419,344,52,94 });
	climb.speed = 0.2f;

	glide.PushBack({ 472,225,102,107 });
	glide.PushBack({ 579,224,102,107 });
	glide.PushBack({ 686,224,102,107 });
	glide.PushBack({ 793,227,102,107 });
	glide.PushBack({ 472,341,102,107 });
	glide.PushBack({ 579,341,102,107 });
	glide.PushBack({ 687,344,102,107 });
	glide.PushBack({ 795,345,102,107 });
	glide.speed = 0.2f;

	jump.PushBack({ 0,466,70,109 });
	jump.PushBack({ 88,464,66,114 });
	jump.PushBack({ 176,463,62,108 });
	jump.PushBack({ 265,463,63,107 });
	jump.PushBack({ 355,463,63,107 });
	jump.PushBack({ 445,463,63,107 });
	jump.PushBack({ 535,463,68,106 });
	jump.PushBack({ 620,463,74,104 });
	jump.PushBack({ 706,463,82,102 });
	jump.PushBack({ 796,463,82,102 });
	jump.speed = 0.1f;
	jump.loop = true;

	currentAnimation = &idleRight;

	
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Awake(pugi::xml_node& config)
{
	
	needRespawn = config.child("level1").attribute("needRespawn").as_bool();
	respawnTile.x = config.child("level1").attribute("respawnX").as_int();
	respawnTile.y = config.child("level1").attribute("respawnY").as_int();


	return true;
}
bool ModulePlayer::Start()
{
	graphics = App->tex->Load("maps/spriteSheet.png");

	//NEED REVISION
	colliderPlayer = App->collision->AddCollider({ position.x,position.y,80,110 }, COLLIDER_PLAYER, this);

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
	if (needRespawn == true)
	{
		iPoint respawnCordenate= App->map->MapToWorld(respawnTile.x, respawnTile.y);
		position.x = respawnCordenate.x;
		position.y = respawnCordenate.y;
		needRespawn = false;
	}
	int speed = 10;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->map->CollisionToWorld(colliderPlayer->rect, right);

		if (movement[right] == true)
		{
			position.x += speed;
			currentAnimation = &walkRight;
		}
		else
		{
			currentAnimation = &idleRight;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->map->CollisionToWorld(colliderPlayer->rect, left);

		if (movement[left] == true)
		{
			position.x -= speed;
			currentAnimation = &walkLeft;
		}
		else
		{
			currentAnimation = &idleLeft;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		App->map->CollisionToWorld(colliderPlayer->rect, up);

		if (movement[up] == true)
		{
			position.y -= speed;
			currentAnimation = &climb;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		App->map->CollisionToWorld(colliderPlayer->rect, down);

		if (movement[down] == true)
		{
			gliding = true;
			speed_jump = 3.0f;
			currentAnimation = &glide;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumping == false)
	{

		//if (movement[down] == false)
		{
			jumping = true;
			speed_jump = original_speed_jump;
			position.y += speed_jump;
			currentAnimation = &jump;
		}
	}

	//	App->map->CollisionToWorld(colliderPlayer->rect, up);


	App->map->CollisionToWorld(colliderPlayer->rect, down);

	if (gliding == false || movement[down] == true) {
		speed_jump += gravity;
	}

	if (movement[down] == true || jumping == true) {
		position.y += speed_jump;
	}


	if (movement[down] == false && speed_jump > 0) {
		jumping = false;
		gliding = false;
		speed_jump = 0;
	}
	colliderPlayer->SetPos(position.x, position.y);
	App->render->Blit(graphics, position.x, position.y, &(currentAnimation->GetCurrentFrame()));
	
	return true;
}


void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	
}