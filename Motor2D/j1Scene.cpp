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
#include "j1FadeToBlack.h"
#include "mPlayer.h"
#include "j1Pathfinding.h"
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
	if (App->map->Load(map.GetString()) == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	App->map->CreateEnemies();


	const SDL_Texture* atlas = App->tex->Load("gui/atlas.png");
	UIButton* a, *b, *c;
	buttons.add(a = App->gui->CreateButton({256,0 }, { 0,113,229,69 }, { 411,169,229,69 }, { 642,169,229,69 }, atlas, this, true));
	buttons.add(b = App->gui->CreateButton({ 0,100 }, { 0,113,229,69 }, { 411,169,229,69 }, { 642,169,229,69 }, atlas, this, true));
	buttons.add(c = App->gui->CreateButton({ 0,100 }, { 0,113,229,69 }, { 411,169,229,69 }, { 642,169,229,69 }, atlas, this, true));
	//set parent
	b->SetParent(a);
	c->SetParent(b);
	UILabel* d;
	d = App->gui->CreateLabel({ -100,50 }, "hello world", { 226,186,31,255 }, App->font->default, this, false);
	d->SetParent(a);

	

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
	if (buttons[0]->eventElement == MouseLeftClickEvent) {
		App->fade->FadeToBlack("level1ND.tmx",2.0);
		


	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->entity_m->player->animation != &App->entity_m->player->dead && App->entity_m->player->jumping == false)
	{
		if (App->map->isLevel1 == true)
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
		if (App->map->isLevel1 == true)
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
		if (App->cap == 30)
			App->cap = 60;
		else
			App->cap=30;
		App->capped_ms = 1000 / App->cap;
	}

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		bool focusButton = false;
		for (uint i = 0; i < buttons.count(); i++)
		{
			if (buttons[i]->eventElement == FocusEventElement)
			{
				focusButton = true;
				buttons[i]->eventElement = NoEventElement;
				//onUiTriggered(buttons[i], NoEventElement);
				if (i + 1 != buttons.count())
					buttons[i + 1]->eventElement = FocusEventElement;
				else
					buttons[0]->eventElement = FocusEventElement;
				break;
			}

		}
		if (!focusButton && buttons.count() > 0) // 0 button focus && are buttons in the scene
		{
			buttons[0]->eventElement = FocusEventElement;
		}

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