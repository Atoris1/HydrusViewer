#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>


class RenderWindow {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;


public:
	RenderWindow(const char* title, int w, int h);
	SDL_Renderer* getRenderer();
	void CleanUp();
};