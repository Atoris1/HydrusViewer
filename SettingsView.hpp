#pragma once
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Utils.hpp"
#include "TextBox.hpp"
#include "MovableImage.h"
#include "User.hpp"
#include "RadioButton.h"
#include "Div.h"
#include "Image.hpp"

class SettingsView {
private:
	SDL_Renderer* renderer;
	SDL_Texture* icon_text;
	SDL_Rect* icon_rect;
	SDL_Rect background_rect;
	SDL_Rect background_rect_bar;
	Image* active_image;
	std::vector<SDL_Rect*> rects;


	TextBox* active_textbox;

	std::vector<TextBox*> textboxes;

	RadioButton radio;
	User* user;

	Div div;

	MovableImage heart_img;
	MovableImage del_img;
	MovableImage download_img;
	MovableImage icon;

	bool active;
	bool show;
	int width;
	int height;
	int x_origin;
	int y_origin;
	int x_pos;
	int y_pos;
	int target_x_pos;
	float dx;
	float dy;


public:
	SettingsView(SDL_Renderer* r, User* u);
	void display();
	void setDimension(SDL_Rect r);
	void update();
	void activate();
	void deactivate();
	bool isActive();
	void setImage(Image* i);
	TextBox* getActiveTextbox();

	void activateImageOptions();
	void deactivateImageOptions();
	void scroll(int velocity);
	void updateDiv();
	void saveData();
	int handleClick(Vector2f pos);


};