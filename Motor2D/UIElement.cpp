#include "UIElement.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"

UIElement::UIElement(iPoint position, j1Module* listener, bool drag) : localPosition(position), listener(listener), dragable(drag)
{

}

UIElement::~UIElement()
{

}

void UIElement::Update(float dt)
{
	//set the position for all childs
	//falta que se mueva con la camara
	if (parent != nullptr)
		positionUi = { localPosition.x + parent->positionUi.x,localPosition.y + parent->positionUi.y };
	else
		positionUi = { localPosition.x - App->render->camera.x, localPosition.y - App->render->camera.y };


	App->input->GetMousePosition(mouse.x, mouse.y);
	mouse.x = mouse.x - App->render->camera.x;
	mouse.y = mouse.y - App->render->camera.y;

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

				if (mousePositionDragX != tmp_mx2 || mousePositionDragY != tmp_my2)
				{
					mousePositionDragX = tmp_mx2;
					mousePositionDragY = tmp_my2;
					localPosition.x += mousePositionDragX;
					localPosition.y += mousePositionDragY;
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
	App->render->Blit(texture, positionUi.x, positionUi.y, &rectUi);
}

void UIElement::DebugDraw() const
{
	SDL_Rect rect = { positionUi.x,positionUi.y,rectUi.w,rectUi.h };
	App->render->DrawQuad(rect, 150, 150, 150, 40);
}


void UIElement::SetParent(UIElement* parent)
{
	this->parent = parent;
	positionToDraw = parent->positionToDraw + 1;
	App->gui->needOrderList = true;
}

void UIElement::SetLocalPosition(iPoint p)
{
	localPosition.x = p.x;
	localPosition.y = p.y;
}

void UIElement::ChangeTexture(const SDL_Texture* tex)
{
	texture = tex;
}