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
#include "j1ScoreBoard.h"
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
	if (App->level != none)
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
		if ((App->level == level_1 || App->level == level_2))
			CreateLevelScene();
		timer.Start();

		
	}

	soundClick = App->audio->LoadFx("audio/click.wav");
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
	
			UpdateGUI();
		}
	}
	else
	{
		//test
		DeleteLevelUI();
		//level_pauseButton->toDelete = true;
		//DeletePauseMenu();
		App->fade->FadeToBlack(this, App->mainMenu, start_screen, 2.0f);
		//CreateMainScene();
		pauseMenu = false;
		App->entity_m->Disable();
		App->scoreBoard->coinCount = 0;
		App->scoreBoard->score = 0;
		App->scoreBoard->coinforLife = 0;
	}
			return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{ 
	//SCORE Record and restore life
	if (App->scoreBoard->scoreRecord < App->scoreBoard->score)
	{
		App->scoreBoard->scoreRecord = App->scoreBoard->score;
	}

	if (App->scoreBoard->coinforLife > 10 && App->entity_m->player_life < 3)
	{
		App->entity_m->player_life++;
		App->scoreBoard->coinforLife = 0;
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
				App->audio->PlayFx(soundClick, 0);
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
				App->scoreBoard->coinCount = 0;
				App->scoreBoard->score = 0;
				App->scoreBoard->coinforLife = 0;
				App->audio->PlayFx(soundClick, 0);
			}
			else if (pause_settingsButton->eventElement == MouseLeftClickEvent)
			{
				CreateSettingsScene();
				settingsIsOpen = true;
				App->audio->PlayFx(soundClick, 0);
			}
			else if (pause_returnButton->eventElement == MouseLeftClickEvent)
			{
				DeleteLevelUI();
				//level_pauseButton->toDelete = true;
				DeletePauseMenu();
				App->fade->FadeToBlack(this, App->mainMenu, start_screen, 2.0f);
				//CreateMainScene();
				pauseMenu = false;
				App->scoreBoard->coinCount = 0;
				App->scoreBoard->score = 0;
				App->scoreBoard->coinforLife = 0;
				App->SaveGame();
				App->audio->PlayFx(soundClick, 0);
			}
		}
	}
	else
	{
		if (settingsmm_volumeslider->eventElement == MouseLeftClickEvent)
		{
			App->audio->volume = settingsmm_volumeslider->getValue() * 100;
			Mix_VolumeMusic((int)(App->audio->volume * 1.28));
		}

		 if (settingsmm_crossButton->eventElement == MouseLeftClickEvent)
		{
			settingsIsOpen = false;
			DeleteSettings();
			//ERROR,BUG need think

			if (App->pause == true)
				App->level = level_1;
			else
				App->level = start_screen;

			App->level = start_screen;
			App->audio->PlayFx(soundClick, 0);
		}
		 UpdateVolumeLabel();
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
			App->fade->FadeToBlack(this, App->scoreBoard, score_board, 2.0);
			App->entity_m->Disable();
			DeleteLevelUI();
			App->scoreBoard->timerlvl2 = (int)timer.ReadSec();
		}
		else if (App->level == level_1) {
			App->fade->FadeToBlack(this,this,level_2, 2.0);
			App->scoreBoard->timerlvl1 = (int)timer.ReadSec();
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
	settingsmm_settingsImage = App->gui->CreateImage({ 100,100 }, { 0,486,363,429 }, App->gui->GetAtlas(), this, false);

	settingsmm_volumeLabel = App->gui->CreateLabel({ 50,100 }, "Volume:", { 0,0,0 }, App->font->default, this, false);
	settingsmm_volumeLabel->SetParent(settingsmm_settingsImage);

	std::string s = std::to_string((int)App->audio->volume);
	p2SString s2 = s.c_str();
	settingsmm_volumeNumber = App->gui->CreateLabel({ 70,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default, this, false);
	settingsmm_volumeNumber->SetParent(settingsmm_volumeLabel);
	settingsmm_volumeslider = App->gui->CreateSlider({ 50,200 }, { 0,916,219,19 }, { 221,917,28,30 }, App->gui->GetAtlas(), this, App->audio->volume / 128);
	settingsmm_volumeslider->SetParent(settingsmm_settingsImage);
	settingsmm_crossButton = App->gui->CreateButton({ 270,10 }, { 407,883,81,82 }, { 407,798,81,82 }, { 407,713,81,82 }, App->gui->GetAtlas(), this, false);
	settingsmm_crossButton->SetParent(settingsmm_settingsImage);
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
	std::string s = std::to_string(App->scoreBoard->coinCount);
	p2SString s2 = s.c_str();
	level_coinNumber = App->gui->CreateLabel({ 385,30 },(char*) s2.GetString(), { 0,0,0 }, App->font->default, this, false);

	level_pauseButton = App->gui->CreateButton({ 450,10 }, { 489,883,81,82 }, { 489,798,81,82 }, { 489,713,81,82 }, App->gui->GetAtlas(), this, false);
	level_scoreLabel = App->gui->CreateLabel({ 960,10 }, "000000", { 0,0,0 }, App->font->default, this, false);
	level_scoreText = App->gui->CreateLabel({ 900,10 }, "score:", { 0,0,0 }, App->font->default, this, false);
	level_time = App->gui->CreateLabel({ 650,10 }, "000", { 0,0,0 }, App->font->default, this, false);
	level_text = App->gui->CreateLabel({ 600,10 }, "time:", { 0,0,0 }, App->font->default, this, false);
	level_coinIcon = App->gui->CreateImage({ 325,10 }, { 430,570,53,53 }, App->gui->GetAtlas(), this, false);

	switch (App->entity_m->player_life)
	{
	case 3:
		life1 = App->gui->CreateImage({ 0,10 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2 = App->gui->CreateImage({ 76,10 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3 = App->gui->CreateImage({ 152 ,10 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
	case 2:
		life1 = App->gui->CreateImage({ 0,10 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2 = App->gui->CreateImage({ 76,10 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3 = App->gui->CreateImage({ 152 ,10 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
	case 1:
		life1 = App->gui->CreateImage({ 0,10 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2 = App->gui->CreateImage({ 76,10 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3 = App->gui->CreateImage({ 152 ,10 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
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

	switch (App->entity_m->player_life)
	{
	case 3:
		life1->ChangeTextureRect({ 493,484,76,75 });// = App->gui->CreateImage({ 76,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2->ChangeTextureRect({ 493,484,76,75 }); //= App->gui->CreateImage({ 152,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3->ChangeTextureRect({ 493,484,76,75 });// = App->gui->CreateImage({ 228 ,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
	case 2:
		life1->ChangeTextureRect({ 493,484,76,75 });// = App->gui->CreateImage({ 76,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2->ChangeTextureRect({ 493,484,76,75 });// = App->gui->CreateImage({ 152,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3->ChangeTextureRect({ 417,484,76,75 });// = App->gui->CreateImage({ 228 ,50 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
	case 1:
		life1->ChangeTextureRect({ 493,484,76,75 });// = App->gui->CreateImage({ 76,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2->ChangeTextureRect({ 417,484,76,75 });// = App->gui->CreateImage({ 152,50 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3->ChangeTextureRect({ 417,484,76,75 });// = App->gui->CreateImage({ 228 ,50 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
	case 0:
		life1->ChangeTextureRect({ 417,484,76,75 });// = App->gui->CreateImage({ 76,50 }, { 493,484,76,75 }, App->gui->GetAtlas(), this, false);
		life2->ChangeTextureRect({ 417,484,76,75 });// = App->gui->CreateImage({ 152,50 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		life3->ChangeTextureRect({ 417,484,76,75 });// = App->gui->CreateImage({ 228 ,50 }, { 417,484,76,75 }, App->gui->GetAtlas(), this, false);
		break;
	}
}


void j1Scene::DeleteSettings()
{
	settingsmm_settingsImage->toDelete = true;
	settingsmm_volumeLabel->toDelete = true;
	settingsmm_volumeNumber->toDelete = true;
	settingsmm_crossButton->toDelete = true;
	settingsmm_volumeslider->toDelete = true;
}


bool j1Scene::Save(pugi::xml_node& config) const
{
	pugi::xml_node scene_node = config.append_child("score");
	scene_node.append_attribute("score") = App->scoreBoard->score;
	scene_node.append_attribute("scoreRecord") = App->scoreBoard->scoreRecord;

	if (App->level == level_1)
		scene_node.append_attribute("timelvl1") = App->scoreBoard->timerlvl1;
	else if(App->level == level_2)
		scene_node.append_attribute("timelvl2") = App->scoreBoard->timerlvl2;

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	App->scoreBoard->score = data.child("score").attribute("score").as_int();
	App->scoreBoard->scoreRecord = data.child("score").attribute("scoreRecord").as_int();

	if (App->level == level_1)
		App->scoreBoard->timerlvl1 = data.child("score").attribute("timelvl1").as_int();
	else if (App->level == level_2)
		App->scoreBoard->timerlvl2 = data.child("score").attribute("timelvl2").as_int();

	return true;
}

void j1Scene::UpdateGUI() {

	std::string s = std::to_string(App->scoreBoard->score);
	p2SString s2 = s.c_str();
	App->font->CalcSize(s2.GetString(), level_scoreLabel->rectUi.w, level_scoreLabel->rectUi.h, App->font->default);
	level_scoreLabel->ChangeTexture(App->font->Print(s2.GetString(), { 0,0,0 }, App->font->default, level_scoreLabel->rectUi.w));

	std::string s3 = std::to_string(App->scoreBoard->coinCount);
	p2SString s4 = s3.c_str();
	App->font->CalcSize(s4.GetString(), level_coinNumber->rectUi.w, level_coinNumber->rectUi.h, App->font->default);
	level_coinNumber->ChangeTexture(App->font->Print(s4.GetString(), { 0,0,0 }, App->font->default, level_coinNumber->rectUi.w));

	std::string s5 = std::to_string((int)timer.ReadSec());
	p2SString s6 = s5.c_str();
	App->font->CalcSize(s6.GetString(), level_time->rectUi.w, level_time->rectUi.h, App->font->default);
	level_time->ChangeTexture(App->font->Print(s6.GetString(), { 0,0,0 }, App->font->default, level_time->rectUi.w));

	if (settingsIsOpen)
	{
		std::string s7 = std::to_string((int)timer.ReadSec());
		p2SString s8 = s7.c_str();
		App->font->CalcSize(s8.GetString(), level_time->rectUi.w, level_time->rectUi.h, App->font->default);
		level_time->ChangeTexture(App->font->Print(s8.GetString(), { 0,0,0 }, App->font->default, level_time->rectUi.w));
	}
	
	
	SetLife(App->entity_m->player_life);
}

void j1Scene::DeleteLevelUI()
{
	level_pauseButton->toDelete = true;
	level_scoreLabel->toDelete = true;
	level_coinNumber->toDelete = true;
	level_time->toDelete = true;
	level_coinIcon->toDelete = true;
	life1->toDelete = true;
	life2->toDelete = true;
	life3->toDelete = true;
	level_scoreText->toDelete = true;
	level_text->toDelete = true;
}

void j1Scene::UpdateVolumeLabel()
{
	int w, h;
	std::string s = std::to_string(App->audio->volume);
	p2SString s2 = s.c_str();
	App->font->CalcSize(s2.GetString(), w, h, App->font->default);
	settingsmm_volumeNumber->ChangeTexture(App->font->Print(s2.GetString(), { 0,0,0 }, App->font->default));
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