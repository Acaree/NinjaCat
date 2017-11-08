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
	
	App->collision->CollisionToWorld(collider->rect,movement2 );

	/*if (movement2[3] == true)
	{
		position.x += 1;
	}*/

	iPoint posP = App->map->WorldToMap(position.x, position.y);
	iPoint p = App->map->WorldToMap(App->player->position.x, App->player->position.y);
	

	//range of perception
	if ((posP.x - p.x <= 2 && posP.x - p.x >= -2) && (posP.y - p.x <= 2 && posP.y - p.y >= -2) && play == false )
	{
		play = true;
		App->pathfinding->CreatePath(posP, p, enemy_path);
	}

	if (play == true)
	{

		if (enemy_path[tip].x == posP.x && enemy_path[tip].y == posP.y)
		{
			if (tip + 1 != enemy_path.Count())
			{

				tip++;
				if (enemy_path[tip].x - posP.x == -1)
				{
					iPoint � = App->map->MapToWorld(enemy_path[tip].x, enemy_path[tip].y);
					while (position.x > �.x)
						position.x -= 1;
				}
				else if (enemy_path[tip].x - posP.x == 1)
				{
					iPoint � = App->map->MapToWorld(enemy_path[tip].x, enemy_path[tip].y);
					while (position.x < �.x)
						position.x += 1;
				}
				else if (enemy_path[tip].y - posP.y == 1)
				{
					iPoint � = App->map->MapToWorld(enemy_path[tip].x, enemy_path[tip].y);
					while (position.y < �.y)
						position.y += 1;

				}
				else if (enemy_path[tip].y - posP.y == -1)
				{
					iPoint � = App->map->MapToWorld(enemy_path[tip].x, enemy_path[tip].y);
					while (position.y > �.y)
						position.y -= 1;

				}
			}
			else
			{
				tip = 0;
				play = false;
			}
		}
	}

}

