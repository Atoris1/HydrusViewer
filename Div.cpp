#pragma once
#include "Div.h"


Div::Div(MovableRect* re, SDL_Renderer* r) : bounds(re),
total_h(0),
total_w(0),
renderer(r),
scroll_bar(NULL),
scroll_bar_box(NULL),
origin_box(new SDL_Rect({ 0,0,0,0 }))
{

	top_m = 10;
	bottom_m = 10;
	left_m = 25;
	right_m = 20;
	total_elements = 0;
	x_padding = 25;
	color = { 255,255,255, 220 };
	max_width = 0;
	scrollbar_enable = false;
	render_bg = false;

};

int Div::size() {
	return total_elements;
};


void Div::setRects(std::vector<MovableImage*> r) {
	total_elements = 0;
	images.clear();
	type_map.clear();
	images = r;
	for (int i = 0; i < r.size(); i++) {
		type_map[i] = {RECTANGLE_IMG, VISABLE, i};
	}
	total_elements = r.size();
}
void Div::addRect(MovableImage* r) {

	images.push_back(r);
	type_map[total_elements] = { RECTANGLE_IMG, VISABLE, (int)images.size() - 1 };
	total_elements++;

};
void Div::addRect(TextBox* r) {

	textboxes.push_back(r);
	type_map[total_elements] = { TEXTBOX, VISABLE, (int)textboxes.size() - 1 };
	total_elements++;

};

void Div::addRect(RadioButton* r) {
	radios.push_back(r);
	type_map[total_elements] = { RADIOBUTTON, VISABLE, (int)radios.size() - 1 };
	total_elements++;
};

void Div::hideElement(int n, int m) {
	for (int i = n; i < n + m; i++) {
		type_map[i].visable = HIDDEN;
	}
};

void Div::showElement(int n, int m) {
	for (int i = n; i < n + m; i++) {
		type_map[i].visable = VISABLE;
	}
};


void Div::addDiv(Div* d) {
	divs.push_back(d);
	type_map[total_elements] = { DIV, VISABLE, (int)divs.size() - 1 };
	total_elements++;
};

void Div::setBounds(MovableRect* r) {
	if (bounds) {
		delete bounds;
	}
	bounds = r;
	origin_box.setRect(new SDL_Rect{ bounds->rect->x, bounds->rect->y, 0, 0 });
	scroll_bar.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m, bounds->rect->y, 20, 0 });
	scroll_bar_box.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m, bounds->rect->y, 20, 0 });

};

void Div::setPos(Vector2f pos) {
	bounds->rect->x = pos.x;
	bounds->rect->y = pos.y;
	origin_box.setRect(new SDL_Rect{ bounds->rect->x, bounds->rect->y, 0, 0 });
	scroll_bar.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m, bounds->rect->y, 20, 0 });
	scroll_bar_box.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m, bounds->rect->y, 20, 0 });
};


void Div::scroll(int velocity) {
	int y_move_amount = 0;


	if (scrollbar_enable) {
		if (velocity > 0) {
			if (origin_box.rect->y <= bounds->rect->y) {
				y_move_amount = bounds->rect->y - origin_box.rect->y;
			}
			if (bounds->rect->y == origin_box.rect->y) {
				return;
			};


			origin_box.move({ 0, y_move_amount }, { 0,velocity });
			for (auto& r : rects) {
				r->move({ 0, y_move_amount }, { 0,velocity });
			}
			for (auto& d : divs) {
				d->move({ 0, y_move_amount }, { 0,velocity });
			}
			
			for (auto& i : images) {
				i->move({ 0, y_move_amount }, { 0,velocity });
			}
			
			for (auto& t : textboxes) {
				t->move({ 0, y_move_amount }, { 0,velocity });
			}
			for (auto& r : radios) {
				r->move({ 0, y_move_amount }, { 0,velocity });
			}
			
			float ratio = bounds->rect->h / total_h;


			scroll_bar.moveToPosition({ 0, bounds->rect->y+1 }, { 0, (int)(-velocity) });

		}
		else {

			if (bounds->rect->y <= (origin_box.rect->y + total_h - bounds->rect->h)) {
				y_move_amount = bounds->rect->y -(origin_box.rect->y + total_h - bounds->rect->h);
			}
			if (bounds->rect->y == (origin_box.rect->y + total_h - bounds->rect->h)) {
				return;
			};


			
			origin_box.move({ 0, y_move_amount }, { 0,velocity });
			for (auto& r : rects) {
				r->move({ 0, y_move_amount }, { 0,velocity });
			}
			for (auto& d : divs) {
				d->move({ 0, y_move_amount }, { 0,velocity });
			}
			for (auto& i : images) {
				i->move({ 0, y_move_amount }, { 0,velocity });
			}
			
			for (auto& t : textboxes) {
				t->move({ 0, y_move_amount }, { 0,velocity });
			}
			for (auto& r : radios) {
				r->move({ 0, y_move_amount }, { 0,velocity });
			}
			

			/*
			cout << "Y_move_Amount -> " << y_move_amount << endl;
			cout << "velocity -> " << velocity << endl;


			
			y_move_amount = y_move_amount / ratio;
			*/
			float ratio = bounds->rect->h / total_h;

			//cout << "difference from bound to originbox is : " << diff << endl;

			//cout << "scroll bar moving to y:" << bounds->rect->y + diff << endl;
			scroll_bar.moveToPosition({ 0,bounds->rect->y + (bounds->rect->h - scroll_bar.rect->h)-1}, { 0, (int)(-velocity) });



		}
	
	
	}
	
};


