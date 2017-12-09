#include "UILabel.h"


UILabel::UILabel(iPoint position, const SDL_Texture* texture, ElementType type, j1Module* listener, bool dragable) : UIElement(position, listener, dragable)
{
	this->texture = texture;
	this->type = type;
}