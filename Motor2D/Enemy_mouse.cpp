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
#include "Enemy_mouse.h"
#include "Enemy.h"
#include "j1Enemies.h"
Enemy_Mouse::Enemy_Mouse(int x, int y) : Enemy(x, y)
{
	
	//collider = App->collision->AddCollider({ 0, 0,24, 24 }, COLLIDER_TYPE::COLLIDER_FLYING_ENEMY, (Module*)App->enemies);
	base.PushBack({ 0,0,86,119 });
	animation = &base;
	
	position.x = x;
	position.y = y;

	
	collider = App->collision->AddCollider({ position.x, position.y,86,119 }, COLLIDER_ENEMY, App->enemies );
}

void Enemy_Mouse::Move()
{
	
	iPoint enemy_tiles_pos = App->map->WorldToMap(position.x, position.y);
	iPoint player_tiles_pos = App->map->WorldToMap(App->player->position.x, App->player->position.y);

	
	//if(player_tiles_pos.x - enemy_tiles_pos.x <= 2 && player_tiles_pos.x - enemy_tiles_pos.x >= -2 && player_tiles_pos.y - enemy_tiles_pos.y <= 2 && player_tiles_pos.y - enemy_tiles_pos.y >= -2)
	//{
		App->pathfinding->CreatePath(enemy_tiles_pos, player_tiles_pos, enemy_path);
	//}

	if (i < enemy_path.Count()) { //enemy_path[i] != nullptr
		//No faltaba comprobar que enemy_path era null eso ya lo hace el count, el player entraba en la siguiente tile y ya te detectaba la comparacion pero se quedaba tocando
		// el iPoint tileInMap coje la posicion en el mapa de la tile para dirijir al enemigo hasta los puntos de la esquina opuesta asi hace el path bien y se queda dentro de la tile
		iPoint tileInMap = App->map->MapToWorld(enemy_path[i].x, enemy_path[i].y);

		if (enemy_tiles_pos.x <= enemy_path[i].x && position.x < tileInMap.x) {
			position.x += 1;
			current_in_path = true;
		}
		else if (enemy_tiles_pos.x >= enemy_path[i].x && position.x > tileInMap.x) {
			position.x -= 1;
			current_in_path = true;
		}
		else if (enemy_tiles_pos.y <= enemy_path[i].y && position.y < tileInMap.y) {
			position.y += 1;
			current_in_path = true;
		}
		else if (enemy_tiles_pos.y >= enemy_path[i].y && position.y > tileInMap.y) {
			position.y -= 1;
			current_in_path = true;
		}
		else {
			current_in_path = false;
		}
		
		
		if (current_in_path==false)
		i++;
	}

}