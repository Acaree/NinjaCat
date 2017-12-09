#ifndef _UIBUTTON_
#define _UIBUTTON_

#include "UiElement.h"

class UIButton : public UIElement
{
public:
	UIButton(iPoint, SDL_Rect, SDL_Rect, SDL_Rect, const SDL_Texture*, ElementType, j1Module*, bool);
	SDL_Rect default_texture_rect = { 0,0,0,0 };
	SDL_Rect mouse_on_rect = { 0,0,0,0 };
	SDL_Rect clicked_rect = { 0,0,0,0 };
};

#endif

