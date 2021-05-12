#pragma once
#include "Utils.hpp"
#include "MovableImage.h"
#include "MovableCircle.h"

class RadioButton {
private:
	SDL_Renderer* renderer;
	MovableImage ball;
	MovableImage bar;
	MovableImage ghost_ball;
	bool active_bool;
	int size;
	Vector2f origin;


public:
	RadioButton(SDL_Renderer* r, Vector2f o, int size);
	bool active();
	bool handleClick(Vector2f mouse_pos);
	void display();
	void updateMovement();
	void move(Vector2f offset, Vector2f velocity);
	void setState(bool state);
	int getWidth();
	int getHeight();
	void setPos(Vector2f pos);
	void enable();
	void disable();

};