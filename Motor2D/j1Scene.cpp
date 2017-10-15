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
#include "mPlayer.h"

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
	App->map->Load(map.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//Check if player are dead or jumping , resolve bug player respawn and die for save and load

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->player->currentAnimation != &App->player->dead && App->player->jumping == false)
	{
		if (App->player->isLevel1 == true)
		{
			App->player->needRespawn1 = true;
		}
		else
		{
			App->player->changeLevel = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && App->player->currentAnimation != &App->player->dead && App->player->jumping == false)
	{
		if (App->player->isLevel1 == true)
		{
			App->player->needRespawn1 = true;
		}
		else
		{
			App->player->needRespawn2 = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN  && App->player->currentAnimation != &App->player->dead && App->player->jumping == false) 
	{
		App->LoadGame();
	}

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN  && App->player->currentAnimation != &App->player->dead && App->player->jumping == false)
		App->SaveGame();

	App->map->Draw();
	
	App->input->GetMousePosition(mouseCoordenates.x, mouseCoordenates.y);
	
	mouseTile =App->map->WorldToMap(mouseCoordenates.x, mouseCoordenates.y);

	p2SString title("%i %i    %i %i", mouseCoordenates.x, mouseCoordenates.y, mouseTile.x , mouseTile.y );
	
	App->win->SetTitle(title.GetString());
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
