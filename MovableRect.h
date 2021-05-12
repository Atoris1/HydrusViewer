#pragma once
#include <SDL.h>
#include "Utils.hpp"

enum movement {	
	NOMOVEMENT = 0,
	POSITIVE = 1,
	NEGATIVE = 2
};

enum offset {
	LEFT = 0,
	RIGHT = 1
};


class MovableRect {
private :
	Vector2f dest;
	float* dy;
	float* dx;
	bool inherit_velocity;
	Vector2f dest_size;
	Vector2f position;

	int growth_rate;
	int x_movement;
	int y_movement;



	int updateSizeInt(int* c, int& d);

public:
	SDL_Rect* rect;
	MovableRect(SDL_Rect* source_rect);
	void moveToPosition(Vector2f destination, Vector2f velocity);
	void move(Vector2f offset, Vector2f velocity);
	void updateMove();
	bool handleClick(Vector2f mousepos, bool circle = false);
	void setRect(SDL_Rect* r);
	void setPos(Vector2f v);
	void display(bool box = false);
	void inheritVelocity(float* x, float* y);
	void removeVelocity();
	int getWidth();
	int getHeight();
	void grow(Vector2f offset, int interval);
	void updateGrowth();
	~MovableRect();
};