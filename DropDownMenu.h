#pragma once
#include "Utils.hpp"
#include "Div.h"


class DropDownMenu{
private:
	MovableImage rect;

	MovableImage top_img;
	MovableImage bottom_img;

	Div* div;
	SDL_Renderer* renderer;

	int speed = 15;	

	int bottom_size = 40;

	Vector2f dest;
	int total_height;
	bool active;
	SDL_Color bg_color;
	

public:
	DropDownMenu(SDL_Rect r, SDL_Renderer* rend);
	void setDiv(Div* d);
	void activate();
	void deactivate();
	void update();
	void display();
	void recalculateHeight();
	bool enabled();
	void setColor(SDL_Color c);
};

