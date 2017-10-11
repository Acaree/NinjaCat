#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "mCollision.h"

ModuleCollision::ModuleCollision()
{
	
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
	

	
	DebugDraw();

	return true;
}

void ModuleCollision::DebugDraw()
{
	

	
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	
}