#ifndef __j1Enemies_H__
#define __j1Enemies_H__

#include "j1Module.h"

#define MAX_ENEMIES 100

enum ENEMY_TYPES
{
	NO_TYPE,
	ENEMY_MOUSE,
	
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

class j1Enemies : public j1Module
{
public:

	j1Enemies();
	~j1Enemies();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y);
	uint audio_explosion;
	uint audio_explosion2;
	uint audio_explosion3;
	uint audio_explosion4;
	uint audio_explosion5;

private:

	void SpawnEnemy(const EnemyInfo& info);

private:
	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];
	SDL_Texture* sprites;
public:
	float damage;
	bool bossdefeated = false;
};

#endif // __ModuleEnemies_H_
