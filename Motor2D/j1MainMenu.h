#ifndef _MAINMENU_
#define _MAINMENU_

#include "j1Module.h"

class UILabel;
class UIButton;
class UIImage;
class UISlider;
struct SDL_Texture;

class j1MainMenu : public j1Module
{
public:
	j1MainMenu();

	// Called before render is available
	bool Awake(pugi::xml_node&);
	
	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	 
	void onUiTriggered(UIElement*, EventElement);
	void DeleteMainMenuSettings();
	void CreateMainScene();
	void CreateSettingsScene();
	void DeleteSettings();
	private:
		//main menu gui
		UIImage* start_mainImage = nullptr;
		UIButton *start_playButton = nullptr, *start_settingsButton = nullptr, *start_quitButton = nullptr;

		//settings main menu
		UIImage* settingsmm_settingsImage = nullptr;
		UIButton* settingsmm_minusVolume = nullptr, *settingsmm_plusVolume = nullptr, *settingsmm_crossButton = nullptr;
		UILabel* settingsmm_volumeLabel = nullptr;

		SDL_Texture* current_volume_tex = nullptr;
		UILabel* current_volume_label = nullptr;

		bool settingsIsOpen = false;
};





#endif 

