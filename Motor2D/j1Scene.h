#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "UiButton.h"
#include "UiImage.h"
#include "UISlider.h"
#include "UiLabel.h"
#include "j1Timer.h"
struct SDL_Texture;


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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void onUiTriggered(UIElement* UIelement, EventElement EventElement);
	//void CreateMainScene();
	void CreateSettingsScene();
	void CreateLevelScene();
	//void DeleteSettings();
	//void DeleteMainMenuSettings();
	void CreatePauseMenu();
	void DeletePauseMenu();
	void DeleteSettings();
	void DeleteLevelUI();
	void UpdateGUI();
	void UpdateVolumeLabel();

	void SetLife(uint);
	p2List<UIButton*> buttons;
	UIImage* life1 = nullptr, *life2 = nullptr, *life3 = nullptr;
	/*uint score = 0;
	uint scoreRecord = 0;
	uint coinforLife = 0;
	uint coinCount = 0;*/
	j1Timer timer;
private:
	p2SString map;
	iPoint mouseCoordenates;
	iPoint mouseTile;
	

	//pause menu gui
	UIImage* pause_settingsImage = nullptr;
	UIButton* pause_crossButton = nullptr, *pause_playButton = nullptr, *pause_replayButton = nullptr, *pause_settingsButton = nullptr, *pause_returnButton = nullptr;

	//main menu gui
	UIImage* start_mainImage = nullptr;
	UIButton *start_playButton = nullptr, *start_settingsButton = nullptr, *start_quitButton = nullptr;

	//settings main menu
	UIImage* settingsmm_settingsImage = nullptr;
	UILabel* settingsmm_volumeLabel = nullptr, *settingsmm_volumeNumber = nullptr;
	UIButton* settingsmm_crossButton = nullptr;
	UISlider* settingsmm_volumeslider = nullptr;

	//level gui
	UIButton* level_pauseButton = nullptr;
	UILabel* level_scoreLabel = nullptr, *level_coinNumber = nullptr, *level_time = nullptr;
	UIImage* level_coinIcon = nullptr;
	
	SDL_Texture* current_volume_tex = nullptr;
	UILabel* current_volume_label = nullptr;
	bool pauseMenu = false;
	bool settingsIsOpen = false;
	
	uint soundClick;
};

#endif // __j1SCENE_H__