#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "UiButton.h"
#include "UiImage.h"
struct SDL_Texture;

enum Scene
{
	Main_scene,
	Settings_scene,
	Levels_scene,
	None_scene
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void onUiTriggered(UIElement* UIelement, EventElement EventElement);
	void CreateMainScene();
	void CreateSettingsScene();
	void DeleteSettings();
private:
	p2SString map;
	iPoint mouseCoordenates;
	iPoint mouseTile;
	p2List<UIButton*> buttons;
	UIButton* playButton= nullptr, *settingsButton = nullptr, *quitButton= nullptr;
	UIImage* settingsImage = nullptr;
	UIButton* plusVolume = nullptr, *minusVolume = nullptr, *crossButton = nullptr;
	Scene actualScene = None_scene;

};

#endif // __j1SCENE_H__