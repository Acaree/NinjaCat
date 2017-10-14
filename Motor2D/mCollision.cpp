#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "mCollision.h"
#include "j1Map.h"
ModuleCollision::ModuleCollision()
{
	for (uint i = 0; i < MAX_COLLIDERS; i++)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

bool ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool ModuleCollision::Update(float dt)
{
	Collider *c1, *c2;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
	
	DebugDraw();

	return true;
}

void ModuleCollision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		}

	}
	
	//DRAW COLLISIONS
	p2List_item<MapLayer*>* item_layer = App->map->data.layermap.start->next->next;
	p2List_item<TileSet*>* item_set = App->map->data.tilesets.start->next->next;


	for (int x = 0; x < item_layer->data->width; x++)
	{
		for (int y = 0; y < item_layer->data->height; y++)
		{
			int tileID = 0;

			if (x > item_layer->data->width || y > item_layer->data->height)
			{
				tileID = 0;
			}
			else {
				tileID = item_layer->data->Get(x, y);
			}

			if (tileID > 0)
			{
				iPoint position = App->map->MapToWorld(x, y);
				SDL_Rect rect = item_set->data->GetTileRect(tileID);

				App->render->Blit(item_set->data->texture, position.x, position.y, &rect);

			}

		}
	}

	
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}