#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UILabel.h"
#include "UiImage.h"
#include "UIButton.h"
#include "j1Fonts.h"
#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Update(float);
	// TODO 2: Create the factory methods
	// Gui creation functions
	UIImage* CreateImage(iPoint, SDL_Rect, const SDL_Texture*, j1Module*, bool);
	UILabel* CreateLabel(iPoint, char*, SDL_Color, _TTF_Font*, j1Module*, bool);
	UIButton* CreateButton(iPoint, SDL_Rect, SDL_Rect, SDL_Rect, const SDL_Texture*, j1Module*, bool);
	void SortByDrawOrder();
	const SDL_Texture* GetAtlas() const;
	bool DeleteUIElements();
	bool needOrderList = false;

private:

	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
	p2List<UIElement*> UiElement;
	bool drawDebug = false;
	
};

#endif // __j1GUI_H__