#include "j1App.h"
#include "Enemy_Mouse.h"
#include "mCollision.h"
#include "j1Textures.h"
#include "mPlayer.h"
#include "Enemy.h"
#include "SDL/include/SDL_timer.h"

#include <math.h>



Enemy_Mouse::Enemy_Mouse(int x, int y) : Enemy(x, y)
{
	
	//collider = App->collision->AddCollider({ 0, 0,24, 24 }, COLLIDER_TYPE::COLLIDER_FLYING_ENEMY, (Module*)App->enemies);
	base.PushBack({ 19,4,49,77 });
	animation = &base;
	originalpos.x = x;
	originalpos.y = y;
	position.x = x;
	position.y = y;
}

void Enemy_Mouse::Move()
{

}

