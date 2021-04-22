#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>


class Tag {
private:
	std::string tag;
	SDL_Texture* text;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Color color;
	int max_length_pixels;


public:
	Tag(SDL_Renderer* rend, std::string text, TTF_Font* f, SDL_Color c, int m);
	void initColor();
	SDL_Texture *getTexture();
	std::string getTag();
	void update();
	void Destroy();
};