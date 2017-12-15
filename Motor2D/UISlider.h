#ifndef _SLIDER_
#define _SLIDER_

#include "UiElement.h"



class UISlider : public UIElement
{
public:
	UISlider(iPoint,SDL_Rect, SDL_Rect, const SDL_Texture*, ElementType, j1Module*, bool);
	float getValue();
	void Draw();

private:
	SDL_Rect rect_bar = { 0,0,0,0 };
	SDL_Rect rect_button = { 0,0,0,0 };
	float currentValue = 0.5;

};

#endif
