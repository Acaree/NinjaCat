#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "mPlayer.h"
#include "j1Enemies.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "Enemy.h"
#include "Enemy_Mouse.h"
#include "j1Map.h"
#include "Enemy_Walk.h"
#include "p2Log.h"

#define SPAWN_MARGIN 140
#define DESPAWN_MARGIN 1400

j1Enemies::j1Enemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;

	name.create("enemies");
}

// Destructor
j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->tex->Load("maps/enemySprites.png");

	
	//COMPROBAR
	for (int it = 0; it < flyPositions.Count(); it++)
	{
		iPoint p = App->map->MapToWorld(flyPositions[it].x, flyPositions[it].y);
		//AddEnemy(ENEMY_MOUSE, p.x, p.y);
	}
	for (int it = 0; it < walkPositions.Count(); it++)
	{
		iPoint p = App->map->MapToWorld(walkPositions[it].x, walkPositions[it].y);
		//AddEnemy(ENEMY_WALK, p.x, p.y);
	}
	flyPositions.Clear();
	walkPositions.Clear();
	//audio_explosion = App->audio->LoadFx("Audio/explosion.wav");
	return true;
}

bool j1Enemies::Awake(pugi::xml_node& config)
{
	//COMPROBAR
	for (pugi::xml_node it = config.child("fly"); it; it = it.next_sibling())
	{
		flyPositions.PushBack({ it.attribute("posx").as_int(),it.attribute("posy").as_int() });
	}
	for (pugi::xml_node it = config.child("zombie"); it; it = it.next_sibling())
	{
		walkPositions.PushBack({ it.attribute("posx").as_int(),it.attribute("posy").as_int() });
	}
	return true;

}
bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{

			if (queue[i].x < (App->render->camera.x - (App->render->camera.w) - SPAWN_MARGIN)*(-1))
			{
  				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %i %i", queue[i].x, queue[i].y);
			}
		}
	}


	return true;
}

// Called before render is available
bool j1Enemies::Update(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			enemies[i]->Move(dt);
		}
	}
	

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			enemies[i]->Draw(sprites);
		}
	}

	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->position.x < (App->render->camera.x + DESPAWN_MARGIN)*(-1))
			{

				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
		}

	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	App->tex->UnLoad(sprites);

	
	// No unload for fxs
	/*
	App->audio->UnloadFX(audio_explosion5);
	*/
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			App->map->data.tile_height;
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{

		case ENEMY_TYPES::NO_TYPE:
			//enemies[i] = new Enemy_Balloon(info.x, info.y);
			break;

		case ENEMY_TYPES::ENEMY_MOUSE:
			enemies[i] = new Enemy_Mouse(info.x, info.y);
			break;
		case ENEMY_TYPES::ENEMY_WALK:
			enemies[i] = new Enemy_Walk(info.x, info.y);
			break;
		}

	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{

}