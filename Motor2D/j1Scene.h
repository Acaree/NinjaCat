#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "UiButton.h"
#include "UiImage.h"
#include "UiLabel.h"
struct SDL_Texture;

enum actual_level {
	none = 0,
	start_screen,
	settings_screen,
	level_1,
	level_2,
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

	actual_level level = none;

	void onUiTriggered(UIElement* UIelement, EventElement EventElement);
	void CreateMainScene();
	void CreateSettingsScene();
	void CreateLevelScene();
	void DeleteSettings();
	void DeleteMainMenuSettings();
	void CreatePauseMenu();
private:
	p2SString map;
	iPoint mouseCoordenates;
	iPoint mouseTile;
	p2List<UIButton*> buttons;
	UIButton* playButton= nullptr, *settingsButton = nullptr, *quitButton= nullptr;
	UIImage* mainImage = nullptr,*settingsImage = nullptr;
	UIButton* plusVolume = nullptr, *minusVolume = nullptr, *crossButton = nullptr;
	UIButton* pauseButton = nullptr;
	SDL_Texture* current_volume_tex = nullptr;
	UILabel* current_volume_label = nullptr;
	bool pauseMenu = false;
};

#endif // __j1SCENE_H__