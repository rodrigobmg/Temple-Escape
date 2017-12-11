#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collider.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "Player.h"
#include "j1UI.h"

#include "UIImage.h"
#include "UIButton.h"

#include "j1Entities.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available

bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	if (!App->audio->PlayMusic("audio/music/arcade_funk.ogg")) {
		//ret = false;
		LOG("Error playing music in j1Scene Start");
	}

	// PAUSE BTN
	uint win_width = 0u, win_height = 0u;
	App->win->GetWindowSize(win_width, win_height);
	win_width /= App->win->GetScale();
	win_height /= App->win->GetScale();
	int tmp_x = 0;
	int tmp_y = 0;
	tmp_x = iPoint((win_width - 293) / 2, (win_height - 231) / 2).x;
	tmp_y = iPoint((win_width - 293) / 2, (win_height - 231) / 2).y;
	pause_menu = (UIImage*)App->ui->AddUIImage(tmp_x, tmp_y, { 135, 0, 293, 231 }, this);
	pause_menu->interactable = false;
	pause_menu->invisible = true;



	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//DEBUG
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && App->fadeToBlack->FadeIsOver()) 
	{
		if (App->map->sceneName != "lvl1.tmx") 
		{
			App->fadeToBlack->F1 = true;
			App->fadeToBlack->FadeToKnowBlack("lvl1.tmx");
		}
		else 
			putPlayerToSpawn();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
		putPlayerToSpawn();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->fadeToBlack->FadeIsOver() && !App->entities->GetPlayer()->isDead)
		App->SaveGame();
		
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN&& App->fadeToBlack->FadeIsOver())
		App->LoadGame();

	//END DEBUG

	App->map->Draw();

	int m_x; int m_y;
	App->input->GetMousePosition(m_x, m_y);
	iPoint p = App->map->WorldToMap(m_x, m_y);

	p2SString title("TEMPLE ESCAPE - Map:%dx%d Tiles:%dx%d Tilesets:%d || Coords in Map: X->%i Y->%i || Coords in World: X->%i Y->%i",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),p.x,p.y,m_x,m_y);

	//App->win->SetTitle(title.GetString()); // TODO: remove this if not needed and above too

	if (App->entities->GetPlayer()->reachedEnd && App->fadeToBlack->FadeIsOver()) 
	{
		App->entities->GetPlayer()->reachedEnd = false;
		App->fadeToBlack->FadeToBlack();
	}

	return true;
}

void j1Scene::putPlayerToSpawn() 
{
	App->entities->GetPlayer()->position.x = App->map->spawn.x;
	App->entities->GetPlayer()->position.y = App->map->spawn.y;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Load(pugi::xml_node& load) 
{
	bool ret = true;

	if (!load.child("currentLvl").empty()) 
	{
		p2SString lvlToLoad = (p2SString)load.child("currentLvl").attribute("name").as_string();

		if (App->map->sceneName != lvlToLoad && App->fadeToBlack->FadeIsOver()) 
		{
			App->entities->GetPlayer()->reachedEnd = false;
			App->fadeToBlack->FadeToKnowBlack(lvlToLoad);
		}
	}

	return ret;
}

bool j1Scene::Save(pugi::xml_node& save) const 
{
	bool ret = true;

	if (save.child("currentLvl").empty()) 
	{
		save = save.append_child("currentLvl");
		p2SString tmp = App->map->sceneName;
		save.append_attribute("name").set_value(tmp.GetString());
	}
	else 
	{
		p2SString tmp = App->map->sceneName;
		save.child("currentLvl").attribute("name").set_value(tmp.GetString());
	}

	return ret;
}

void j1Scene::OnUITrigger(UIElement* elementTriggered, UI_State ui_state) {
	if (elementTriggered->type == IMAGE) {
		UIImage* tmpImg = (UIImage*)elementTriggered;
		switch (ui_state)
		{
		case STATE_NORMAL:
			break;
		case STATE_MOUSE_ENTER:
			break;
		case STATE_MOUSE_LEAVE:
			break;
		case STATE_LEFT_MOUSE_PRESSED:
			break;
		case STATE_NO_DEF:
			break;
		default:
			break;
		}
	}
	else if (elementTriggered->type == BUTTON) {
		UIButton* tmpBtn = (UIButton*)elementTriggered;
		switch (ui_state)
		{
		case STATE_NORMAL:
			break;
		case STATE_MOUSE_ENTER: {
			for (int i = 0; i < buttons.Count(); i++)
			{
				if (buttons[i]->current_state == STATE_FOCUSED) {
					buttons[i]->current_state = STATE_NORMAL;
					tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
					break;
				}
			}
			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_focused);
			break;
		}
		case STATE_MOUSE_LEAVE:
			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
			break;
		case STATE_LEFT_MOUSE_PRESSED:
			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_pressed);
			break;
		case STATE_NO_DEF:
			break;
		default:
			break;
		}
	}
}