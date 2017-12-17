#include "j1MainMenu.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1ScoreBoard.h"
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
	//background = App->tex->Load("maps/Background.png");
	App->audio->PlayMusic("audio/intro.ogg");
	//Main Scene
	CreateMainScene();
	font_credits = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf", 20);
	settingsIsOpen = false;
	animationsFinish = false;
	creditsIsOpen = false;

	soundClick = App->audio->LoadFx("audio/click.wav");
	return true;
}

bool j1MainMenu::PreUpdate()
{
	if(!animationsFinish)
	animationsFinish=AnimationsScence();
	else
	{
		if (!settingsIsOpen && !creditsIsOpen)
		{
			if(start_playButton->eventElement == MouseLeftClickEvent)
			{
				App->fade->FadeToBlack(this, App->scene, level_1, 2.0);
				App->level = level_1;
				DeleteMainMenuSettings();
				App->scene->buttons.clear();
				//App->scene->CreateLevelScene();
				App->entity_m->Enable();
				App->audio->PlayMusic("audio/level.ogg");
				if (App->entity_m->player != nullptr)
					App->entity_m->player_life = 3;
				App->audio->PlayFx(soundClick, 0);

			}
			else if (start_continueButton->eventElement == MouseLeftClickEvent)
			{
				App->LoadGame();
				App->audio->PlayFx(soundClick, 0);
			}
			else if (start_settingsButton->eventElement == MouseLeftClickEvent)
			{
				CreateSettingsScene();
				settingsIsOpen = true;
				App->audio->PlayFx(soundClick, 0);
			}
			else if (start_quitButton->eventElement == MouseLeftClickEvent)
			{//quit button
				return false;
				App->audio->PlayFx(soundClick, 0);
			}
			else if (start_creditsButton->eventElement == MouseLeftClickEvent)
			{
				DeleteMainMenuSettings();
				creditsIsOpen = true;
				CreateCredits();
				App->audio->PlayFx(soundClick, 0);
			}
		}
		else if(settingsIsOpen)
		{
			if (settingsmm_volumeslider->eventElement == MouseLeftClickEvent)
			{
				App->audio->volume = settingsmm_volumeslider->getValue() * 100;
				Mix_VolumeMusic((int)(App->audio->volume * 1.28));
				App->audio->PlayFx(soundClick, 0);
			}

			else if (settingsmm_crossButton->eventElement == MouseLeftClickEvent)
			{
				App->audio->PlayFx(soundClick, 0);
				settingsIsOpen = false;
				DeleteSettings();
				//ERROR,BUG need think

				if (App->pause == true)
					App->level = level_1;
				else
					App->level = start_screen;

				App->level = start_screen;
			}
			UpdateVolumeLabel();
		}
		else if (creditsIsOpen)
		{
			if (start_quitButton->eventElement == MouseLeftClickEvent)
			{
				App->audio->PlayFx(soundClick, 0);
				DeleteCredits();
				CreateMainScene();
				creditsIsOpen = false;
				animationsFinish = false;
			}
		}
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
	start_title->toDelete = true;
	start_creditsButton->toDelete = true;
}

void j1MainMenu::CreateMainScene()
{
	
	background = App->tex->Load("maps/Background.png");
	//Main Scene
	start_mainImage = App->gui->CreateImage({ 0,0 }, { 0,0,1200,800 }, background, this, false);
	start_title= App->gui->CreateImage({150,-295 }, { 643,13,909,295 }, App->gui->GetAtlas(), this, false);
	start_creditsButton = App->gui->CreateButton({1364,10}, { 380,414,190,69 }, { 190,414,190,69 }, { 0,414,190,69 },App->gui->GetAtlas(),this,false);
	pugi::xml_document config_file;
	if (pugi::xml_parse_result result = config_file.load_file("save_game.xml"))
	{
		start_playButton = App->gui->CreateButton({ -160,-70 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_continueButton = App->gui->CreateButton({ -160,-70 }, { 380,69,190,69 }, { 190,69,190,69 }, { 0,69,190,69 }, App->gui->GetAtlas(), this, false);
		start_settingsButton = App->gui->CreateButton({ -160,-70 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ -160,-70 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);
		
	}
	else
	{
		start_playButton = App->gui->CreateButton({ -160,-70 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_settingsButton = App->gui->CreateButton({ -160,-70 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ -160,-70 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);
	}

}

void j1MainMenu::CreateSettingsScene()
{
	settingsmm_settingsImage = App->gui->CreateImage({ 100,100 }, { 0,486,363,429 }, App->gui->GetAtlas(), this, false);

	settingsmm_volumeLabel = App->gui->CreateLabel({ 50,100 },"Volume:" ,{0,0,0},App->font->default,this,false);
	settingsmm_volumeLabel->SetParent(settingsmm_settingsImage);

	std::string s = std::to_string((int)App->audio->volume);
	p2SString s2 = s.c_str();
	settingsmm_volumeNumber= App->gui->CreateLabel({ 70,0 },(char*) s2.GetString(), { 0,0,0 }, App->font->default, this, false);
	settingsmm_volumeNumber->SetParent(settingsmm_volumeLabel);
	settingsmm_volumeslider = App->gui->CreateSlider({ 100,200 }, { 0,916,219,19 }, { 221,917,28,30 }, App->gui->GetAtlas(), this, App->audio->volume / 128);
	settingsmm_volumeslider->SetParent(settingsmm_settingsImage);
	settingsmm_crossButton = App->gui->CreateButton({ 270,10 }, { 407,883,81,82 } , { 407,798,81,82 }, { 407,713,81,82 }, App->gui->GetAtlas(), this, false);
	settingsmm_crossButton->SetParent(settingsmm_settingsImage);
}

void j1MainMenu::DeleteSettings()
{
	settingsmm_settingsImage->toDelete = true;
	settingsmm_volumeLabel->toDelete = true;
	settingsmm_volumeNumber->toDelete = true;
	settingsmm_crossButton->toDelete = true;
	settingsmm_volumeslider->toDelete = true;
}

bool j1MainMenu::AnimationsScence()
{
	bool ret = true;
	if (start_title->localPosition.y <= 0)
	{
		start_title->localPosition.y += 2;
		ret = false;
	}
	if(start_creditsButton->localPosition.x >= 800)
	start_creditsButton->localPosition.x -= 4;

	if (start_continueButton == nullptr)
	{
		if (start_playButton->localPosition.x <= 200)
			start_playButton->localPosition.x += 10;
		if(start_playButton->localPosition.y <= 600)
			start_playButton += 10;

		if (start_settingsButton->localPosition.x <= 400)
			start_settingsButton->localPosition.x += 10;
		if (start_settingsButton->localPosition.y != 600)
			start_settingsButton->localPosition.y <= 10;

		if (start_quitButton->localPosition.x <= 600)
		{
			start_quitButton->localPosition.x += 10;
			ret = false;
		}
		if (start_quitButton->localPosition.y <= 600)
		{
			start_quitButton->localPosition.y += 10;
			ret = false;
		}
	}
	else
	{
		if (start_playButton->localPosition.x <= 100)
			start_playButton->localPosition.x += 10;
		if (start_playButton->localPosition.y <= 600)
			start_playButton->localPosition.y += 10;

		if (start_continueButton->localPosition.x <= 300)
			start_continueButton->localPosition.x += 10;
		if (start_continueButton->localPosition.y <= 600)
			start_continueButton->localPosition.y += 10;

		if (start_settingsButton->localPosition.x <= 500)
			start_settingsButton->localPosition.x += 10;
		if (start_settingsButton->localPosition.y <= 600)
			start_settingsButton->localPosition.y += 10;

		if (start_quitButton->localPosition.x <= 700)
		{
			start_quitButton->localPosition.x += 10;
			ret = false;
		}
		if (start_quitButton->localPosition.y <= 600)
		{
			start_quitButton->localPosition.y += 10;
			ret = false;
		}
	}

	return ret;
}

void j1MainMenu::CreateCredits()
{
	char* authors = "AUTHORS: Alex Campamar and Alfonso Sanchez-Cortes\nACopyright JS Foundation and other contributors, https://js.foundation/\nThis software consists of voluntary contributions made by many individuals.\nFor exact contribution history, see the revision history available at https : \n//github.com/Acaree/NinjaCat.\nThe following license applies to all parts of this software except as documented below : \nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'), \nto deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, \nand/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : \nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\nIN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, \nWHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\nAll files located in the node_modules and external directories are externally maintained libraries used by this software which have their own licenses; \nwe recommend you read them, as their terms may differ from the terms above.\n ";
	credit_label = App->gui->CreateLabel({ 0,0 }, authors, {255,255,255},font_credits,this,false,1000);
	start_quitButton = App->gui->CreateButton({900,650}, { 407, 883, 81, 82 }, { 407,798,81,82 }, { 407,713,81,82 }, App->gui->GetAtlas(), this, false);
}

void j1MainMenu::DeleteCredits()
{
	start_quitButton->toDelete = true;
	credit_label->toDelete = true;
}

void j1MainMenu::UpdateVolumeLabel()
{
	int w, h;
	std::string s = std::to_string(App->audio->volume);
	p2SString s2 = s.c_str();
	App->font->CalcSize(s2.GetString(), w, h, App->font->default);
	settingsmm_volumeNumber->ChangeTexture(App->font->Print(s2.GetString(), { 0,0,0 }, App->font->default));
}
/*		start_playButton = App->gui->CreateButton({ 0,0 }, { 380,0,190,69 }, { 190,0,190,69 }, { 0,0,190,69 }, App->gui->GetAtlas(), this, true);
		start_continueButton = App->gui->CreateButton({ 300,600 }, { 380,69,190,69 }, { 190,69,190,69 }, { 0,69,190,69 }, App->gui->GetAtlas(), this, false);
		start_settingsButton = App->gui->CreateButton({ 500,600 }, { 380,138,190,69 }, { 190,138,190,69 }, { 0,138,190,69 }, App->gui->GetAtlas(), this, true);
		start_quitButton = App->gui->CreateButton({ 700,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);*/