#include "UISlider.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

UISlider::UISlider(iPoint position, SDL_Rect rectBar, SDL_Rect rectButton, const SDL_Texture* texture, ElementType element, j1Module* listener, float initialValue) : UIElement(position,listener,dragable)
{
	this->rectUi = rectBar;
	this->texture = texture;
	this->rect_button = rectButton;
	this->rect_bar = rectBar;
	this->type = element;
	currentValue = initialValue;
	button_position.y = position.y;
	button_position.x = position.x + (rectUi.w*currentValue);
	dragable = true;
}

float UISlider::getValue()
{
	return currentValue;
}

void UISlider::Draw()
{
	//slider
	App->render->Blit(texture, positionUi.x, positionUi.y, &rect_bar);
	//button
	App->render->Blit(texture, button_position.x, button_position.y, &rect_button);
}

void UISlider::SetSliderButtonPos(int position) {
	button_position.x = position;
}
