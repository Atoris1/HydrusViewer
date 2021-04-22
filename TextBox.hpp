#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Tag.hpp"


class TextBox {
protected:
	std::vector<Tag> tags;
	std::vector<SDL_Rect> tag_rects;
	std::string selected_tag;

	SDL_Rect rect;
	SDL_Rect line_rect;
	int linedelay = 0;

	const int tag_width = 1000;
	TTF_Font* font;
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Renderer* renderer;

public:
	TextBox(SDL_Renderer* rend);
	void display(bool box = false);
	void update();
	void autoRect();
	void addTag(std::string tag);
	void remove(int n);
	std::string getText();
	void setRect(SDL_Rect rectangle);
	void setColor(SDL_Color);
	void setTags(std::vector<std::string>);
	void setTagsString(std::string tag_string);
	void setFont(const char* font_name, int font_size);
	int size();
	std::string pop_back();
	~TextBox();
};