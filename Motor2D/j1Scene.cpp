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
#include "j1FadeToBlack.h"
#include "j1Gui.h"

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
	CreateMainScene();
	actualScene = Main_scene;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	switch (actualScene)
	{
	case Main_scene:
		if (buttons.count() != 0)
		{
			if (playButton->eventElement == MouseLeftClickEvent)
			{
				App->fade->FadeToBlack("level1ND.tmx", 2.0);
				App->map->level = level_1;
				actualScene = Levels_scene;
				buttons.clear();
				CreateLevelScene();
			}
			else if (settingsButton->eventElement == MouseLeftClickEvent)
			{
				CreateSettingsScene();
				actualScene = Settings_scene;
			}
			else if (quitButton->eventElement == MouseLeftClickEvent) //quit button
				return false;
		}
		break;

	case Settings_scene:
		if (plusVolume->eventElement == MouseLeftClickEvent)
		{
			App->audio->volume += 2;
			Mix_VolumeMusic((int)(App->audio->volume * 1.28));
		}
		else if (minusVolume->eventElement == MouseLeftClickEvent)
		{
			App->audio->volume -= 2;
			Mix_VolumeMusic((int)(App->audio->volume * 1.28));
		}
		else if (crossButton->eventElement == MouseLeftClickEvent)
		{
			DeleteSettings();
			actualScene = Main_scene;
		}
		break;

	case Levels_scene:
		iPoint p = {(int)( 100 - App->render->camera.x / App->win->scale ),(int)( 100 - App->render->camera.y / App->win->scale) };
		pauseButton->SetLocalPosition(p);
		break;
	}
	//Menu options

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{ 
	//Check if player are dead or jumping , resolve bug player respawn and die for save and load
	if (App->entity_m->player != nullptr) {
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
		{
			if (App->map->level == level_1)
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
			if (App->map->level == level_1)
			{
				App->entity_m->player->needRespawn1 = true;
			}
			else
			{
				App->entity_m->player->needRespawn2 = true;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (App->entity_m->player->godMode == true)
				App->entity_m->player->godMode = false;
			else
				App->entity_m->player->godMode = true;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		//fadecommit esto no esta bien, funciona pero esta mal
		if (App->map->level == level_2 || App->map->level == start_screen) {
			App->fade->FadeToBlack("level1ND.tmx", 2.0);
			App->map->level = level_1;
		}
		else if (App->map->level == level_1) {
			App->fade->FadeToBlack("level2ND.tmx", 2.0);
			App->map->level = level_2;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN  && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
	{
		App->LoadGame();
	}

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN  && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
		App->SaveGame();

	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->cap == 30)
			App->cap = 60;
		else
			App->cap=30;
		App->capped_ms = 1000 / App->cap;
	}

	App->map->Draw();
	//Volume Control
	

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

void j1Scene::onUiTriggered(UIElement* UIelement, EventElement EventElement)
{
	if (UIelement->type == ButtonElement)
	{
		UIButton* b = (UIButton*)UIelement;
		switch (EventElement)
		{
		case NoEventElement:
			UIelement->rectUi = b->default_texture_rect;
			break;
		case MouseEnterEvent:
			UIelement->rectUi = b->mouse_on_rect;
			break;
		case MouseLeaveEvent:
			UIelement->rectUi = b->default_texture_rect;
			break;
		case MouseRightClickEvent:
			UIelement->rectUi = b->clicked_rect;
			break;
		case MouseLeftClickEvent:
			UIelement->rectUi = b->clicked_rect;
			break;
		case FocusEventElement:
			UIelement->rectUi = b->mouse_on_rect;
			break;
		}
	}
}

void j1Scene::CreateMainScene()
{
	const SDL_Texture* background = App->tex->Load("maps/Background.png");

	App->gui->CreateImage({ 0,0 }, { 0,0,1200,800 }, background, this, false);
	buttons.add(playButton = App->gui->CreateButton({ 100,600 }, { 276,0,138,142 }, { 138,0,138,142 }, { 0,0,138,142 }, App->gui->GetAtlas(), this, true));
	buttons.add(settingsButton = App->gui->CreateButton({ 800,0 }, { 276,284,138,142 }, { 138,284,138,142 }, { 0,284,138,142 }, App->gui->GetAtlas(), this, true));
	buttons.add(quitButton = App->gui->CreateButton({ 800,600 }, { 276,142,138,142 }, { 138,142,138,142 }, { 0,142,138,142 }, App->gui->GetAtlas(), this, true));
}

void j1Scene::CreateSettingsScene()
{
	settingsImage = App->gui->CreateImage({ -500,100 }, { 0,426,414,426 }, App->gui->GetAtlas(), this, true);
	settingsImage->SetParent(settingsButton);
	minusVolume = App->gui->CreateButton({ 50,100 }, { 138,1350,69,70 }, { 69,1350,69,70 }, { 0,1350,69,70 }, App->gui->GetAtlas(), this, false);
	plusVolume = App->gui->CreateButton({ 250,100 }, { 138,1420,69,70 }, { 69,1420,69,70 }, { 0,1420,69,70 }, App->gui->GetAtlas(), this, false);
	minusVolume->SetParent(settingsImage);
	plusVolume->SetParent(settingsImage);
	crossButton = App->gui->CreateButton({ 330,10 }, { 345,1350,69,70 }, { 276,1350,69,70 }, { 207,1350,69,70 }, App->gui->GetAtlas(), this, false);
	crossButton->SetParent(settingsImage);
}

void j1Scene::DeleteSettings()
{
	settingsImage->toDelete = true;
	minusVolume->toDelete = true;
	plusVolume->toDelete = true;
	crossButton->toDelete = true;
}

void j1Scene::CreateLevelScene()
{
	pauseButton = App->gui->CreateButton({App->render->camera.x + App->render->camera.w/2, 0}, { 0,852,138,142 }, { 0,852,138,142 }, { 0,852,138,142 }, App->gui->GetAtlas(),this,false);

}