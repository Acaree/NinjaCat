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

	dead.PushBack({ 7,816,65,107});
	dead.PushBack({ 80,820,83,106 });
	dead.PushBack({ 169,835,94,91 });
	dead.PushBack({ 288,850,102,94 });
	dead.PushBack({ 408,855,108,69 });
	dead.PushBack({ 647,860,109,64 });
	dead.PushBack({ 767,860,110,64 });
	dead.speed = 0.2f;
	dead.loop = false;

	glideRight.PushBack({ 472,225,102,107 });
	glideRight.PushBack({ 579,224,102,107 });
	glideRight.PushBack({ 686,224,102,107 });
	glideRight.PushBack({ 793,227,102,107 });
	glideRight.PushBack({ 472,341,102,107 });
	glideRight.PushBack({ 579,341,102,107 });
	glideRight.PushBack({ 687,344,102,107 });
	glideRight.PushBack({ 795,345,102,107 });
	glideRight.speed = 0.2f;

	glideLeft.PushBack({ 472,585,102,107 });
	glideLeft.PushBack({ 579,584,102,107 });
	glideLeft.PushBack({ 686,584,102,107 });
	glideLeft.PushBack({ 793,587,102,107 });
	glideLeft.PushBack({ 472,701,102,107 });
	glideLeft.PushBack({ 579,700,102,107 });
	glideLeft.PushBack({ 684,700,102,107 });
	glideLeft.PushBack({ 790,703,102,107 });
	glideLeft.speed = 0.2f;

	jumpRight.PushBack({ 0,466,70,109 });
	jumpRight.PushBack({ 88,464,66,114 });
	jumpRight.PushBack({ 176,463,62,108 });
	jumpRight.PushBack({ 265,463,63,107 });
	jumpRight.PushBack({ 355,463,63,107 });
	jumpRight.PushBack({ 445,463,63,107 });
	jumpRight.PushBack({ 535,463,68,106 });
	jumpRight.PushBack({ 620,463,74,104 });
	jumpRight.PushBack({ 706,463,82,102 });
	jumpRight.PushBack({ 796,463,82,102 });
	jumpRight.speed = 0.1f;
	jumpRight.loop = false;

	jumpLeft.PushBack({ 364,586,66,114 });
	jumpLeft.PushBack({ 279,584,62,108 });
	jumpLeft.PushBack({ 195,583,63,107 });
	jumpLeft.PushBack({ 105,583,63,107 });
	jumpLeft.PushBack({ 15,583,63,107 });
	jumpLeft.PushBack({ 371,699,68,106 });
	jumpLeft.PushBack({ 276,699,74,104 });
	jumpLeft.PushBack({ 185,699,82,102 });
	jumpLeft.PushBack({ 91,699,82,102 });
	jumpLeft.PushBack({ 2,699,82,102 });
	jumpLeft.speed = 0.1f;
	jumpLeft.loop = false;

	currentAnimation = &idleRight;
	lookingleft = false;

	
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Awake(pugi::xml_node& config)
{
	
	needRespawn1 = config.child("level1").attribute("needRespawn").as_bool();
	respawnTile1.x = config.child("level1").attribute("respawnX").as_int();
	respawnTile1.y = config.child("level1").attribute("respawnY").as_int();

	needRespawn2 = config.child("level2").attribute("needRespawn").as_bool();
	respawnTile2.x= config.child("level2").attribute("respawnX").as_int();
	respawnTile2.y = config.child("level2").attribute("respawnY").as_int();
	return true;
}
bool ModulePlayer::Start()
{
	graphics = App->tex->Load("maps/spriteSheet.png");
	jumpsound=App->audio->LoadFx("audio/jump_fx.wav");
	glidesound = App->audio->LoadFx("audio/glide.wav");
	
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
	App->map->CollisionToWorld(colliderPlayer->rect, movement);

	int speed = 10;

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		changeLevel = true;
	}

	if (needRespawn1 == true || needRespawn2 == true || changeLevel == true) {
		Respawn();
	}
	
	if (currentAnimation != &dead) {


		if (movement[down] == true) {
			CalculateGravity();
		}
		else {
			if (lookingleft == true) {
				currentAnimation = &idleLeft;
			}
			else {
				currentAnimation = &idleRight;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && movement[down] == false) {
			currentAnimation = &idleRight;
			lookingleft = false;
		}


		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && movement[down] == true && gliding == false) {
			currentAnimation = &jumpRight;
			lookingleft = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			if (movement[down] == false) {
				currentAnimation = &walkRight;
				lookingleft = false;
			}
			if (movement[right] == true)
			{
				position.x += speed;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP && movement[down] == false)
		{
			currentAnimation = &idleLeft;
			lookingleft = true;
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && movement[down] == true && gliding == false) {
			currentAnimation = &jumpLeft;
			lookingleft = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{

			if (movement[down] == false) {
				currentAnimation = &walkLeft;
				lookingleft = true;
			}
			if (movement[left] == true)
			{
				position.x -= speed;
			}
		}


		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{


			if (movement[down] == true)
			{

				speed_jump = 3.0f;
				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || lookingleft == true) {
					currentAnimation = &glideLeft;
					if (gliding == false) {
						App->audio->PlayFx(glidesound, 1);
						gliding = true;
					}
					lookingleft = true;
				}
				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || lookingleft == false) {
					currentAnimation = &glideRight;
					if (gliding == false) {
						App->audio->PlayFx(glidesound, 1);
						gliding = true;
					}
					lookingleft = false;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
			gliding = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumping == false && movement[down]==false)
		{
			jumping = true;
			speed_jump = original_speed_jump;
			position.y += speed_jump;

			if (lookingleft == true) {
				App->audio->PlayFx(jumpsound);
				currentAnimation = &jumpLeft;
			}

			else {
				App->audio->PlayFx(jumpsound);
				currentAnimation = &jumpRight;
			}
		}


	}

		App->render->camera.x = 200 - position.x;
		App->render->camera.y = 300 - position.y;
		colliderPlayer->SetPos(position.x, position.y);
		App->render->Blit(graphics, position.x, position.y, &(currentAnimation->GetCurrentFrame()));

		if (currentAnimation != &jumpLeft) {
			jumpLeft.Reset();
		}
		if (currentAnimation != &jumpRight) {
			jumpRight.Reset();
		}
		if (currentAnimation != &dead) {
			dead.Reset();
		}


	return true;
}


void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	
}

