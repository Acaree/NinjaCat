#ifndef _SCOREBOARD_
#define _SCOREBOARD_

#include "j1Module.h"
#include "SDL_ttf\include\SDL_ttf.h"

class UILabel;
class UIButton;
class UIImage;
class UISlider;
struct SDL_Texture;

class j1ScoreBoard : public j1Module
{
public:
	j1ScoreBoard();

	//virtual ~j1MainMenu();
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
	void DeleteUI();
	void CreateUI();

	bool animationsFinish;
	bool AnimationsScence();

	int score_value=0;
private:
	//main menu gui
	SDL_Texture* background = nullptr;
	UIImage* score_mainImage = nullptr;
	UILabel* score_score = nullptr;
	UIButton* score_backToMenu = nullptr;
	
};
#endif