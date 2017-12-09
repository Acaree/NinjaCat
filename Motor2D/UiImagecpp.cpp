#include "UiImage.h"


UIImage::UIImage(iPoint position, SDL_Rect rect, const SDL_Texture* texture, ElementType type, j1Module* listener, bool dragable) : UIElement(position, listener, dragable)
{
	this->rectUi = rect;
	this->texture = texture;
	this->type = type;
}