Vector2f Div::calculateSize() {
	updatePositions();
	return { bounds->rect->w, total_h };
};


void Div::updatePositions() {

	int start_x_pos = bounds->rect->x + left_m;
	int start_y_pos = bounds->rect->y + top_m;
	int current_width = 0;
	int current_height = 0;


	if (bounds->rect->y != origin_box.rect->y) {
		start_y_pos += origin_box.rect->y - bounds->rect->y;
	}

	max_width = bounds->rect->w - left_m - right_m;




	

	int previous_height = 0;

	for (int i = 0; i < total_elements; i++) {
		if (type_map[i].visable == VISABLE) {// make sure object should be visible in the div before calculating required space

			if (type_map[i].type == RECTANGLE_IMG) {
				if ((current_width + images[type_map[i].index]->getWidth()) > max_width) {//If item will not fit on current line
					current_height += previous_height;
					previous_height = 0;
					current_width = 0;

				}
				if (images[type_map[i].index]->getHeight() < previous_height) {
					int diff = previous_height - images[type_map[i].index]->getHeight();
					start_y_pos += diff / 2;
					images[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
					start_y_pos -= diff / 2;
				}
				else {
					images[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
				}
				current_width += images[type_map[i].index]->getWidth();
				current_width += x_padding;
				previous_height = std::max(images[type_map[i].index]->getHeight(), previous_height);
			}

			else if (type_map[i].type == RADIOBUTTON) {
				if ((current_width + radios[type_map[i].index]->getWidth()) > max_width) {//If item will not fit on current line
					current_height += previous_height;
					previous_height = 0;
					current_width = 0;

				}
				if (radios[type_map[i].index]->getHeight() < previous_height) {
					int diff = previous_height - radios[type_map[i].index]->getHeight();
					start_y_pos += diff / 2;
					radios[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
					start_y_pos -= diff / 2;
				}
				else {
					radios[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
				}


				current_width += radios[type_map[i].index]->getWidth();
				current_width += x_padding;
				previous_height = std::max(radios[type_map[i].index]->getHeight(), previous_height);
			}

			else if (type_map[i].type == TEXTBOX) {
				//cout << "vars before text box" << endl;
				//cout << "current_height ->" << current_height <<endl;
				//cout << "current_width ->" << current_height << endl;
				//cout << "width ->" << textboxes[type_map[i].index]->getWidth() << endl;
				//cout << "height ->" << textboxes[type_map[i].index]->getHeight() << endl;
				//cout << "previous height ->" << previous_height << endl;


				if ((current_width + textboxes[type_map[i].index]->getWidth()) > max_width) {//If item will not fit on current line
					current_height += previous_height;
					current_width = 0;
					previous_height = 0;


				}
				if (textboxes[type_map[i].index]->getHeight() < previous_height) {
					int diff = previous_height - textboxes[type_map[i].index]->getHeight();
					start_y_pos += diff / 2;
					textboxes[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
					start_y_pos -= diff / 2;
				}
				else {
					textboxes[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
					textboxes[type_map[i].index]->update();
				}


				current_width += textboxes[type_map[i].index]->getWidth();
				current_width += x_padding;
				previous_height = std::max(textboxes[type_map[i].index]->getHeight(), previous_height);
			}

			else if (type_map[i].type == DIV) {
				if ((current_width + divs[type_map[i].index]->calculateSize().x) > max_width) {//If item will not fit on current line
					current_height += previous_height;
					current_width = 0;
					previous_height = 0;


				}
				if (divs[type_map[i].index]->calculateSize().y < previous_height) {
					int diff = previous_height - divs[type_map[i].index]->calculateSize().y;
					start_y_pos += diff / 2;
					divs[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
					start_y_pos -= diff / 2;
				}
				else {
					divs[type_map[i].index]->setPos({ start_x_pos + current_width, start_y_pos + current_height });
				}


				current_width += divs[type_map[i].index]->calculateSize().x;
				current_width += x_padding;
				previous_height = std::max(divs[type_map[i].index]->calculateSize().y, previous_height);
			}

		}

		else if (type_map[i].visable == HIDDEN) {
			if (type_map[i].type == RECTANGLE_IMG) {
				images[type_map[i].index]->setPos({ -200,-200});
			}
		}
		
	}

	total_h = current_height + previous_height + top_m + bottom_m;


	if (total_h > bounds->rect->h) {
		scrollbar_enable = true;

		scroll_bar_box.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m, bounds->rect->y, 20, bounds->rect->h});

		int scroll_bar_height = (bounds->rect->h * bounds->rect->h) / total_h;

		if (scroll_bar.rect != NULL) {
			scroll_bar.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m + 1,scroll_bar.rect->y, 20 - 2, scroll_bar_height });
		}
		else {
			scroll_bar.setRect(new SDL_Rect{ bounds->rect->x + bounds->rect->w - right_m + 1, bounds->rect->y + 1, 20 - 2, scroll_bar_height });
		}

		
	}
	else {
		scrollbar_enable = false;
	}
};


void Div::setMargins(int left, int right, int top, int bottom) {
	left_m = left;
	right_m = right;
	top_m = top;
	bottom_m = bottom;
};

void Div::display(bool box) {
	if (box) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, bounds->rect);
	}

	if (scrollbar_enable) {
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderDrawRect(renderer, scroll_bar_box.rect);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, color.a);
		SDL_RenderFillRect(renderer, scroll_bar.rect);
	
	}

	for (int i = 0; i < total_elements; i++) {
		if (type_map[i].visable == VISABLE) {
			if (type_map[i].type == RECTANGLE_IMG) {
				images[type_map[i].index]->display();
			}
			else if (type_map[i].type == DIV) {
				divs[type_map[i].index]->display();
			}
			else if (type_map[i].type == TEXTBOX) {
				textboxes[type_map[i].index]->display();
			}
			else if (type_map[i].type == RADIOBUTTON) {
				radios[type_map[i].index]->display();
			}

		}
	}



};


void Div::move(Vector2f offset, Vector2f velocity) {
	bounds->move(offset, velocity);
	scroll_bar.move(offset, velocity);
	scroll_bar_box.move(offset, velocity);
	for (auto& r : images) {
		r->move(offset, velocity);
	}
	for (auto& r : rects) {
		r->move(offset, velocity);
	}
	for (auto& r : radios) {
		r->move(offset, velocity);
	}
	for (auto& r : textboxes) {
		r->move(offset, velocity);
	}
	for (auto& d : divs) {
		d->move(offset, velocity);
	}

};


void Div::updateMove() {
	bounds->updateMove();
	scroll_bar.updateMove();
	scroll_bar_box.updateMove();
	origin_box.updateMove();
	for (auto& r : rects) {
		r->updateMove();
	}
	for (auto& i : images) {
		i->updateMove();
	}
	for (auto& i : radios) {
		i->updateMovement();
	}
	for (auto& i : textboxes) {
		i->updateMovement();
	}
	for (auto& d : divs) {
		d->updateMove();
	}

	for (auto& i : images) {
		i->updateColor();
	}

};

