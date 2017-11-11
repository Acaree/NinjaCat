#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "mCollision.h"
#include "j1Map.h"
#include "mPlayer.h"

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
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
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
		case COLLIDER_ENEMY:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_WALKENEMY:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
		}

	}
	
	//DRAW COLLISIONS
	p2List_item<MapLayer*>* item_layer = App->map->data.layers.start->next->next;
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

void ModuleCollision::CollisionToWorld(Collider* player, bool* movement)
{
	SDL_Rect playerRect = player->rect;
	// number of gid, layer collision
	uint wall = 141, dead = 143, playerStart = 144, changeLvl = 142;
	MapLayer* layerCollision;
	if (App->map->data.layers.start->next->next != nullptr)
	{
		layerCollision = App->map->data.layers.start->next->next->data;
		//points rect player, +- 34 set de x  smaller rect than original, -- bug
		iPoint rightUp = App->map->WorldToMap(playerRect.x + playerRect.w, playerRect.y);
		iPoint rightDown = App->map->WorldToMap(playerRect.x + playerRect.w - 34, playerRect.y + playerRect.h);
		iPoint leftUp = App->map->WorldToMap(playerRect.x, playerRect.y);
		iPoint leftDown = App->map->WorldToMap(playerRect.x + 34, playerRect.y + playerRect.h);
		//tiles in point of rect player
		int rightUpPlayer = layerCollision->Get(rightUp.x, rightUp.y);
		int rightDownPlayer = layerCollision->Get(rightDown.x, rightDown.y);
		int leftDownPlayer = layerCollision->Get(leftDown.x, leftDown.y);
		int leftUpPlayer = layerCollision->Get(leftUp.x, leftUp.y);

		for (int i = up; i <= death; i++)
			switch (i)
			{
			case right:
				rightDown = App->map->WorldToMap(playerRect.x + playerRect.w, playerRect.y + playerRect.h);
				rightDownPlayer = layerCollision->Get(rightDown.x, rightDown.y);
				leftDown = App->map->WorldToMap(playerRect.x + 50, playerRect.y + playerRect.h);
				leftDownPlayer = layerCollision->Get(leftDown.x, leftDown.y);
				//3 options: player in front of wall, player whit  "floor" , point rectangle player right down floor and rest don't collision
				if ((wall == leftDownPlayer && wall == rightDownPlayer && rightUpPlayer == wall) || (wall != leftDownPlayer && wall == rightDownPlayer && rightUpPlayer == wall) || (wall != leftDownPlayer && wall == rightDownPlayer && rightUpPlayer != wall))
				{
					movement[right] = false;
				}
				else
				{
					movement[right] = true;
				}
				if (player->type == COLLIDER_WALKENEMY)
				{
					if (leftDownPlayer == 0)
					{
						movement[right] = false;
					}
				}
				break;

			case left:
				rightDown = App->map->WorldToMap(playerRect.x + playerRect.w - 50, playerRect.y + playerRect.h);
				rightDownPlayer = layerCollision->Get(rightDown.x, rightDown.y);
				leftDown = App->map->WorldToMap(playerRect.x, playerRect.y + playerRect.h);
				leftDownPlayer = layerCollision->Get(leftDown.x, leftDown.y);
				// same right but left
				if ((wall == rightDownPlayer && wall == leftDownPlayer && leftUpPlayer == wall) || (wall != rightDownPlayer && wall == leftDownPlayer && leftUpPlayer == wall) || (wall != rightDownPlayer && wall == leftDownPlayer && leftUpPlayer != wall))
				{
					movement[left] = false;
				}
				else
				{
					movement[left] = true;
				}
				if (player->type == COLLIDER_WALKENEMY)
				{
					if (leftDownPlayer == 0)
					{
						movement[left] = false;
					}
				}
				
				break;


			case down:

				if (wall == leftDownPlayer || wall == rightDownPlayer)
				{
					movement[down] = false;
					App->player->jumping = false;
				}
				else
				{
					movement[down] = true;
				}
				break;

			case up:

				if (wall == leftUpPlayer || wall == rightUpPlayer)
				{
					movement[up] = false;
				}
				else
				{
					movement[up] = true;
				}
				break;

			case death:
				//check all positions 
				if ((leftUpPlayer == dead || rightUpPlayer == dead || leftDownPlayer == dead || rightDownPlayer == dead) && player->type == COLLIDER_PLAYER)
				{
					if (now == 0) {
						now = SDL_GetTicks();

					}
					if (now + 800 > SDL_GetTicks()) {
						App->player->currentAnimation = &App->player->dead;
						// stop all movement, else player go out of map, bug
						movement[down] = false;
						movement[left] = false;
						movement[right] = false;
					}
					else {
						App->player->currentAnimation = &App->player->idleRight;
						now = 0;
						if (App->player->isLevel1)
						{
							App->player->needRespawn1 = true;

						}
						else
						{
							App->player->needRespawn2 = true;
						}
						movement[down] = false;
					}
				}
				else if (player->type == COLLIDER_ENEMY)
				{
					iPoint playerTile = App->map->WorldToMap(App->player->position.x, App->player->position.y);
					iPoint enemyTile = App->map->WorldToMap(playerRect.x, playerRect.y);

					//if(enem)

				}
			case nextLevel:

				if (rightDownPlayer == changeLvl || rightUpPlayer == changeLvl)
				{
					App->player->changeLevel = true;
					if (App->player->isLevel1 == false)
					{
						App->player->isLevel1 = true;
					}
					else
					{
						App->player->isLevel1 = false;
					}
				}
			}
	}
}