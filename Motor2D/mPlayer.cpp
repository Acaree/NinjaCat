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
#include "j1Entities.h"

Player::Player(int x, int y) : Entity(x, y)
{
	pugi::xml_document config_file;
	pugi::xml_node config;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	config = config_file.child("config");

	needRespawn1 = config.child("level1").attribute("needRespawn").as_bool();
	respawnTile1.x = config.child("level1").attribute("respawnX").as_int();
	respawnTile1.y = config.child("level1").attribute("respawnY").as_int();

	needRespawn2 = config.child("level2").attribute("needRespawn").as_bool();
	respawnTile2.x = config.child("level2").attribute("respawnX").as_int();
	respawnTile2.y = config.child("level2").attribute("respawnY").as_int();


	graphics = App->tex->Load("maps/spriteSheet.png");
	jumpsound = App->audio->LoadFx("audio/jump_fx.wav");
	glidesound = App->audio->LoadFx("audio/glide.wav");

	idleRight = App->tex->CreateAnimation("player", "idleRight", true);
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

	collider = App->collision->AddCollider({ (int)position.x,(int)position.y,80,110 }, COLLIDER_PLAYER, App->entity_m);
	animation = &idleRight;

	

	death = false;
}

Player::~Player()
{
	App->tex->UnLoad(graphics);
	


}
	//App->tex->UnLoad(graphics);


// Load assets

// Unload assets

// Update: draw background
void Player::Move(float dt)
{
	
		App->collision->CollisionToWorld(collider, movement);

		speed = 300 * dt;

		InputsPlayer(movement, dt);
		
		App->render->camera.x = 200 - position.x;
		App->render->camera.y = 300 - position.y;

		if (attacking == false)
		{
			collider->SetPos(position.x, position.y);
		}
		else if (lookingleft)
		{
			collider->SetPos(position.x - 40, position.y);
		}
		else
		{
			collider->SetPos(position.x+20, position.y);
		}

		ResetAnimations();
	
	if (animation == &hitLeft)
	{
		App->render->Blit(graphics, position.x - 60, position.y, &(animation->GetCurrentFrame()));
	}
	else
	{
		App->render->Blit(graphics, position.x, position.y, &(animation->GetCurrentFrame()));
	}

}

void Player::InputsPlayer(bool* movement, float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		changeLevel = true;
	}

	if (needRespawn1 == true || needRespawn2 == true || changeLevel == true) {
		Respawn();
	}

	if (death == true) {
		Dead();
	}

	if (animation != &dead && dt != 0) {



		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && attacking == false)
		{
			
			if (lookingleft) {
				animation = &hitLeft;
				started_attack = SDL_GetTicks();
				attacking = true;
			}
			else {
				animation = &hitRight;
				started_attack = SDL_GetTicks();
				attacking = true;
			}


		}

		if (started_attack + 500 < SDL_GetTicks() && attacking == true) {
			if (lookingleft)
			{
				animation = &idleLeft;
				
			}
			else
			{
				animation = &idleRight;
			}
			App->collision->EraseCollider(attack_collider);
			started_attack = 0;
			attacking = false;
			
		}

			if (collider->to_delete==false)
			CalculateGravity();
		

		if (attacking == false) {
			if (lookingleft == true) {
				animation = &idleLeft;
			}
			else {
				animation = &idleRight;
			}


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				lookingleft = true;
			}


			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				lookingleft = false;
			}

			if (movement[down] == true && gliding == false) {
				if (lookingleft)
					animation = &jumpLeft;
				else
					animation = &jumpRight;
			}

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{

				if (movement[down] == false) {
					animation = &walkRight;
				}
				if (movement[right] == true)
				{
					position.x += speed;
				}
			}



			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{

				if (movement[down] == false) {
					animation = &walkLeft;
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
						animation = &glideLeft;
						if (gliding == false) {
							App->audio->PlayFx(glidesound, 1);
							gliding = true;
						}
					}
					if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || lookingleft == false) {
						animation = &glideRight;
						if (gliding == false) {
							App->audio->PlayFx(glidesound, 1);
							gliding = true;
						}
					}
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
				gliding = false;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumping == false && movement[down] == false && dt != 0)
			{
				jumping = true;
				speed_jump = original_speed_jump;
				position.y += speed_jump;
				App->audio->PlayFx(jumpsound);

				if (lookingleft == true) {
					animation = &jumpLeft;
				}

				else {
					animation = &jumpRight;
				}
			}
		}
	}

}

