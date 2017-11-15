#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

j1Textures::j1Textures() : j1Module()
{
	name.create("textures");
}

// Destructor
j1Textures::~j1Textures()
{}

// Called before render is available
bool j1Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Textures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool j1Textures::CleanUp()
{
	LOG("Freeing textures and Image library");
	p2List_item<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		SDL_DestroyTexture(item->data);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const j1Textures::Load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool j1Textures::UnLoad(SDL_Texture* texture)
{
	p2List_item<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		if(texture == item->data)
		{
			SDL_DestroyTexture(item->data);
			textures.del(item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const j1Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.add(texture);
	}

	return texture;
}

// Retrieve size of a texture
void j1Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}


Animation j1Textures::CreateAnimation(char* anim_type, char* anim, bool loop) {
	Animation ret;

	pugi::xml_document anim_file;
	pugi::xml_parse_result result = anim_file.load_file("animations.xml");
	pugi::xml_node anim_node = anim_file.child("animations").child(anim_type).child(anim).child("frame");
	
	while (anim_node != nullptr) {
		ret.PushBack({ anim_node.attribute("x").as_int(),anim_node.attribute("y").as_int(),anim_node.attribute("width").as_int(),anim_node.attribute("height").as_int() });
		anim_node = anim_node.next_sibling();
	}
	ret.speed = anim_file.child("animations").child(anim_type).child("speeds").attribute(anim).as_float();
	ret.loop = loop;
	return ret;
}

float j1Textures::NormalizeAnimSpeed(char* anim_type, char* anim, float dt) {
	
	pugi::xml_document anim_file;
	pugi::xml_parse_result result = anim_file.load_file("animations.xml");
	
	float ret;
	ret = anim_file.child("animations").child(anim_type).child("speeds").attribute(anim).as_float()*dt;
	return ret;
}