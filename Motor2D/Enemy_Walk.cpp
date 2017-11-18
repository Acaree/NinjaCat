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
#include "j1Pathfinding.h"
#include "p2DynArray.h"
#include "Enemy_Walk.h"
#include "Entity.h"
#include "j1Entities.h"
#include "j1Textures.h"

Enemy_Walk::Enemy_Walk(int x, int y) : Entity(x, y)
{

	//collider = App->collision->AddCollider({ 0, 0,24, 24 }, COLLIDER_TYPE::COLLIDER_FLYING_ENEMY, (Module*)App->enemies);
	//base.PushBack({ 0,0,86,119 });
	walkLeft = App->tex->CreateAnimation("zombie", "walkLeft", true);
	walkRight = App->tex->CreateAnimation("zombie", "walkRight", true);
	deadLeft = App->tex->CreateAnimation("zombie", "deadLeft", false);
	deadRight = App->tex->CreateAnimation("zombie", "deadRight", false);
	//dead = App->tex->CreateAnimation("girl", "dead", false);
	zombiesound = App->audio->LoadFx("audio/zombie.wav");

	originalpos.x=position.x = x;
	originalpos.y=position.y = y;
	

	collider = App->collision->AddCollider({(int)position.x, (int)position.y,86,119 }, COLLIDER_WALKENEMY, App->entity_m);
	timer.Start();
	soundtimer.Start();
}

void Enemy_Walk::Move(float dt)
{

	if (soundtimer.Read() > 10000) {
		App->audio->PlayFx(zombiesound, 1);
		soundtimer.Start();
	}

	speed = 30 * dt;

	death = App->collision->CollisionToWorld(collider, movement);
	
	if (movement[down] == true)
		CalculateGravity();
	/*if (movement[death] == true)  //need think
	{
	if (now == 0) {
	now = SDL_GetTicks();

	}
	if (now + 800 > SDL_GetTicks()) {
	animation = &deadFly;
	}
	collider->to_delete = true;
	}*/

	iPoint enemy_tiles_pos = App->map->WorldToMap(position.x, position.y);
	iPoint player_tiles_pos = App->map->WorldToMap(App->entity_m->player->position.x, App->entity_m->player->position.y);


	if (player_tiles_pos.x - enemy_tiles_pos.x <= 3 && player_tiles_pos.x - enemy_tiles_pos.x >= -3)
	{
 		App->pathfinding->CreatePathManhattan(enemy_tiles_pos, player_tiles_pos, enemy_path);
	
	}
	else {
		
		if (timer.Read()>2000) {
		
			if (movingLeft) {
				movingLeft = false;
				timer.Start();
				animation = &walkLeft;
			}
			else {
				movingLeft = true;
				timer.Start();
				animation = &walkRight;
			}
		}
		
		if (movingLeft)
			App->pathfinding->CreatePathManhattan(enemy_tiles_pos, { enemy_tiles_pos.x + 1 , enemy_tiles_pos.y }, enemy_path);
		else
			App->pathfinding->CreatePathManhattan(enemy_tiles_pos, { enemy_tiles_pos.x - 1 , enemy_tiles_pos.y }, enemy_path);
			
			
	}
	
		if (i < enemy_path.Count()) {
			iPoint tileInMap = App->map->MapToWorld(enemy_path[i].x, enemy_path[i].y);

			if (death == true)
			{

				animation = &deadLeft;
			}
			else if (enemy_tiles_pos.x <= enemy_path[i].x && position.x < tileInMap.x && movement[right] == true) {
				position.x += speed;
				animation = &walkRight;
				current_in_path = true;
			}
			else if (enemy_tiles_pos.x >= enemy_path[i].x && position.x > tileInMap.x && movement[left] == true) {
				position.x -= speed;
				animation = &walkLeft;
				current_in_path = true;
			}
			else {
				current_in_path = false;
			}

			if (current_in_path == false)
				i++;
		}
		else {
			i = 0;
		}

	}


void Enemy_Walk::CalculateGravity() {
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
		speed_jump = 0;
	}
}

void Enemy_Walk::NormalizeAnimations(float dt) {


		walkLeft.speed = App->tex->NormalizeAnimSpeed("zombie", "walkLeft", dt);
		walkRight.speed = App->tex->NormalizeAnimSpeed("zombie", "walkRight", dt);
		deadLeft.speed = App->tex->NormalizeAnimSpeed("zombie", "deadLeft", dt);
		deadRight.speed = App->tex->NormalizeAnimSpeed("zombie", "deadRight", dt);

}