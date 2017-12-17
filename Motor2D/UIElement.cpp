#include "UIElement.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"


UIElement::UIElement(iPoint position, j1Module* listener, bool drag) : localPosition(position), listener(listener), dragable(drag)
{

}

UIElement::~UIElement()
{
	App->tex->UnLoad((SDL_Texture*)texture);
}

void UIElement::Update(float dt)
{
	//set the position for all childs
	if (parent != nullptr) 
			positionUi = { localPosition.x + parent->positionUi.x,localPosition.y + parent->positionUi.y };
	else
		positionUi = {localPosition.x, localPosition.y};


	App->input->GetMousePosition(mouse.x, mouse.y);
	

	if (mouse.x >= positionUi.x && mouse.x <= positionUi.x + rectUi.w && mouse.y >= positionUi.y && mouse.y <= positionUi.y + rectUi.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) || App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
			eventElement = MouseLeftClickEvent;
		else
			eventElement = MouseEnterEvent;

		if (dragable)
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT || App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				int tmp_mx2, tmp_my2;
				App->input->GetMouseMotion(tmp_mx2, tmp_my2);
				if (type == SliderElement) {
					{
						if (mouse.x >= positionUi.x && mouse.x >= positionUi.x) {
							SetSliderButtonPos(mouse.x);
							
							//localPosition.x = mouse.x;
							//positionUi.x = mouse.x;
						}
					}
				}

				else {
					if (mousePositionDragX != tmp_mx2 || mousePositionDragY != tmp_my2)
					{
						mousePositionDragX = tmp_mx2;
						mousePositionDragY = tmp_my2;
						localPosition.x += mousePositionDragX;
						localPosition.y += mousePositionDragY;
					}
				}
			}
	}

	else if (eventElement == MouseEnterEvent)
		eventElement = MouseLeaveEvent;
	else
		if (eventElement != FocusEventElement)
			eventElement = NoEventElement;


	listener->onUiTriggered(this, eventElement);

}

void UIElement::Draw()
{
	App->render->Blit(texture, positionUi.x, positionUi.y, &rectUi,0.0f);
}

void UIElement::DebugDraw() const
{
	SDL_Rect rect = { positionUi.x,positionUi.y,rectUi.w,rectUi.h };
	App->render->DrawQuad(rect, 0, 0, 0, 100,false,false);
}

void UIElement::SetParent(UIElement* parent)
{
	this->parent = parent;
	positionToDraw = parent->positionToDraw + 1;
}

void UIElement::SetLocalPosition(iPoint p)
{
	localPosition.x = p.x;
	localPosition.y = p.y;
}

void UIElement::ChangeTexture(const SDL_Texture* tex)
{
	if (texture != nullptr)
		App->tex->UnLoad((SDL_Texture*)texture);

	texture = tex;
}

void UIElement::ChangeTextureRect(SDL_Rect new_rect)
{
	rectUi = new_rect;
}

void UIElement::SetSliderButtonPos(int position) {}
