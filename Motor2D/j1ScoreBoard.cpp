#include "j1ScoreBoard.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1FadeToBlack.h"
#include "j1MainMenu.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Entities.h"

j1ScoreBoard::j1ScoreBoard() : j1Module()
{
	name.create("scoreBoard");
}

bool j1ScoreBoard::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1ScoreBoard::Start()
{
	if (enabled)
	{
		App->audio->PlayMusic("audio/intro.ogg");

		CreateUI();
	}
	return true;
}

bool j1ScoreBoard::PreUpdate()
{
	if (enabled) {
		if (!animationsFinish)
			animationsFinish = AnimationsScence();
		else
		{
			if (score_backToMenu->eventElement == MouseLeftClickEvent)
			{
				App->fade->FadeToBlack(this, App->mainMenu, start_screen, 0.5f);
				DeleteUI();
			}
		}
	}
	return true;
}

bool j1ScoreBoard::Update(float dt)
{

	return true;
}

bool j1ScoreBoard::PostUpdate()
{
	return true;
}

bool j1ScoreBoard::CleanUp()
{
	DeleteUI();
	return true;
}

bool j1ScoreBoard::Load(pugi::xml_node& node)
{
	return true;
}

bool j1ScoreBoard::Save(pugi::xml_node& node) const
{
	return true;
}

void j1ScoreBoard::onUiTriggered(UIElement* UIelement, EventElement EventElement)
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



bool j1ScoreBoard::AnimationsScence()
{
	bool ret = true;

	return ret;
}

void j1ScoreBoard::DeleteUI(){
	App->tex->UnLoad(background);
	if (score_mainImage != nullptr)
	{
		score_mainImage->toDelete = true;
		score_score->toDelete = true;
		score_backToMenu->toDelete = true;
	}
}

void j1ScoreBoard::CreateUI(){
	
	background = App->tex->Load("maps/Background.png");
	score_mainImage = App->gui->CreateImage({ 0,0 }, { 0,0,1200,800 }, background, this, false);
	score_score = App->gui->CreateLabel({ 500,200 }, "aqui va la score jeje", { 0,0,0 }, App->font->default, this, false);
	score_backToMenu = App->gui->CreateButton({ 500,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, true);
}
