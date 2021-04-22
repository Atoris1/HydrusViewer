#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <vector>
#include "RenderWindow.hpp"



RenderWindow::RenderWindow(const char* name, int w, int h) :window(NULL), renderer(NULL){
	// Create our window
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		std::cout << "Error renderer creation";
	}
	if (!window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
};
SDL_Renderer* RenderWindow::getRenderer() {
	return renderer;
};

void RenderWindow::CleanUp() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
};
