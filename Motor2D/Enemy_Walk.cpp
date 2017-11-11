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
	//dead = App->tex->CreateAnimation("girl", "dead", false);
	animation = &walkLeft;

	position.x = x;
	position.y = y;


	collider = App->collision->AddCollider({ position.x, position.y,86,119 }, COLLIDER_ENEMY, App->enemies);
}

void Enemy_Walk::Move()
{
	App->collision->CollisionToWorld(collider, movement);
	if(movement[down] == true)
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


	App->pathfinding->CreatePathManhattan(enemy_tiles_pos, player_tiles_pos, enemy_path);

	if (i < enemy_path.Count()) { //enemy_path[i] != nullptr
								  //No faltaba comprobar que enemy_path era null eso ya lo hace el count, el player entraba en la siguiente tile y ya te detectaba la comparacion pero se quedaba tocando
								  // el iPoint tileInMap coje la posicion en el mapa de la tile para dirijir al enemigo hasta los puntos de la esquina opuesta asi hace el path bien y se queda dentro de la tile
		iPoint tileInMap = App->map->MapToWorld(enemy_path[i].x, enemy_path[i].y);

		if (enemy_tiles_pos.x <= enemy_path[i].x && position.x < tileInMap.x && movement[right] == true) {
			position.x += 1;
			animation = &walkRight;
			current_in_path = true;
		}
		else if (enemy_tiles_pos.x >= enemy_path[i].x && position.x > tileInMap.x && movement[left] == true) {
			position.x -= 1;
			animation = &walkLeft;
			current_in_path = true;
		}
		else if (enemy_tiles_pos.y <= enemy_path[i].y && position.y < tileInMap.y && movement[up] == true) {
			position.y += 1;
			current_in_path = true;
		}
		else if (enemy_tiles_pos.y >= enemy_path[i].y && position.y > tileInMap.y && movement[down] == true) {
			position.y -= 1;
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