void Player::NormalizeAnimations(float dt)
{
	
		idleRight.speed = App->tex->NormalizeAnimSpeed("player", "idleRight", dt);
		idleLeft.speed = App->tex->NormalizeAnimSpeed("player", "idleLeft", dt);
		walkRight.speed = App->tex->NormalizeAnimSpeed("player", "walkRight", dt);
		walkLeft.speed = App->tex->NormalizeAnimSpeed("player", "walkLeft", dt);
		dead.speed = App->tex->NormalizeAnimSpeed("player", "dead", dt);
		glideRight.speed = App->tex->NormalizeAnimSpeed("player", "glideRight", dt);
		glideLeft.speed = App->tex->NormalizeAnimSpeed("player", "glideLeft", dt);
		jumpRight.speed = App->tex->NormalizeAnimSpeed("player", "jumpRight", dt);
		jumpLeft.speed = App->tex->NormalizeAnimSpeed("player", "jumpLeft", dt);
		hitRight.speed = App->tex->NormalizeAnimSpeed("player", "hitRight", dt);
		hitLeft.speed = App->tex->NormalizeAnimSpeed("player", "hitLeft", dt);
}
void Player::ResetAnimations()
{
	if (animation != &jumpLeft) {
		jumpLeft.Reset();
	}
	if (animation != &jumpRight) {
		jumpRight.Reset();
	}
	if (animation != &dead) {
		dead.Reset();
	}
	if (animation != &hitLeft) {
		hitLeft.Reset();
	}
	if (animation != &hitRight) {
		hitRight.Reset();
	}

}



void Player::CalculateGravity() {
	//Trap for colliders work "good" 
	if (movement[down] == true) {
		if (speed_jump < 20)
		{
			speed_jump += gravity;
		}
		else
		{
			speed_jump = 20;
		}

		position.y += speed_jump;
	}
	if (movement[down] == false && speed_jump > 0)
	{
		jumping = false;
		gliding = false;
		speed_jump = 0;
	}
}

void Player::Respawn()
{
	// set de level, clean up and load map
	death = false;

	if (changeLevel == true) {
		if (animation != &dead) {
			App->map->CleanUp();
			changeLevel = false;
			if (App->map->isLevel1 == true)
			{

				App->map->Load("level2ND.tmx");
				App->map->isLevel1 = false;
				needRespawn2 = true;
			}
			else if (App->map->isLevel1 == false)
			{
				App->map->Load("level1ND.tmx");
				App->map->isLevel1 = true;
				needRespawn1 = true;
			}
			animation = &idleRight;
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
		//isLevel1 = true;
		App->map->CreateEnemies();
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
		//isLevel1 = false;
		App->map->CreateEnemies();
	}
	animation = &idleRight;
	//load respawn: if is true don't take the position of tile in config.xml take the position in save_game.xml
	loadRespawn = false;
	// movement true : collider respawn > collider player
	movement[down] = true;

	
}

void Player::Dead()
{

	if (now == 0) {
		now = SDL_GetTicks();
		
	}
	if (now + 1000 > SDL_GetTicks()) {
		animation = &dead;
		// stop all movement, else player go out of map, bug
		movement[down] = false;
		movement[left] = false;
		movement[right] = false;
	}
	else {
		animation = &idleRight;
		now = 0;
		//dead_start = false;
		
		if (App->map->isLevel1)
		{
			needRespawn1 = true;
		}
		else
		{
			needRespawn2 = true;
		}
		movement[down] = false;
	}

}

void Player::Draw(SDL_Texture* sprites) {}
void Player::OnCollision(Collider* collider) {}