bool ModulePlayer::Save(pugi::xml_node& config) const
{
	pugi::xml_node player = config.append_child("player");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;
	player.append_attribute("level1") = isLevel1;
	return true;
}

bool ModulePlayer::Load(pugi::xml_node& data)
{
	bool tmp = data.child("player").attribute("level1").as_bool();
		
	if (tmp != isLevel1)
	{
		changeLevel = true;
		loadRespawn = true;
	}
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();
	

	return true;
}

void ModulePlayer::CalculateGravity() {
	//Trap for colliders work "good" 
	if (speed_jump < 20)
	{
		speed_jump += gravity;
	}
	else
	{
		speed_jump = 20;
	}

	position.y += speed_jump;

	if (movement[down] == false && speed_jump > 0)
	{
		jumping = false;
		gliding = false;
		speed_jump = 0;
	}
}

void ModulePlayer::Respawn()
{

	if (changeLevel == true) {
		if (currentAnimation != &dead) {
			App->map->CleanUp();
			changeLevel = false;
			if (isLevel1 == true)
			{

				App->map->Load("level2ND.tmx");
				needRespawn2 = true;
			}
			else if (isLevel1 == false)
			{

				App->map->Load("level1ND.tmx");
				needRespawn1 = true;
			}
			currentAnimation = &idleRight;
		}
	}

	if (needRespawn1 == true)
	{
		if (loadRespawn == false)
		{
			iPoint respawnCordenate = App->map->MapToWorld(respawnTile1.x, respawnTile1.y);
			position.x = respawnCordenate.x;
			position.y = respawnCordenate.y;
		}
		needRespawn1 = false;
		isLevel1 = true;
	}
	else if (needRespawn2 == true)
	{
		if (loadRespawn == false)
		{

			iPoint respawnCordenate = App->map->MapToWorld(respawnTile2.x, respawnTile2.y);
			position.x = respawnCordenate.x;
			position.y = respawnCordenate.y;
		}
		needRespawn2 = false;
		isLevel1 = false;

	}

	loadRespawn = false;
	movement[down] = true;

	
}
