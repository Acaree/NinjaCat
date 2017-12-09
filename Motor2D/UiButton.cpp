#include "UIButton.h"

UIButton::UIButton(iPoint position, SDL_Rect rect_default, SDL_Rect rect_hover, SDL_Rect rec_click, const SDL_Texture* texture, ElementType type, j1Module* listener, bool dragable) :UIElement(position, listener, dragable)
{
	this->default_texture_rect = rect_default;
	this->rectUi = rect_default;
	this->mouse_on_rect = rect_hover;
	this->clicked_rect = rec_click;
	this->texture = texture;
	this->type = type;

}
