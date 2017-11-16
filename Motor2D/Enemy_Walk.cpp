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
#include "Enemy.h"
#include "j1Enemies.h"
#include "j1Textures.h"
Enemy_Walk::Enemy_Walk(int x, int y) : Enemy(x, y)
{

	//collider = App->collision->AddCollider({ 0, 0,24, 24 }, COLLIDER_TYPE::COLLIDER_FLYING_ENEMY, (Module*)App->enemies);
	//base.PushBack({ 0,0,86,119 });
	walkLeft = App->tex->CreateAnimation("zombie", "walkLeft", true);
	walkRight = App->tex->CreateAnimation("zombie", "walkRight", true);
	deadLeft = App->tex->CreateAnimation("zombie", "deadLeft", false);
	deadRight = App->tex->CreateAnimation("zombie", "deadRight", false);
	//dead = App->tex->CreateAnimation("girl", "dead", false);
	animation = &walkLeft;

	originalpos.x=position.x = x;
	originalpos.y=position.y = y;
	

	collider = App->collision->AddCollider({(int)position.x, (int)position.y,86,119 }, COLLIDER_WALKENEMY, App->enemies);
	start_time = SDL_GetTicks();
}

void Enemy_Walk::Move(float dt)
{

	float speed = 30 * dt;

	walkLeft.speed = App->tex->NormalizeAnimSpeed("zombie", "walkLeft", dt);
	walkRight.speed = App->tex->NormalizeAnimSpeed("zombie", "walkRight", dt);
	deadLeft.speed = App->tex->NormalizeAnimSpeed("zombie", "deadLeft", dt);
	deadRight.speed = App->tex->NormalizeAnimSpeed("zombie", "deadRight", dt);

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
	iPoint player_tiles_pos = App->map->WorldToMap(App->player->position.x, App->player->position.y);


	if (player_tiles_pos.x - enemy_tiles_pos.x <= 3 && player_tiles_pos.x - enemy_tiles_pos.x >= -3)
	{
 		App->pathfinding->CreatePathManhattan(enemy_tiles_pos, player_tiles_pos, enemy_path);
		//App->pathfinding->CreatePathManhattan(enemy_tiles_pos, player_tiles_pos, enemy_path);
		//originalpos = App->map->MapToWorld(enemy_tiles_pos.x, enemy_tiles_pos.y);
	}
	/*
	else
	{
		iPoint previousTile = App->map->MapToWorld(enemy_tiles_pos.x - 1, enemy_tiles_pos.y);
		if (position.x == originalpos.x)
		{
			App->pathfinding->CreatePathManhattan(enemy_tiles_pos, {enemy_tiles_pos.x -1 , enemy_tiles_pos.y}, enemy_path);
		}
		else if (movement[left] == false)
		{
			iPoint originTile = App->map->WorldToMap(originalpos.x, originalpos.y);
			App->pathfinding->CreatePathManhattan(enemy_tiles_pos, originTile, enemy_path);

			if(originalpos.x == position.x)
			App->pathfinding->CreatePathManhattan(enemy_tiles_pos, { enemy_tiles_pos.x - 1 , enemy_tiles_pos.y }, enemy_path);
		}

	}*/

	else {
		//guarrada
		//creo que esto se puede hacer con el module timer


		if (start_time + 2000 < SDL_GetTicks()) {
			if (movingLeft) {
				movingLeft = false;
				start_time = SDL_GetTicks();
				animation = &walkLeft;
			}
			else {
				movingLeft = true;
				start_time = SDL_GetTicks();
				animation = &walkRight;
			}
		}

		if (movingLeft)
			position.x += speed;
		else
			position.x -= speed;
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
		jumping = false;
		gliding = false;
		speed_jump = 0;
	}
}
