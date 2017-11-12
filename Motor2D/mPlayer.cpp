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

	idleRight= App->tex->CreateAnimation("player", "idleRight", true);
	idleLeft = App->tex->CreateAnimation("player", "idleLeft", true);
	walkRight = App->tex->CreateAnimation("player", "walkRight", true);
	walkLeft = App->tex->CreateAnimation("player", "walkLeft", true);
	dead = App->tex->CreateAnimation("player", "dead", false);
	glideRight = App->tex->CreateAnimation("player", "glideRight", true);
	glideLeft = App->tex->CreateAnimation("player", "glideLeft", true);
	jumpRight = App->tex->CreateAnimation("player", "jumpRight", false);
	jumpLeft = App->tex->CreateAnimation("player", "jumpLeft", false);
	hitLeft = App->tex->CreateAnimation("player", "hitLeft", false);
	hitRight = App->tex->CreateAnimation("player", "hitRight", false);
	
	colliderPlayer = App->collision->AddCollider({ (int)position.x,(int)position.y,80,110 }, COLLIDER_PLAYER, this);
	currentAnimation = &idleRight;
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
	App->collision->CollisionToWorld(colliderPlayer, movement);
		
	float speed = 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		if (lookingleft) {
			currentAnimation = &hitLeft;
			started_attack = SDL_GetTicks();
		}
		else {
			currentAnimation = &hitRight;
			started_attack = SDL_GetTicks();
		}
	}

	if (started_attack + 500 < SDL_GetTicks()) {
		if (lookingleft)
			currentAnimation = &idleLeft;
		else
			currentAnimation = &idleRight;
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		changeLevel = true;
	}

	if (needRespawn1 == true || needRespawn2 == true || changeLevel == true) {
		Respawn();
	}
	
	if (currentAnimation != &dead && currentAnimation != &hitLeft && currentAnimation != &hitRight) {


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
		if (currentAnimation != &hitLeft) {
			hitLeft.Reset();
		}
		if (currentAnimation != &hitRight) {
			hitRight.Reset();
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
	// set de level, clean up and load map
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
	//load respawn: if is true don't take the position of tile in config.xml take the position in save_game.xml
	loadRespawn = false;
	// movement true : collider respawn > collider player
	movement[down] = true;

	
}
