#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"
#include "j1Timer.h"
#include "SDL/include/SDL_render.h"
#include "j1Gui.h"


j1FadeToBlack::j1FadeToBlack()
{
	
	name.create("fade");
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	screen = { 0,0, (int) (App->win->width*App->win->scale), (int) (App->win->height*App->win->scale) };
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;


			App->map->CleanUp();

			if (App->map->Load(map_on) == true)
			{
				int w, h;
				uchar* data = NULL;
				if (App->map->CreateWalkabilityMap(w, h, &data))
					App->pathfinding->SetMap(w, h, data);
				RELEASE_ARRAY(data);
				App->map->CreateEnemies();
				App->gui->DeleteUIElements();
			}

			if (App->entity_m->player != nullptr) {
				App->entity_m->player->Respawn();

				if (App->map->level == level_1)
				{
					App->map->level == level_2;
					App->entity_m->player->needRespawn1 = true;
				}
				else if (App->map->level == level_2)
				{
					App->map->level == level_1;
					App->entity_m->player->needRespawn2 = true;
				}
				App->entity_m->player->animation = &App->entity_m->player->idleRight;
			}
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(char* map_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
		this->map_on = map_on;
	}

	return ret;
}

bool j1FadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}