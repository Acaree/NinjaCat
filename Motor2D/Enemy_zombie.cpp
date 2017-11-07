#include "j1App.h"
#include "Enemy_zombie.h"
#include "mCollision.h"
#include "j1Textures.h"
#include "mPlayer.h"
#include "Enemy.h"
#include "SDL/include/SDL_timer.h"

#include <math.h>



Enemy_Zombie::Enemy_Zombie(int x, int y) : Enemy(x, y)
{
	
	
	//collider = App->collision->AddCollider({ 0, 0,24, 24 }, COLLIDER_TYPE::COLLIDER_FLYING_ENEMY, (Module*)App->enemies);
	for (int i = 0; i < 10; i++)
	{
		base.PushBack({ 0*i,0,86,119 });
	}
	base.speed = 0.2;
	animation = &base;
	originalpos.x = x;
	originalpos.y = y;
	position.x = x;
	position.y = y;
}

void Enemy_Zombie::Move()
{

}

