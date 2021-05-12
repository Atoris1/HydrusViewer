#pragma once
#include "TextBox.hpp"
#include "SearchBox.hpp"
#include "Tag.hpp"
#include "Utils.hpp"
#include "algorithm"

SearchBox::SearchBox(SDL_Renderer* rend) : TextBox(rend) {


};

void SearchBox::update() {
	int text_width = 0;
	int text_height = 0;

	int width_offset = 0;
	int height_offset = 0;

	int max_width = width - right_m - left_m;
	int max_height = height - top_m - bottom_m;

	int x_start_pos = x_origin + left_m;
	int y_start_pos = y_origin + top_m;

	int total_width = 0;

	for (auto r : tag_rects) {
		delete r;
	}
	tag_rects.clear();

	box_rect = { x_origin, y_origin, width, height };

	all_rects.clear();
	all_rects.push_back(&box_rect);


	for (int i = 0; i < tags.size(); i++) {
		if (tags[i]->getTag() != "") {
			SDL_QueryTexture(tags[i]->getTexture(), NULL, NULL, &text_width, &text_height);
			total_width += text_width;
		}
	}
	total_width += x_padding * tags.size() - 1;
	int diff = 0;
	if (total_width > width) {
		diff = std::abs(width - total_width);
	}

	y_start_pos = y_origin / 2 + y_padding / 2 - 5;



	for (int i = 0; i < tags.size(); i++) {

		text_width = 0;
		text_height = 0;
		SDL_QueryTexture(tags[i]->getTexture(), NULL, NULL, &text_width, &text_height);


		SDL_Rect* temp = new SDL_Rect({ x_start_pos - diff, y_start_pos, text_width, text_height });
		tag_rects.push_back(temp);
		all_rects.push_back(temp);

		rect_to_tag[tag_rects.size() - 1] = i;
		x_start_pos += text_width + x_padding;
	}

	cursor_rect = { x_start_pos - diff - x_padding, y_start_pos , 7, y_padding - 4 };
	all_rects.push_back(&cursor_rect);

	height_offset += y_padding;


	if (autosize) {
		setRect({ box_rect.x, box_rect.y, box_rect.w, height_offset + top_m + bottom_m });
	}


	

	for (int i = 0; i < tags.size(); i++) {

		if (tags[i]->getTag() != "") {
			
		}
	}




};

