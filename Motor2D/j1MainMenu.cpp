#include "j1MainMenu.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Entities.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("mainMenu");
}

bool j1MainMenu::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1MainMenu::Start()
{
	background = App->tex->Load("maps/Background.png");
	//Main Scene
	start_mainImage = App->gui->CreateImage({ 0,0 }, { 0,0,1200,800 }, background, this, false);

	pugi::xml_document config_file;
	if (pugi::xml_parse_result result = config_file.load_file("save_game.xml"))
	{
		start_playButton = App->gui->CreateButton({ 100,600 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_continueButton = App->gui->CreateButton({ 300,600 }, { 380,69,190,69 }, { 190,69,190,69 }, { 0,69,190,69 }, App->gui->GetAtlas(), this, false);
		start_settingsButton = App->gui->CreateButton({ 500,600 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ 700,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);

	}
	else
	{
		start_playButton = App->gui->CreateButton({ 200,600 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_settingsButton = App->gui->CreateButton({400,600 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ 600,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);
	}

	return true;
}

bool j1MainMenu::PreUpdate()
{
	if (!settingsIsOpen)
	{
		if (start_playButton->eventElement == MouseLeftClickEvent)
		{
			App->fade->FadeToBlack(this,App->scene,level_1, 2.0);
			App->level = level_1;
			DeleteMainMenuSettings();
			App->scene->buttons.clear();
			//App->scene->CreateLevelScene();
			App->entity_m->Enable();

		}
		else if (start_continueButton->eventElement == MouseLeftClickEvent)
		{
			App->LoadGame();
		}
		else if (start_settingsButton->eventElement == MouseLeftClickEvent)
		{
			CreateSettingsScene();
			settingsIsOpen = true;
		}
		else if (start_quitButton->eventElement == MouseLeftClickEvent) //quit button
			return false;
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
	return true;
}

bool j1MainMenu::Update(float dt)
{
	

	return true;
}

bool j1MainMenu::PostUpdate()
{
	return true;
}

bool j1MainMenu::CleanUp()
{
	return true;
}

bool j1MainMenu::Load(pugi::xml_node& node)
{
	return true;
}

bool j1MainMenu::Save(pugi::xml_node& node) const
{
	return true;
}

void j1MainMenu::onUiTriggered(UIElement* UIelement, EventElement EventElement)
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

void j1MainMenu::DeleteMainMenuSettings()
{
	App->tex->UnLoad(background);
	start_mainImage->toDelete = true;
	start_playButton->toDelete = true;
	start_settingsButton->toDelete = true;
	start_quitButton->toDelete = true;
	start_continueButton->toDelete = true;
}

void j1MainMenu::CreateMainScene()
{
	background = App->tex->Load("maps/Background.png");
	//Main Scene
	start_mainImage = App->gui->CreateImage({ 0,0 }, { 0,0,1200,800 }, background, this, false);

	pugi::xml_document config_file;
	if (pugi::xml_parse_result result = config_file.load_file("save_game.xml"))
	{
		start_playButton = App->gui->CreateButton({ 100,600 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_continueButton = App->gui->CreateButton({ 300,600 }, { 380,69,190,69 }, { 190,69,190,69 }, { 0,69,190,69 }, App->gui->GetAtlas(), this, false);
		start_settingsButton = App->gui->CreateButton({ 500,600 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ 700,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);

	}
	else
	{
		start_playButton = App->gui->CreateButton({ 200,600 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_settingsButton = App->gui->CreateButton({ 400,600 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ 600,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);
	}
}

void j1MainMenu::CreateSettingsScene()
{
	settingsmm_settingsImage = App->gui->CreateImage({ 100,100 }, { 0,486,363,429 }, App->gui->GetAtlas(), this, true);
	
	settingsmm_minusVolume = App->gui->CreateButton({ 50,100 }, { 138,1350,69,70 }, { 69,1350,69,70 }, { 0,1350,69,70 }, App->gui->GetAtlas(), this, false);
	settingsmm_plusVolume = App->gui->CreateButton({ 250,100 }, { 138,1420,69,70 }, { 69,1420,69,70 }, { 0,1420,69,70 }, App->gui->GetAtlas(), this, false);
	settingsmm_minusVolume->SetParent(settingsmm_settingsImage);
	settingsmm_plusVolume->SetParent(settingsmm_settingsImage);
	settingsmm_crossButton = App->gui->CreateButton({ 270,10 }, { 407,883,81,82 } , { 407,798,81,82 }, { 407,713,81,82 }, App->gui->GetAtlas(), this, false);
	settingsmm_crossButton->SetParent(settingsmm_settingsImage);
	settingsmm_volumeLabel = App->gui->CreateLabel({ 150,100 }, "100", { 0,0,0 }, App->font->default, this, false);
	settingsmm_volumeLabel->SetParent(settingsmm_settingsImage);
}

void j1MainMenu::DeleteSettings()
{
	settingsmm_settingsImage->toDelete = true;
	settingsmm_minusVolume->toDelete = true;
	settingsmm_plusVolume->toDelete = true;
	settingsmm_crossButton->toDelete = true;
	settingsmm_volumeLabel->toDelete = true;
}
