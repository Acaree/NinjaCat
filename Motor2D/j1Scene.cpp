#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "mPlayer.h"
#include "j1Pathfinding.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	map = config.child("map").attribute("name").as_string();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->audio->PlayMusic("audio/music.ogg");
	if (App->map->Load(map.GetString()) == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	App->map->CreateEnemies();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	iPoint p = App->map->WorldToMap(x, y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			//App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//Check if player are dead or jumping , resolve bug player respawn and die for save and load

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
	{
		if (App->entity_m->player->isLevel1 == true)
		{
			App->entity_m->player->needRespawn1 = true;
		}
		else
		{
			App->entity_m->player->changeLevel = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
	{
		if (App->entity_m->player->isLevel1 == true)
		{
			App->entity_m->player->needRespawn1 = true;
		}
		else
		{
			App->entity_m->player->needRespawn2 = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN  && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
	{
		App->LoadGame();
	}

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN  && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (App->entity_m->player->godMode == true)
			App->entity_m->player->godMode = false;
		else
			App->entity_m->player->godMode = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->capped_ms == 1000/30)
			App->capped_ms = 1000/60;
		else
			App->capped_ms = 1000/30;
	}

	App->map->Draw();
	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
