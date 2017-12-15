#include "UISlider.h"
#include "j1App.h"
#include "j1Render.h"

UISlider::UISlider(iPoint position, SDL_Rect rectSlider, SDL_Rect rectButton, const SDL_Texture* texture, ElementType element, j1Module* listener, bool dragable) : UIElement(position,listener,dragable)
{
	this->rect_bar = rectSlider;
	this->rect_button = rectButton;
	this->texture = texture;
	this->type = element;
}

float UISlider::getValue()
{
	return currentValue;
}

void UISlider::Draw()
{
	//slider
	App->render->Blit(texture,positionUi.x,positionUi.y,&rect_bar);
	//button
	App->render->Blit(texture, positionUi.x + positionUi.x*currentValue, positionUi.y, &rect_button);
}