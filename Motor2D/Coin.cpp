#include "Coin.h"
#include "j1Textures.h"
#include "j1App.h"
#include "mCollision.h"
#include "j1Entities.h"
#include "mCollision.h"
Coin::Coin(int x, int y) : Entity(x, y)
{
	rotateAnimation = App->tex->CreateAnimation("coin","rotate",true);
	collider = App->collision->AddCollider({ x, y,128,128 }, COLLIDER_COIN, App->entity_m);
	animation = &rotateAnimation;
}

void Coin::Move(float dt)
{
	if (!isDead)
	{
		NormalizeAnimations(dt);

		if (isDead = App->collision->CollisionToWorld(collider, movement))
			Dead(dt);
	}

}

void Coin::NormalizeAnimations(float dt)
{
	rotateAnimation.speed = App->tex->NormalizeAnimSpeed("coin", "rotate", dt);
}

void Coin::Dead(float dt)
{
	death = true;

}