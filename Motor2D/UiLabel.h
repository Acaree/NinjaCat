#ifndef _UILABEL_
#define _UILABEL_

#include "UIElement.h"

class UILabel : public UIElement
{
public:
	UILabel(iPoint, const SDL_Texture *, ElementType, j1Module*, bool);
};

#endif

