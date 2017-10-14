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

	//when dead animation introduced, it need to call idle animation
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && movement[down] == false) {
		currentAnimation = &idleRight;
		lookingleft = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP && movement[down] == false)
	{
		currentAnimation = &idleLeft;
		lookingleft = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->map->CollisionToWorld(colliderPlayer->rect, right);
		if (movement[down]==false && (currentAnimation == &idleLeft || currentAnimation == &idleRight)) {
			currentAnimation = &walkRight;
			lookingleft = false;
		}
		if (movement[right] == true)
		{
			position.x += speed;
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->map->CollisionToWorld(colliderPlayer->rect, left);
		if (movement[down] == false && (currentAnimation == &idleLeft || currentAnimation == &idleRight)) {
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
		App->map->CollisionToWorld(colliderPlayer->rect, down);

		if (movement[down] == true)
		{
			gliding = true;
			speed_jump = 3.0f;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || lookingleft == true) {
				currentAnimation = &glideLeft;
				lookingleft = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || lookingleft == false) {
				currentAnimation = &glideRight;
				lookingleft = false;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
		gliding = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumping == false)
	{
			jumping = true;
			speed_jump = original_speed_jump;
			position.y += speed_jump;

			if (lookingleft == true) {
				currentAnimation = &jumpLeft;
			}

			else {
				currentAnimation = &jumpRight;
			}
	}

	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && movement[down] == true && gliding==false) {
		currentAnimation = &jumpLeft;
		lookingleft = true;
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && movement[down] == true && gliding == false) {
		currentAnimation = &jumpRight;
		lookingleft = false;
	}

	App->map->CollisionToWorld(colliderPlayer->rect, down);

	if (gliding == false || movement[down] == true) {
		speed_jump += gravity;
	}

	if (movement[down] == true || jumping == true) {
		position.y += speed_jump;
	}


	if (movement[down] == false && speed_jump > 0) {

			if (lookingleft == true && currentAnimation != &walkLeft) {
				currentAnimation = &idleLeft;
				lookingleft = true;
			}
			else if (lookingleft == false && currentAnimation != &walkRight) {
				currentAnimation = &idleRight;
				lookingleft = false;
			}	
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