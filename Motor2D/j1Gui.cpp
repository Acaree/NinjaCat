#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	if (needOrderList == true)
	{
		//Order List to draw;
		SortByDrawOrder();
		needOrderList = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		drawDebug = !drawDebug;

	p2List_item<UIElement*> *Ui_item = UiElement.start;

	while (Ui_item != nullptr)
	{

		Ui_item->data->Update(dt);
		Ui_item = Ui_item->next;
	}

	Ui_item = UiElement.start;
	while (Ui_item != nullptr)
	{
		Ui_item->data->Draw();
		if (drawDebug == true)
			Ui_item->data->DebugDraw();
		Ui_item = Ui_item->next;
	}





	return true;

}
// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
UIImage* j1Gui::CreateImage(iPoint position, SDL_Rect rect, const SDL_Texture* texture, j1Module* listener, bool dragable)
{
	UIImage* newImage = new UIImage(position, rect, texture, ElementType::ImageElement, listener, dragable);
	UiElement.add((UIElement*)newImage);
	needOrderList = true;
	return newImage;
}

UILabel* j1Gui::CreateLabel(iPoint position, char* text, SDL_Color color, _TTF_Font* font, j1Module* listener, bool dragable)
{
	const SDL_Texture* tex = App->font->Print(text, color, font);

	UILabel* newLabel = new UILabel(position, tex, ElementType::LabelElement, listener, dragable);
	App->tex->GetSize(tex, (uint&)newLabel->rectUi.w, (uint&)newLabel->rectUi.h);
	UiElement.add((UIElement*)newLabel);
	needOrderList = true;
	return newLabel;
}

UIButton* j1Gui::CreateButton(iPoint position, SDL_Rect default_rect, SDL_Rect mouse_on, SDL_Rect clicked, const SDL_Texture* texture, j1Module* listener, bool dragable)
{
	UIButton* newButton = new UIButton(position, default_rect, mouse_on, clicked, texture, ElementType::ButtonElement, listener, dragable);
	UiElement.add((UIElement*)newButton);
	needOrderList = true;
	return newButton;
}

void j1Gui::SortByDrawOrder()
{
	bool swap = true;

	while (swap)
	{
		swap = false;
		p2List_item<UIElement*> *item = UiElement.start;

		while (item != nullptr && item->next != nullptr)
		{
			if (item->data->positionToDraw > item->next->data->positionToDraw)
			{
				SWAP(item->data, item->next->data);
				swap = true;
			}
			item = item->next;
		}
	}
}