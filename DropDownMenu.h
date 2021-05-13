#pragma once
#include "Utils.hpp"
#include "Div.h"


class DropDownMenu{
private:
	MovableImage rect;
	Div* div;
	SDL_Renderer* renderer;

	int total_height;
	bool active;
	

public:
	DropDownMenu(SDL_Rect r, SDL_Renderer* rend);
	void setDiv(Div* d);
	void activate();
	void deactivate();
	void update();
	void display();
	void recalculateHeight();
};

