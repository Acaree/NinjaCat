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
		score_backToMenu->toDelete = true;
		score_mainImage->toDelete = true;
		score_timer->toDelete = true;
		score_textTimer->toDelete = true;
		score_coins->toDelete = true;
		score_textcoins->toDelete = true;
		score_level->toDelete = true;
		score_textlevel->toDelete = true;
		score_life->toDelete = true;
		score_textlife->toDelete = true;
		score_record->toDelete = true;
		score_textrecord->toDelete = true;
		score_total->toDelete = true;
		score_texttotal->toDelete = true;
	}
}

void j1ScoreBoard::CreateUI(){

	background = App->tex->Load("maps/Background.png");

	score_mainImage = App->gui->CreateImage({ 0,0 }, { 0,0,1200,800 }, background, this, false);
	
	std::string s = std::to_string(score);
	p2SString s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_level->rectUi.w, score_level->rectUi.h, App->font->default);
	score_textlevel= App->gui->CreateLabel({ 400,10 }, "Score Level:", { 0,0,0 }, App->font->default, this, false);
	score_level = App->gui->CreateLabel({90,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default,this,false);

	 timescore=timerlvl1 + timerlvl2;
	s = std::to_string(timescore);
	s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_timer->rectUi.w, score_timer->rectUi.h, App->font->default);
	score_textTimer = App->gui->CreateLabel({ 0,70 }, "Score Time:", { 0,0,0 }, App->font->default, this, false);
	score_timer = App->gui->CreateLabel({ 90,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default, this, false);

	s = std::to_string(App->entity_m->player_life);
	s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_life->rectUi.w, score_life->rectUi.h, App->font->default);
	score_textlife = App->gui->CreateLabel({ 0,70 }, "Score Lifes:", { 0,0,0 }, App->font->default, this, false);
	score_life = App->gui->CreateLabel({ 90,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default, this, false);

	s = std::to_string(coinCount);
	s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_coins->rectUi.w, score_coins->rectUi.h, App->font->default);
	score_textcoins = App->gui->CreateLabel({ 0,70 }, "Score Coints:", { 0,0,0 }, App->font->default, this, false);
	score_coins = App->gui->CreateLabel({ 90,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default, this, false);

	int totalscore = (int)(App->entity_m->player_life*(score - (App->scene->timer.ReadSec()/3) + 3 * coinCount));
	if (totalscore < 0)
		totalscore = 0;

	s = std::to_string(totalscore);
	s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_total->rectUi.w, score_total->rectUi.h, App->font->default);
	score_texttotal = App->gui->CreateLabel({ 0,70 }, "Total:", { 0,0,0 }, App->font->default, this, false);
	score_total = App->gui->CreateLabel({ 90,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default, this, false);

	if (scoreRecord < totalscore)
		scoreRecord = totalscore;

	s = std::to_string(scoreRecord);
	s2 = s.c_str();
	//App->font->CalcSize(s2.GetString(), score_textrecord->rectUi.w, score_textrecord->rectUi.h, App->font->default);
	score_textrecord = App->gui->CreateLabel({ 0,70 }, "Record:", { 0,0,0 }, App->font->default, this, false);
	score_record = App->gui->CreateLabel({ 90,0 }, (char*)s2.GetString(), { 0,0,0 }, App->font->default, this, false);

	score_textlevel->SetParent(score_mainImage);
	score_level->SetParent(score_textlevel);
	score_textTimer->SetParent(score_textlevel);
	score_timer->SetParent(score_textTimer);
	score_textlife->SetParent(score_textTimer);
	score_life->SetParent(score_textlife);
	score_textcoins->SetParent(score_textlife);
	score_coins->SetParent(score_textcoins);
	score_texttotal->SetParent(score_textcoins);
	score_total->SetParent(score_texttotal);
	
	score_textrecord->SetParent(score_texttotal);
	score_record->SetParent(score_textrecord);

	score_backToMenu = App->gui->CreateButton({ 500,600 }, { 380,207,190,69 }, { 190,207,190,69 }, { 0,207,190,69 }, App->gui->GetAtlas(), this, false);
}
