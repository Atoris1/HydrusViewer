#pragma once
#include "MovableImage.h"
#include "SDL.h"
#include <SDL_image.h>

MovableImage::MovableImage(SDL_Rect* source_rect, SDL_Renderer* r): MovableRect(source_rect), renderer(r), texture(NULL) {
	color_refresh = false;
	alpha_step = 0;
	color_step = 0;
	dest_color = { 255,255,255,255 };
};

void MovableImage::display(bool box) {
	if (texture[texture_index]) {
		SDL_RenderCopy(renderer, texture[texture_index], NULL, rect);
	}
};

void MovableImage::setImage(string path, int alpha, int r, int g, int b, int pos) {
	std::string p = "Resources/sys_data/" + path;
	SDL_Surface* sur = IMG_Load(p.c_str());
	SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_SetTextureAlphaMod(temp, alpha);
	SDL_SetTextureColorMod(temp, r, g, b);
	texture.push_back(temp);
	
	current_color.r = r;
	current_color.g = g;
	current_color.b = b;
	current_color.a = alpha;
	SDL_FreeSurface(sur);
	return;
}

void MovableImage::chooseImage(int pos) {
	texture_index = pos;
}

void MovableImage::setColorMod(SDL_Color c, int interval) {
	dest_color.r = c.r;
	dest_color.g = c.g;
	dest_color.b = c.b;
	color_refresh = true;
	color_step = interval;
	
}

void MovableImage::setAlphaMod(int alpha, int interval) {
	dest_color.a = alpha;
	color_refresh = true;
	alpha_step = interval;
}


void MovableImage::updateColorUint8(Uint8& c, Uint8& d, int step) {
	if (c != d) {
		if (c < d) {
			if (step + c > 255) {
				c = 255;
			}
			else {
				c += step;
			}
			
			if (c >= d || c <= 0 || c > 255) {
				c = d;
			}
		}
		else {
			if (c - step < 0) {
				c = 0;
			}
			else {
				c -= step;
			}
			if (c <= d || c <= 0 || c > 255) {
				c = d;
			}
		}
	}
}


void MovableImage::updateColor() {
	if (color_refresh) {
		
		updateColorUint8(current_color.r, dest_color.r, color_step);
		updateColorUint8(current_color.g, dest_color.g, color_step);
		updateColorUint8(current_color.b, dest_color.b, color_step);
		updateColorUint8(current_color.a, dest_color.a, alpha_step);

		//std::cout << "setting color to r,g,b,a  -> " << (int)current_color.r << "," << (int)current_color.g << "," << (int)current_color.b << "," <<  (int)current_color.a << "," << endl;

		SDL_SetTextureColorMod(texture[texture_index], current_color.r, current_color.g, current_color.b);
		SDL_SetTextureAlphaMod(texture[texture_index], current_color.a);

		if (current_color.r == dest_color.r &&
			current_color.g == dest_color.g &&
			current_color.b == dest_color.b &&
			current_color.a == dest_color.a) {
			color_refresh = false;
		}
	}

};

