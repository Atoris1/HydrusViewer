#pragma once
#include "TagBox.cpp"
#include "Tag.hpp"

Tag::Tag(SDL_Renderer* rend, std::string text, TTF_Font* f, SDL_Color c, int m): renderer(rend), tag(text), font(f), color(c), max_length_pixels(m), text(NULL) {
};

SDL_Texture* Tag::getTexture() {
	return text;
};

std::string Tag::getTag() {
	return tag;
}

void Tag::update() {
	if (font == NULL) {
		std::cerr << "error font not loaded" << std::endl;
	}
	if (text != NULL) {
		SDL_DestroyTexture(text);
	}
	else {
		initColor();
		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, tag.c_str(), color, max_length_pixels);
		text = SDL_CreateTextureFromSurface(renderer, surface);
		printf("Error : %s\n", TTF_GetError());
		SDL_FreeSurface(surface);
	}
};

void Tag::initColor() {
	if (tag.substr(0, 5) == "chara") {
		color = { 17,137,27 };
	}
	else if(tag.substr(0, 5) == "ratin") {
		color = { 15,15,15 };
	}
	else if (tag.substr(0, 5) == "creat") {
		color = { 206,49,49 };
	}
	else if (tag.substr(0, 5) == "mediu") {
		color = { 107,214,198 };
	}
	else if (tag.substr(0, 5) == "serie") {
		color = { 199,73,221 };
	}
	else if (tag.substr(0, 5) == "gende") {
		color = { 233,155,247 };
	}
	else if (tag.substr(0, 5) == "pecor") {
		color = { 250,157,32 };
	}
};

void Tag::Destroy() {
	if (text != NULL) {
		SDL_DestroyTexture(text);
	}
};

