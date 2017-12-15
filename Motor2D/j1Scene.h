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
	pause_screen,
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
	void DeletePauseMenu();

	void SetLife(uint);
private:
	p2SString map;
	iPoint mouseCoordenates;
	iPoint mouseTile;
	p2List<UIButton*> buttons;

	//pause menu gui
	UIImage* pause_settingsImage = nullptr;
	UIButton* pause_crossButton = nullptr, *pause_playButton = nullptr, *pause_replayButton = nullptr, *pause_settingsButton = nullptr, *pause_returnButton = nullptr;

	//main menu gui
	UIImage* start_mainImage = nullptr;
	UIButton *start_playButton = nullptr, *start_settingsButton = nullptr, *start_quitButton = nullptr;

	//settings main menu
	UIImage* settingsmm_settingsImage = nullptr;
	UIButton* settingsmm_minusVolume = nullptr, *settingsmm_plusVolume = nullptr, *settingsmm_crossButton = nullptr;
	UILabel* settingsmm_volumeLabel = nullptr;

	//level gui
	UIButton* level_pauseButton = nullptr;
	UILabel* level_scoreLabel = nullptr;
	UIImage* level_lifesImage[3];
	
	SDL_Texture* current_volume_tex = nullptr;
	UILabel* current_volume_label = nullptr;
	bool pauseMenu = false;
};

#endif // __j1SCENE_H__