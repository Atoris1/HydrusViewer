#pragma once
#include "MovableRect.h"

class MovableImage : public MovableRect {
private:
	SDL_Renderer* renderer;
	std::vector<SDL_Texture*> texture;
	int color_step;
	int alpha_step;

	SDL_Color current_color;
	SDL_Color dest_color;
	bool color_refresh;

	int texture_index;

	void updateColorUint8(Uint8& c, Uint8& d, int step);

	


public:
	MovableImage(SDL_Rect* source_rect, SDL_Renderer* r);
	void display(bool box = false);
	void setImage(string path, int alpha = 255, int r = 255, int g = 255, int b = 255 ,int pos = 0);
	void chooseImage(int pos);
	void setColorMod(SDL_Color c, int interval = 255);
	void setAlphaMod(int alpha, int interval = 255);
	void updateColor();
};