#pragma once
#include "TextBox.hpp"
#include "Tag.hpp"
#include "Utils.hpp"

TextBox::TextBox(SDL_Renderer* rend): renderer(rend), font(NULL) {
	setFont("arial.ttf", 22);
	setColor({ 15, 15, 15 });
	update();
};


void TextBox::display(bool box) {
	int i = 0;
	for (auto& tag : tags) {
		//
		if (box) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &tag_rects[i]);
			SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);
		}


		//std::cout << "tag value : <" << tag.getTag() << ">" <<  std::endl;
		SDL_RenderCopy(renderer, tag.getTexture(), NULL, &tag_rects[i]);

		i++;
	}
	if (linedelay > 40) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 160);
		SDL_RenderFillRect(renderer, &line_rect);
	}
	SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);
	if (linedelay > 80) {
		linedelay = 0;
	}
	linedelay++;

};

void TextBox::update() {
	int total_width = 0;
	int currentwidth = 0;
	int xpadding = 20;
	for (int i = 0; i < tags.size(); i++) {
		SDL_QueryTexture(tags[i].getTexture(), NULL, NULL, &currentwidth, NULL);
		total_width += currentwidth + xpadding;
	}
	

	int start_xpos = 0;
	start_xpos = (1920 / 2 - (total_width / 2));

	line_rect = SDL_Rect({ start_xpos + total_width - xpadding, 50 / 2, 8, 60 });
	tag_rects.clear();
	int width = 400;
	int height = 30;
	for (int i = 0; i < tags.size(); i++) {
		SDL_QueryTexture(tags[i].getTexture(), NULL, NULL, &width, &height);
		tag_rects.push_back(SDL_Rect({ start_xpos, 50/2, width, height }));
		start_xpos += width + xpadding;
	}
};

void TextBox::autoRect() {
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { 1920 / 2 - (texW / 2), 50 / 2, texW, texH };
	setRect(dstrect);
};
void TextBox::addTag(std::string tag) {
	Tag t = Tag(renderer, tag, font, color, tag_width);
	t.update();
	tags.push_back(t);
	update();
};
void TextBox::remove(int n) {
	if (tags.size() < n) {
		tags.clear();
	}
	else {
		for (int i = 0; i < n; i++) {
			tags.pop_back();
		}
	}
	update();
};
std::string TextBox::getText() {
	std::string result = "";
	for (auto& tag : tags) {

		result += tag.getTag();
		result += " ";
	};
	if (result.size() != 0) {
		result.pop_back();
	}
	std::cout << "result" << result << std::endl;
	return result;
};
void TextBox::setRect(SDL_Rect rectangle) {
	rect = rectangle;
};
void TextBox::setColor(SDL_Color c) {
	color = c;
};
void TextBox::setTags(std::vector<std::string> new_tags) {
	tags.clear();
	for (auto& tag : new_tags) {
		Tag t = Tag(renderer, tag, font, color, tag_width);
		t.update();
		tags.push_back(t);
	}
	update();
};
void TextBox::setTagsString(std::string tag_string) {
	std::vector<std::string> new_tags = createTagsFromString(tag_string);
	setTags(new_tags);
};

void TextBox::setFont(const char* font_name, int font_size) {
	if (font != NULL) {
		TTF_CloseFont(font);
	}
	font = TTF_OpenFont(font_name, font_size);
	update();
};

int TextBox::size() {
	return tags.size();
};

std::string TextBox::pop_back() {
	//std::cout << "fucntion size" << size() << std::endl;
	//std::cout << "acutal size" << tags.size() << std::endl;
	std::string result;
	result = tags[tags.size()-1].getTag();
	tags.pop_back();
	update();
	return result;
};


TextBox::~TextBox() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
};