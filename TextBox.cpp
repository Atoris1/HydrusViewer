#pragma once
#include "TextBox.hpp"
#include "Tag.hpp"
#include "algorithm"



TextBox::TextBox(SDL_Renderer* rend) : renderer(rend), font(NULL), x_origin(0), y_origin(0), texture(NULL), top_m(0), bottom_m(0), left_m(0), right_m(0), x_padding(22), y_padding(22) {
	bg_color = { 58, 58, 58, 220 };
	setFont("search.ttf", 22);
	font_size = 22;
	setColor({ 15, 15, 15 });
	update();
};


void TextBox::display() {
	if (box) {
		if (texture) {
			SDL_RenderCopy(renderer, texture, NULL, &box_rect);
		}
		else {

			SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
			SDL_RenderFillRect(renderer, &box_rect);
		}	
	}
	if (active) {
		if (timer.getTicks() < 500) {
			SDL_SetRenderDrawColor(renderer, 255,255,255, 170);
			SDL_RenderFillRect(renderer, &cursor_rect);
		}
		else if (timer.getTicks() > 1000) {
			timer.reset();
		}
	
	
	}


	int i = 0;
	for (auto& tag : tags) {
		if (tag->getTag() == "\n") {
		
		}
		else {
			SDL_RenderCopy(renderer, tag->getTexture(), NULL, tag_rects[i]);
			i++;
		}
	}

	SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);

};

void TextBox::update() {//FIX MARGINS NOT WORKING ON SETPOS
	int text_width = 0;
	int text_height = 0;

	int width_offset = 0;
	int height_offset = 0;

	int max_width = width - right_m - left_m;
	int max_height = height - top_m - bottom_m;

	int x_start_pos = x_origin + left_m;
	int y_start_pos = y_origin + top_m;



	for (auto r : tag_rects) {
		delete r;
	}
	tag_rects.clear();

	box_rect = { x_origin, y_origin, width, height };

	all_rects.clear();
	all_rects.push_back(&box_rect);



	for (int i = 0; i < tags.size(); i++) {

		text_width = 0;
		text_height = 0;
		SDL_QueryTexture(tags[i]->getTexture(), NULL, NULL, &text_width, &text_height); 
		//cout << "For tag <" << tags[i]->getTag() << ">\n width : " << text_width << "\n height : " << text_height << endl;




		if (width_offset + text_width  > (max_width) && tags[i]->getTag() != "\n") {
			width_offset = 0;
			height_offset += y_padding;
			y_padding = font_size;
		}
		//This should be merged in with top statement but for clarity reasons it is here for now
		if (tags[i]->getTag() == "\n") {
			width_offset = 0;
			height_offset += y_padding;
			y_padding = font_size;
		}
		

		else {
			SDL_Rect* temp = new SDL_Rect({ x_start_pos + width_offset, y_start_pos + height_offset, text_width, text_height });
			tag_rects.push_back(temp);
			all_rects.push_back(temp);
			rect_to_tag[tag_rects.size()-1] = i;
			width_offset += text_width + x_padding;
			y_padding = std::max(text_height, font_size);
		}
		
	}

	cursor_rect = { x_start_pos + width_offset - x_padding, y_start_pos + height_offset, 7, y_padding-4 };
	all_rects.push_back(&cursor_rect);

	height_offset += y_padding;
	
	
	if (autosize){
		setRect({ box_rect.x, box_rect.y, box_rect.w, height_offset + top_m + bottom_m });
	}


};

void TextBox::activate() {
	active = true;
	timer.start();
};
void TextBox::deactivate() {
	active = false;
}

void TextBox::autoRect() {
	int text_width;
	int text_height;
	int width_offset = 0;
	int height_offset = 0;

	delete tag_rects[0];
	SDL_QueryTexture(tags[0]->getTexture(), NULL, NULL, &text_width, &text_height);
	SDL_Rect *dstrect =  new SDL_Rect({ (width / 2) - (text_width / 2) + (x_origin), (height /2) - (text_height / 2) + y_origin, text_width, text_height });
	tag_rects[0] = dstrect;	
	all_rects[1] = (dstrect);
};

void TextBox::addTag(std::string tag) {



	Tag* t = new Tag(renderer, tag, font, color, max_tag_width);
	t->update();
	tags.push_back(t);
};

void TextBox::remove(int n) {
	if (tags.size() < n) {
		tags.clear();
	}
	else {
		for (int i = 0; i < n; i++) {
			delete tags[tags.size() - 1];
			tags.pop_back();
		}
	}
	update();
};

std::string TextBox::getText() {
	std::string result = "";
	for (auto& tag : tags) {

		result += tag->getTag();
		result += " ";
	};
	if (result.size() != 0) {
		result.pop_back();
	}
	return result;
};

vector<std::string> TextBox::getTags() {
	std::vector<std::string> temp;
	for (auto& t : tags) {
		temp.push_back(t->getTag());
	}
	return temp;
};

