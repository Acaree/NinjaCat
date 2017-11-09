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
		
/*	pugi::xml_document anim_file;
	pugi::xml_parse_result result = anim_file.load_file("animations.xml");
	pugi::xml_node player_anim = anim_file.child("player");*/
	name.create("player");
	//animations

	/*//idleleft
	pugi::xml_node idleLeft_node = player_anim.child("idleLeft").child("frame");
	while (idleLeft_node != nullptr) {
		idleLeft.PushBack({ idleLeft_node.attribute("x").as_int(),idleLeft_node.attribute("y").as_int(),idleLeft_node.attribute("width").as_int(),idleLeft_node.attribute("height").as_int() });
		idleLeft_node = idleLeft_node.next_sibling();
	}
	idleLeft.speed= player_anim.child("speeds").attribute("idle").as_float();

	//walk right
	pugi::xml_node walkRight_node = player_anim.child("walkRight").child("frame");
	while (walkRight_node != nullptr) {
		walkRight.PushBack({ walkRight_node.attribute("x").as_int(),walkRight_node.attribute("y").as_int(),walkRight_node.attribute("width").as_int(),walkRight_node.attribute("height").as_int() });
		walkRight_node = walkRight_node.next_sibling();
	}
	walkRight.speed= player_anim.child("speeds").attribute("walk").as_float();

	//walk left
	pugi::xml_node walkLeft_node = player_anim.child("walkLeft").child("frame");
	while (walkLeft_node != nullptr) {
		walkLeft.PushBack({ walkLeft_node.attribute("x").as_int(),walkLeft_node.attribute("y").as_int(),walkLeft_node.attribute("width").as_int(),walkLeft_node.attribute("height").as_int() });
		walkLeft_node = walkLeft_node.next_sibling();
	}
	walkLeft.speed= player_anim.child("speeds").attribute("walk").as_float();

	//dead
	pugi::xml_node dead_node = player_anim.child("dead").child("frame");
	while (dead_node != nullptr) {
		dead.PushBack({ dead_node.attribute("x").as_int(),dead_node.attribute("y").as_int(),dead_node.attribute("width").as_int(),dead_node.attribute("height").as_int() });
		dead_node = dead_node.next_sibling();
	}
	dead.speed = player_anim.child("speeds").attribute("dead").as_float();
	dead.loop = player_anim.child("speeds").attribute("loop").as_bool();

	//glide right
	pugi::xml_node glideRight_node = player_anim.child("glideRight").child("frame");
	while (glideRight_node != nullptr) {
		glideRight.PushBack({ glideRight_node.attribute("x").as_int(),glideRight_node.attribute("y").as_int(),glideRight_node.attribute("width").as_int(),glideRight_node.attribute("height").as_int() });
		glideRight_node = glideRight_node.next_sibling();
	}
	glideRight.speed = player_anim.child("speeds").attribute("glide").as_float();

	//glide left
	pugi::xml_node glideLeft_node = player_anim.child("glideLeft").child("frame");
	while (glideLeft_node != nullptr) {
		glideLeft.PushBack({ glideLeft_node.attribute("x").as_int(),glideLeft_node.attribute("y").as_int(),glideLeft_node.attribute("width").as_int(),glideLeft_node.attribute("height").as_int() });
		glideLeft_node = glideLeft_node.next_sibling();
	}
	glideLeft.speed = player_anim.child("speeds").attribute("glide").as_float();

	//jump right
	pugi::xml_node jumpRight_node = player_anim.child("jumpRight").child("frame");
	while (jumpRight_node != nullptr) {
		jumpRight.PushBack({ jumpRight_node.attribute("x").as_int(),jumpRight_node.attribute("y").as_int(),jumpRight_node.attribute("width").as_int(),jumpRight_node.attribute("height").as_int() });
		jumpRight_node = jumpRight_node.next_sibling();
	}
		jumpRight.speed = player_anim.child("speeds").attribute("jump").as_float();
		jumpRight.loop = player_anim.child("speed").attribute("loop").as_bool();

	//jump left
	pugi::xml_node jumpLeft_node = player_anim.child("jumpLeft").child("frame");
	while (jumpLeft_node != nullptr) {
		jumpLeft.PushBack({ jumpLeft_node.attribute("x").as_int(),jumpLeft_node.attribute("y").as_int(),jumpLeft_node.attribute("width").as_int(),jumpLeft_node.attribute("height").as_int() });
		jumpLeft_node = jumpLeft_node.next_sibling();
	}
		jumpLeft.speed = player_anim.child("speeds").attribute("jump").as_float();
		jumpLeft.loop = player_anim.child("speeds").attribute("loop").as_bool();

	currentAnimation = &idleRight;
	lookingleft = false;*/
	
	
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
	
	colliderPlayer = App->collision->AddCollider({ position.x,position.y,80,110 }, COLLIDER_PLAYER, this);
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
	App->collision->CollisionToWorld(colliderPlayer->rect, movement);

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
