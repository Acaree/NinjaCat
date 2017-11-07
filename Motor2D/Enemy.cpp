#include "j1App.h"
#include "Enemy.h"
#include "mCollision.h"
#include "j1Render.h"
#include "j1Audio.h"

Enemy::Enemy(int x, int y) : position(x, y)
{}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;


}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	
	if (animation != nullptr) {
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
	}


}

void Enemy::OnCollision(Collider* collider)
{

}