void TextBox::setRect(SDL_Rect rectangle) {
	box_rect = rectangle;
	x_origin = box_rect.x;
	y_origin = box_rect.y;
	width = box_rect.w;
	height = box_rect.h;
	max_tag_width = rectangle.w - left_m - right_m;
	dest = { x_origin, y_origin };
	for (auto& tag : tags) {
		tag->setMaxWidth(max_tag_width);
	}
};
void TextBox::setColor(SDL_Color c) {
	color = c;
};
void TextBox::setTags(std::vector<std::string> new_tags) {
	for (auto& t : tags) {
		delete t;
	}
	tags.clear();
	for (auto& tag : new_tags) {
		Tag* t =  new Tag(renderer, tag, font, color, max_tag_width);
		t->update();
		tags.push_back(t);
	}
	update();
};
void TextBox::setTag(std::string new_tag) {
	for (auto& t : tags) {
		delete t;
	}
	tags.clear();
	Tag* t = new Tag(renderer, new_tag, font, color, max_tag_width);
	t->update();
	tags.push_back(t);
	update();
};
void TextBox::setTagsString(std::string tag_string) {
	std::vector<std::string> new_tags = createTagsFromString(tag_string);
	setTags(new_tags);
};
void TextBox::setFont(const char* font_name, int s) {
	font_size = s;
	if (font != NULL) {
		TTF_CloseFont(font);
	}
	string temp = "Resources/fonts/";
	temp += font_name;
	font = TTF_OpenFont(temp.c_str(), font_size);
	y_padding = font_size;
	update();
};
void TextBox::setPos(Vector2f pos) {
	x_origin = pos.x;
	y_origin = pos.y;
	box_rect.x = pos.x;
	box_rect.y = pos.y;
};
void TextBox::setBoxImage(std::string path ,int alpha) {
	std::string p = "Resources/sys_data/" + path;
	SDL_Surface* sur = IMG_Load(p.c_str());
	texture = (SDL_CreateTextureFromSurface(renderer, sur));
	SDL_SetTextureAlphaMod(texture, alpha);
	SDL_FreeSurface(sur);
	return;
}
void TextBox::setMargins(int left, int right, int top, int bottom) {
	left_m = left;
	right_m = right;
	top_m = top;
	bottom_m = bottom;
	max_tag_width = width - left_m - right_m;
	for (auto& tag : tags) {
		tag->setMaxWidth(max_tag_width);
	}
};
void TextBox::setPadding(int x, int y) {
	if (x != -1) {
		x_padding = x;
	}
	if (y != -1) {
		y_padding = y;
	}
};

void TextBox::enableUpperCase() {
	upper = true;
};

std::vector<SDL_Rect*> TextBox::getRects() {
	
	return all_rects;
}

void TextBox::move(Vector2f offset, Vector2f velocity, int time) {

	dest = {x_origin + offset.x, y_origin + offset.y};
	dx = velocity.x;
	dy = velocity.y;
}

void TextBox::moveToPosition(Vector2f destination, Vector2f velocity, int time) {

	if (time > 0) {
		std::thread(&TextBox::moveWait, this, destination, velocity, time).detach();
	}
	else {
		dest = destination;
		dx = velocity.x;
		dy = velocity.y;
	}

}

void TextBox::moveWait(Vector2f destination, Vector2f velocity, int time) {
	std::this_thread::sleep_for(std::chrono::seconds(time));
	moveToPosition(destination, velocity);
}

void TextBox::updateMovement() {

	if (dy == 0 && dx == 0) { return; }
	

	int int_dy = floor(dy);

	if (dx > 0) {//If moving to the right	
		if (dx < 5) { dx = 0; };
		if (x_origin < dest.x) {
			x_origin += dx;
			for (auto& r : getRects()) {
				r->x += dx;
			}
			dx -= dx / 30;
		}
		else {
			int diff = x_origin - dest.x;
			for (auto& r : getRects()) {
				r->x -= diff;
			}
			x_origin -= diff;
			dx = 0;
		}
	}

	else if (dx < 0) {//If moving to the left
		if (dx > -5) { dx = 0; };
		if (x_origin > dest.x) {
			x_origin += dx;
			for (auto& r : getRects()) {
				r->x += dx;
			}
			dx -= dx / 30;
		}
		else {
			int diff = x_origin - dest.x;
			for (auto& r : getRects()) {
				r->x -= diff;
			}
			x_origin -= diff;
			dx = 0;
		}
	}

	if (dy > 0) {//If moving down
		if (dy < 4) { dy = 0; };
		if (y_origin < dest.y) {
			y_origin += int_dy;
			for (auto& r : getRects()) {
				r->y += int_dy;
			}
			dy -= dy / 30;
		}
		else {
			int diff = y_origin - dest.y;
			for (auto& r : getRects()) {
				r->y -= diff;
			}
			y_origin -= diff;
			dy = 0;
		}
	}

	else if (dy < 0) {//If moving up
		if (dy > -4) { dy = 0; };
		if (y_origin > dest.y) {
			y_origin += int_dy;
			for (auto& r : getRects()) {
				r->y += int_dy;
			}
			dy -= dy / 30;
		}
		else {
			int diff = y_origin - dest.y;
			for (auto& r : getRects()) {
				r->y -= diff;
			}
			y_origin -= diff;
			dy = 0;
		}
	}
};

string TextBox::getTagAtPos(Vector2f mouse_pos) {
	if (mouse_pos.x > x_origin + width) { return ""; }
	if (mouse_pos.y > y_origin + height) { return ""; }
	
	int i = 0;
	for (auto& rect : tag_rects) {
		
		if (mouse_pos.x <= rect->x + rect->w && mouse_pos.x >= rect->x) {
			if (mouse_pos.y <= rect->y + rect->h && mouse_pos.y >= rect->y) {

				return tags[rect_to_tag[i]]->getTag();
			}
		}
		i++;
		

	};
	return "";


}

bool TextBox::handleClick(Vector2f mouse_pos) {
	if (rectangleColision(box_rect, mouse_pos)) {
		return true;
	};
	return false;
};

int TextBox::size() {
	return tags.size();
};

std::string TextBox::pop_back() {
	std::string result = "";
	if (size() > 0) {
		result = tags[tags.size() - 1]->getTag();
		delete tags[tags.size() - 1];
		tags.pop_back();
	}
	return result;

};

TextBox::~TextBox() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
	//for (auto& t:tags) {
	//	delete t;
	//}
};