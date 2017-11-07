#include "j1App.h"
#include "Enemy_Mouse.h"
#include "mCollision.h"
#include "j1Textures.h"
#include "mPlayer.h"
#include "Enemy.h"
#include "SDL/include/SDL_timer.h"
#include "j1Module.h"
#include <math.h>
#include"j1Enemies.h"
#include "j1Input.h"
#include "j1Map.h"


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

	if (movement2[3] == true)
	{
		position.x += 1;
	}

}

