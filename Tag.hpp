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
	bool autocolor;


public:
	Tag(SDL_Renderer* rend, std::string text, TTF_Font* f, SDL_Color c, int m); //Default constructor
	Tag(SDL_Renderer* rend, std::string text, TTF_Font* f, SDL_Color c, int m, bool autoc); //Constructor with option of having tags autocolor
	void initColor();
	SDL_Texture *getTexture();
	std::string getTag();
	void setColor(SDL_Color c);
	void update();
	void setMaxWidth(int m);
	~Tag();

};