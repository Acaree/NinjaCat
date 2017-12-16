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
#include "j1MainMenu.h"
#include "j1Gui.h"
#include <stdio.h>

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
	App->map->Enable();
	App->entity_m->Enable();
	App->pathfinding->Enable();
	bool ret = true;
	map = config.child("map").attribute("name").as_string();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
		
	if (App->level == level_1) {
		App->map->Load("level1ND.tmx");
	}

	else if (App->level == level_2) {
		App->map->Load("level2ND.tmx");
	}

	if (App->map != nullptr) {
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);
		App->map->CreateEnemies();
		App->gui->DeleteUIElements();
	}
	if (App->entity_m->player != nullptr) {
		if (App->level == level_1)
		{
			App->entity_m->player->needRespawn1 = true;
		}
		else if (App->level == level_2)
		{
			App->entity_m->player->needRespawn2 = true;
		}
		App->entity_m->player->Respawn();
		App->entity_m->player->animation = &App->entity_m->player->idleRight;
	}
	CreateLevelScene();
	return true;
}


bool j1Scene::PreUpdate()
{
	if (App->entity_m->player_life > 0)
	{
		if (pauseMenu == false) {
			App->pause = false;
		}
		if (level_pauseButton->eventElement == MouseLeftClickEvent && pauseMenu == false)
		{
			App->pause = true;
			CreatePauseMenu();
			pauseMenu = true;
		}
		if (App->entity_m->player != nullptr) {
			std::string m = std::to_string(score);
			char* m2 = (char *)alloca(m.size() + 1);
			memcpy(m2, m.c_str(), m.size() + 1);
			level_scoreLabel->ChangeTexture(App->font->Print(m2, { 0,0,0 }, App->font->default));
			SetLife(App->entity_m->player_life);
		}
	}
	else
	{
		level_pauseButton->toDelete = true;
		//DeletePauseMenu();
		App->fade->FadeToBlack(this, App->mainMenu, start_screen, 2.0f);
		//CreateMainScene();
		pauseMenu = false;
		App->entity_m->Disable();
	}
			return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{ 
	//SCORE Record and restore life
	if (scoreRecord < score)
	{
		scoreRecord = score;
		if (coinCount > 10 && App->entity_m->player_life < 3)
		{
			App->entity_m->player_life++;
			coinCount = 0;
		}
	}
	if (settingsIsOpen == false)
	{
		if (pauseMenu == true) {
			if (pause_playButton->eventElement == MouseLeftClickEvent || pause_crossButton->eventElement == MouseLeftClickEvent)
			{
				if (App->pause == true)
					App->pause = false;
				DeletePauseMenu();
				pauseMenu = false;
			}

			if (pause_replayButton->eventElement == MouseLeftClickEvent)
			{
				if (App->level = level_1) {
					App->entity_m->player->needRespawn1 = true;
				}
				else {
					App->entity_m->player->needRespawn2 = true;
				}
				DeletePauseMenu();
				App->pause = false;
				pauseMenu = false;
			}
			else if (pause_settingsButton->eventElement == MouseLeftClickEvent)
			{
				CreateSettingsScene();
				settingsIsOpen = true;

			}
			else if (pause_returnButton->eventElement == MouseLeftClickEvent)
			{
				level_pauseButton->toDelete = true;
				DeletePauseMenu();
				App->fade->FadeToBlack(this, App->mainMenu, start_screen, 2.0f);
				//CreateMainScene();
				pauseMenu = false;
			}
		}
	}
	else
	{
		if (settingsmm_plusVolume->eventElement == MouseLeftClickEvent)
		{
			if (App->audio->volume < 100)
				App->audio->volume += 10;
			Mix_VolumeMusic((int)(App->audio->volume * 1.28));
		}
		else if (settingsmm_minusVolume->eventElement == MouseLeftClickEvent)
		{
			if (App->audio->volume > 0)
				App->audio->volume -= 10;
			Mix_VolumeMusic((int)(App->audio->volume * 1.28));
		}
		else if (settingsmm_crossButton->eventElement == MouseLeftClickEvent)
		{
			settingsIsOpen = false;
			DeleteSettings();
			//ERROR,BUG need think

			if (App->pause == true)
				App->level = level_1;
			else
				App->level = start_screen;

			App->level = start_screen;
		}
		std::string s = std::to_string(App->audio->volume);
		char* s2 = (char *)alloca(s.size() + 1);
		memcpy(s2, s.c_str(), s.size() + 1);
		settingsmm_volumeLabel->ChangeTexture(App->font->Print(s2, { 0,0,0 }, App->font->default));

	}
	
	//Check if player are dead or jumping , resolve bug player respawn and die for save and load
	if (App->entity_m->player != nullptr) {
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
		{
			if (App->level == level_1)
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
			if (App->level == level_1)
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
		if (App->level == level_2) {
			//fade to score, main menu or level 1. to decide
		}
		else if (App->level == level_1) {
			App->fade->FadeToBlack(this,this,level_2, 2.0);
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
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	
	bool ret = true;
	Mix_VolumeMusic((int)(App->audio->volume * 1.28));
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->CleanUp();
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


void j1Scene::CreateSettingsScene()
{
	settingsmm_settingsImage = App->gui->CreateImage({ 100,100 }, { 0,486,363,429 }, App->gui->GetAtlas(), this, true);

	settingsmm_minusVolume = App->gui->CreateButton({ 50,100 }, { 138,1350,69,70 }, { 69,1350,69,70 }, { 0,1350,69,70 }, App->gui->GetAtlas(), this, false);
	settingsmm_plusVolume = App->gui->CreateButton({ 250,100 }, { 138,1420,69,70 }, { 69,1420,69,70 }, { 0,1420,69,70 }, App->gui->GetAtlas(), this, false);
	settingsmm_minusVolume->SetParent(settingsmm_settingsImage);
	settingsmm_plusVolume->SetParent(settingsmm_settingsImage);
	settingsmm_crossButton = App->gui->CreateButton({ 270,10 }, { 407,883,81,82 }, { 407,798,81,82 }, { 407,713,81,82 }, App->gui->GetAtlas(), this, false);
	settingsmm_crossButton->SetParent(settingsmm_settingsImage);
	settingsmm_volumeLabel = App->gui->CreateLabel({ 150,100 }, "100", { 0,0,0 }, App->font->default, this, false);
	settingsmm_volumeLabel->SetParent(settingsmm_settingsImage);
}


/*void j1Scene::DeleteSettings()
{
	settingsmm_settingsImage->toDelete = true;
	settingsmm_minusVolume->toDelete = true;
	settingsmm_plusVolume->toDelete = true;
	settingsmm_crossButton->toDelete = true;
	settingsmm_volumeLabel->toDelete = true;
}*/

void j1Scene::CreateLevelScene()
{
	level_pauseButton = App->gui->CreateButton({ App->render->camera.w / 2, 5 }, { 489,883,81,82 }, { 489,798,81,82 }, { 489,713,81,82 }, App->gui->GetAtlas(), this, false);
	level_scoreLabel = App->gui->CreateLabel({ 30,20 }, "000000", { 0,0,0 }, App->font->default, this, false);

	if (App->entity_m->player != nullptr)
	{
		if (level_lifesImage[0] == nullptr) {
			for (int i = 0; i < App->entity_m->player_life; i++)
			{
				level_lifesImage[i] = App->gui->CreateImage({ 76 * i,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, true);
			}

			for (int i = 3; i > App->entity_m->player_life; i--) {
				level_lifesImage[i] = App->gui->CreateImage({ 76 * (i - 1),50 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, true);
			}
		}
	}
}



void j1Scene::CreatePauseMenu()
{
	pause_settingsImage = App->gui->CreateImage({ 0,100 }, { 0,486,363,429 }, App->gui->GetAtlas(), this, true);
	pause_settingsImage->SetParent(level_pauseButton);
	pause_crossButton = App->gui->CreateButton({ 270,10 }, { 407,883,81,82 }, { 407,798,81,82 },  { 407, 713, 81, 82 }, App->gui->GetAtlas(), this, false);
	pause_crossButton->SetParent(pause_settingsImage);

	pause_playButton = App->gui->CreateButton({ 50,30 }, { 380,276,190,69 }, { 190,276,190,69 }, { 0,276,190,69 }, App->gui->GetAtlas(), this, false);
	pause_replayButton = App->gui->CreateButton({50,130 }, { 380,345,190,69 }, { 190,345,190,69 }, { 0,345,190,69 }, App->gui->GetAtlas(), this, false);
	pause_settingsButton = App->gui->CreateButton({ 50,230 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, false);
	pause_returnButton = App->gui->CreateButton({50,330 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, false);

	pause_playButton->SetParent(pause_settingsImage);
	pause_replayButton->SetParent(pause_settingsImage);
	pause_settingsButton->SetParent(pause_settingsImage);
	pause_returnButton->SetParent(pause_settingsImage);
}

void j1Scene::DeletePauseMenu()
{
	pause_settingsImage->toDelete = true;
	pause_crossButton->toDelete = true;
	pause_playButton->toDelete = true;
	pause_replayButton->toDelete = true;
	pause_settingsButton->toDelete = true;
	pause_returnButton->toDelete = true;
}

void j1Scene::SetLife(uint life)
{

	if (level_lifesImage[0] != nullptr) {
		for (int i = 0; i < life; i++)
		{
			level_lifesImage[i]->ChangeTextureRect({ 493,484,76,75 });
		}

		for (int i = 3; i > life; i--) {
			level_lifesImage[i-1]->ChangeTextureRect({ 417,484,76,75 });
		}
	}
}


void j1Scene::DeleteSettings()
{
	settingsmm_settingsImage->toDelete = true;
	settingsmm_minusVolume->toDelete = true;
	settingsmm_plusVolume->toDelete = true;
	settingsmm_crossButton->toDelete = true;
	settingsmm_volumeLabel->toDelete = true;
}


bool j1Scene::Save(pugi::xml_node& config) const
{
	pugi::xml_node scene_node = config.append_child("score");
	scene_node.append_attribute("score") = App->scene->score;
	scene_node.append_attribute("scoreRecord") = App->scene->scoreRecord;
	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	score = data.child("score").attribute("score").as_int();
	scoreRecord = data.child("score").attribute("scoreRecord").as_int();
	return true;
}

//previous preupdate

/*
//change volume to a char* from int
//std::string s = std::to_string(App->audio->volume);
//char* s2 = (char *)alloca(s.size() + 1);
//memcpy(s2, s.c_str(), s.size() + 1);


switch (App->level)
{
case start_screen:
	/if (start_playButton->eventElement == MouseLeftClickEvent)
	{
	App->fade->FadeToBlack(level_1, 2.0);
	level = level_1;
	DeleteMainMenuSettings();
	buttons.clear();
	CreateLevelScene();

	}
	else if (start_settingsButton->eventElement == MouseLeftClickEvent)
	{
	CreateSettingsScene();
	level = settings_screen;
	}
	else if (start_quitButton->eventElement == MouseLeftClickEvent) //quit button
	return false;/
	break;

case settings_screen:
	/if (settingsmm_plusVolume->eventElement == MouseLeftClickEvent)
	{
	if (App->audio->volume < 100)
	App->audio->volume += 10;
	Mix_VolumeMusic((int)(App->audio->volume * 1.28));
	}
	else if (settingsmm_minusVolume->eventElement == MouseLeftClickEvent)
	{
	if (App->audio->volume > 0)
	App->audio->volume -= 10;
	Mix_VolumeMusic((int)(App->audio->volume * 1.28));
	}
	else if (settingsmm_crossButton->eventElement == MouseLeftClickEvent)
	{
	DeleteSettings();
	//ERROR,BUG need think

	if (App->pause == true)
	level = level_1;
	else
	level = start_screen;

	level = start_screen;
	}

	settingsmm_volumeLabel->ChangeTexture(App->font->Print(s2, { 0,0,0 }, App->font->default));/
	break;

case level_1:
	if (pauseMenu == false) {
		App->pause = false;
	}
	if (level_pauseButton->eventElement == MouseLeftClickEvent && pauseMenu == false)
	{
		if (App->pause == true)
			App->pause = false;
		else
		{
			App->pause = true;
			CreatePauseMenu();
			level = pause_screen;
		}
		pauseMenu = true;
	}
	if (App->entity_m->player != nullptr)
	{
		if (level_lifesImage[0] == nullptr) {
			for (int i = 0; i < App->entity_m->player_life; i++)
			{
				level_lifesImage[i] = App->gui->CreateImage({ 76 * i,50 }, { 537,0,76,75 }, App->gui->GetAtlas(), this, true);
			}

			for (int i = 3; i > App->entity_m->player_life; i--) {
				level_lifesImage[i] = App->gui->CreateImage({ 76 * (i - 1),50 }, { 460,0,76,75 }, App->gui->GetAtlas(), this, true);
			}
		}
		std::string m = std::to_string(App->entity_m->player->score);
		char* m2 = (char *)alloca(m.size() + 1);
		memcpy(m2, m.c_str(), m.size() + 1);
		level_scoreLabel->ChangeTexture(App->font->Print(m2, { 0,0,0 }, App->font->default));
		SetLife(App->entity_m->player_life);
	}

	break;
case pause_screen:
	//need new scene or restructured
	if (pause_playButton != nullptr)
	{
		if (pause_playButton->eventElement == MouseLeftClickEvent || pause_crossButton->eventElement == MouseLeftClickEvent)
		{
			if (App->pause == true)
				App->pause = false;
			DeletePauseMenu();
			pauseMenu = false;
			level = level_1;
		}
		else if (level_pauseButton->eventElement != MouseLeftClickEvent)
			pauseMenu = false;

		if (pause_replayButton->eventElement == MouseLeftClickEvent)
		{
			App->entity_m->player->needRespawn1 = true;
			DeletePauseMenu();
			App->pause = false;
			level = level_1;
		}
		else if (pause_settingsButton->eventElement == MouseLeftClickEvent)
		{
			//CreateSettingsScene();
			level = settings_screen;
		}
		else if (pause_returnButton->eventElement == MouseLeftClickEvent)
		{
			level_pauseButton->toDelete = true;
			DeletePauseMenu();
			level = start_screen;
			App->fade->FadeToBlack(level, 2.0f);
			//CreateMainScene();
			pauseMenu = false;
		}

	}



	break;

case level_2:
	//	pauseButton->SetLocalPosition(p);
	break;
}

//Menu options

return true;*/