#pragma once
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <fstream>
#include <sstream>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SettingsView.hpp"
#include <filesystem>


string boolToString(bool b) {
	if (b) {
		return "True";
	}
	return "False";
}



SettingsView::SettingsView(SDL_Renderer* r, User* u) :
	renderer(r),
	heart_img(NULL, r),
	del_img(NULL, r),
	download_img(NULL, r),
	radio(r, { 1950,225 }, 50),
	user(u),
	div(NULL, r),
	dx(0),
	dy(0),
	width(400),
	height(1080),
	x_origin(1920),
	y_origin(0),
	icon(new SDL_Rect({ 1920 - 100, 0, 100, 100 }), r),
	active_image(NULL)
{

		x_pos = 1920;
		y_pos = 0;
		target_x_pos = 1920;
		active = false;
		show = false;

		heart_img.setRect(new SDL_Rect({x_origin+ 25,height - 100,100,100}));
		heart_img.setImage("heart.png", 220);
		heart_img.setColorMod(SDL_Color{ 255,255,255 });
		del_img.setRect(new SDL_Rect({ x_origin + 150,height - 100,100,100 }));
		del_img.setImage("trashcan.png", 220);
		del_img.setColorMod({ 50,50,50 });
		download_img.setRect(new SDL_Rect({ x_origin + 275,height - 100,100,100 }));
		download_img.setImage("download.png", 220);
		download_img.setColorMod({ 45,45,45 });
		download_img.updateColor();
		del_img.updateColor();
		heart_img.updateColor();

		radio.setState(u->j["spongebob"].get<bool>());


		
		



		//Settings icon
		icon.setImage("Menu.png", 225, 255, 255, 180);
		//SDL_Surface* settings_sur = IMG_Load("Resources/sys_data/Menu.png");
		//icon_text = (SDL_CreateTextureFromSurface(renderer, settings_sur));
		//SDL_FreeSurface(settings_sur);
		//SDL_SetTextureAlphaMod(icon_text, 225);
		icon_rect = icon.rect;

		//Settings submenu
		background_rect = { x_pos, 100, 400, height };
		background_rect_bar = { x_pos, 0, 400, 100 };

		rects.push_back(&background_rect);
		rects.push_back(&background_rect_bar);


		TextBox* tempbox = new TextBox(renderer);
		tempbox->autosize = false;
		tempbox->setRect({ x_origin, y_origin + 350, 350, 55 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(80, 25, 13, 13);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("Favorites <3");
		tempbox->setBoxImage("smallbutton.png", 255);
		tempbox->tag = "favorites";
		textboxes.push_back(tempbox);



		tempbox = new TextBox(renderer);
		tempbox->setRect({ x_origin + 25, y_origin + 350, width - 50, 50 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 15, 15);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("Hydrus api key :");
		tempbox->addTag(u->j["api-key"].get<string>());
		tempbox->editable = true;
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setRect({0, 0, width - 50, 50 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 15, 0);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("\n");
		tempbox->addTag("\n");
		tempbox->addTag("Host ip address :");
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setRect({ 0, 0, width - 50, 50 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 0, 15);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag(u->j["host"].get<string>());
		tempbox->editable = true;
		tempbox->tag = "host";
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setRect({ 0,0, width - 50, 50 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 15, 15);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("\n");
		tempbox->addTag("\n");
		tempbox->addTag("Spongebob mode :");
		tempbox->addTag(boolToString(u->j["spongebob"].get<bool>()));
		tempbox->tag = "spongebob";
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setRect({0, 0, width - 50, 20 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 15, 0);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("\n");
		tempbox->addTag("\n");
		tempbox->addTag("Default Search :");
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setRect({ 0, 0, width - 50, 50 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 0, 15);
		tempbox->setFont("search.ttf", 30);
		tempbox->setRect({ 0, 0, width - 50, 50 });
		for (auto& tag : u->j["default_search"]) {
			tempbox->addTag(tag);
			tempbox->addTag("\n");
		}
		tempbox->pop_back();
		tempbox->editable = true;
		tempbox->tag = "default_search";
		tempbox->box = false;
		textboxes.push_back(tempbox);


		tempbox = new TextBox(renderer);
		tempbox->setRect({ 0,0, width - 50, 50 });
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setMargins(15, 15, 15, 15);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("\n");
		tempbox->addTag("Window Width x Height :");
		tempbox->addTag("\n");
		tempbox->addTag(to_string(u->j["window_width"].get<int>()));
		tempbox->addTag("x");
		tempbox->addTag(to_string(u->j["window_height"].get<int>()));
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setColor({ 255,255,255,220 });
		tempbox->setRect({ 0,0, width - 50, 25 });
		tempbox->setMargins(15, 15, 15, 0);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag("\n");
		tempbox->addTag("\n");
		tempbox->addTag("Save Directory Path :");
		tempbox->box = false;
		textboxes.push_back(tempbox);

		tempbox = new TextBox(renderer);
		tempbox->setColor({ 255,255,255,220 });
		tempbox->enableUpperCase();
		tempbox->setRect({ 0,0, width - 50, 50 });
		tempbox->setMargins(15, 15, 0, 15);
		tempbox->setFont("search.ttf", 30);
		tempbox->addTag(u->j["save_directory"].get<string>());
		tempbox->editable = true;
		tempbox->tag = "save_directory";
		tempbox->box = false;
		textboxes.push_back(tempbox);

		div.setBounds(new MovableRect(new SDL_Rect({ x_origin, y_origin + 100, width, height - 100 })));

		div.addRect(&heart_img);
		div.addRect(&del_img);
		div.addRect(&download_img);
		
		
		

		Div* temp = new Div(new MovableRect(new SDL_Rect{ 0,0,350,300 }), renderer);
		temp->setMargins(120, 0, 0, 0);
		temp->addRect(&radio);
		temp->updatePositions();

		div.addRect(textboxes[0]);
		div.addRect(textboxes[1]);
		div.addRect(textboxes[2]);
		div.addRect(textboxes[3]);
		div.addRect(textboxes[4]);
		div.addDiv(temp);
		div.addRect(textboxes[5]);
		div.addRect(textboxes[6]);
		div.addRect(textboxes[7]);
		div.addRect(textboxes[8]);
		div.addRect(textboxes[9]);

		deactivateImageOptions();
	};

	void SettingsView::display() {
		if (show) {
			SDL_SetRenderDrawColor(renderer, 25,96,104,210);
			SDL_RenderFillRect(renderer, &background_rect);
			div.display();



			SDL_SetRenderDrawColor(renderer, 58, 205, 220, 255);
			SDL_RenderFillRect(renderer, &background_rect_bar);

			
			
		}

		icon.display();
	};

	void SettingsView::setDimension(SDL_Rect r) {
		width = r.w;
		height = r.h;
	};

	void SettingsView::update() {
		//TODO
		//delete this entire funcion.


		div.updateMove();
		icon.updateMove();

		if (dx == 0 ) {return;}



		if (dx > 0) {//Moving right
			if (x_pos < target_x_pos) { //Still moving to our destination 
				for (auto& rect : rects) {
						rect->x += dx;
				}


				//move the icon
				if (icon_rect->x < 1920 - 100) {
					icon_rect->x += dx;
				}

				x_pos += dx;
				dx -= dx / 30;
			}

			else { //Reached our destination
				int diff = target_x_pos - x_pos;
				for (auto& rect : rects) {
					rect->x -= diff;
				}
				x_pos -= diff;
				icon_rect->x = 1920 - 100; //rest out icon position
				dx = 0;
				show = false;
			}

			

		}
		else if (dx < 0) {//Moving left
			if (x_pos > target_x_pos) {//Still moving to our destination 
				//Move every element in settings menu
				for (auto rect : rects) {
					rect->x += dx;
				}


				//Move the icon
				if (icon_rect->x >= x_pos) {
					icon_rect->x += dx;
				}
				x_pos += dx;
				dx -= dx / 30;
			}

			else { //Reached our destination
				int diff = target_x_pos - x_pos;
				for (auto& rect : rects) {
					rect->x += diff;
				}
				x_pos += diff;

				dx = 0;
			}
		}
		


	};

	void SettingsView::activate() {
		active = true;
		show = true;
		dx = -25;
		target_x_pos = 1920 - 400;
		div.move({ -400, 0 }, { -25, 0 });


	};

	void SettingsView::deactivate() {
		if (active) {
		active = false;
		dx = 25;
		target_x_pos = 1920;
		div.move({ 400, 0 }, { 25, 0 });


		}
	};

	void SettingsView::scroll(int velocity) {
		if (velocity > 0) {
			div.scroll(velocity);
			//textbox->moveToPosition({ x_origin + 25, 200 }, { 0, velocity });
			
			
		}
		else {
			div.scroll(velocity);
			//textbox->move({ 0, -1000 }, { 0, velocity });
		}
	
	};

	void SettingsView::activateImageOptions() {
		std::vector<int> fav_vector = user->j["favorites"].get<std::vector<int>>();
		if (vectContains(fav_vector, stoi(active_image->getId()))){
			heart_img.setColorMod(SDL_Color{ 255,45,120 });
		}
		else {
		
		}
		div.showElement(0, 3);
		div.updatePositions();
	};

	void SettingsView::setImage(Image* i) {
		active_image = i;
		std::vector<int> fav_vector = user->j["favorites"].get<std::vector<int>>();
		if (vectContains(fav_vector, stoi(active_image->getId()))) {
			heart_img.setColorMod(SDL_Color{ 255,45,120 });
		}
		else {
			heart_img.setColorMod(SDL_Color{ 255,255,255 }, 10);
		}
	};

	void SettingsView::deactivateImageOptions() {
		heart_img.setColorMod(SDL_Color{ 255,255,255 });
		div.hideElement(0, 3);
		div.updatePositions();
	};

	bool SettingsView::isActive() {
		return active;
	};

	TextBox* SettingsView::getActiveTextbox() {
		return active_textbox;
	};

	void SettingsView::updateDiv() {
		div.updatePositions();
	};

	//Will return a int depending on action needing to be performed
	int SettingsView::handleClick(Vector2f pos) {
		if (pos.x < (1920 - 500)) {
			return NOCLICK;
		}

		if (active) {
			if (heart_img.handleClick(pos)) {
				std::vector<int> fav_vector = user->j["favorites"].get<std::vector<int>>();
				if (vectContains(fav_vector, stoi(active_image->getId()))) {
					heart_img.setColorMod(SDL_Color{ 255,255,255 }, 10);
					return FAVORITE;
				}
				else {
					heart_img.setColorMod(SDL_Color{ 255,45,120}, 10);
					return FAVORITE;
				}
			}
			else if(del_img.handleClick(pos)) {
				return DELETE_IMAGE;
			}
			else if (download_img.handleClick(pos)) {
				return DOWNLOAD;
			}
			else if (radio.handleClick(pos)) {
				if (radio.active()) {
					user->j["spongebob"] = true;
				}
				else {
					user->j["spongebob"] = false;
				}
				user->saveSettings();

				for (auto& t : textboxes) {
					if (t->tag == "spongebob") {
						t->pop_back();
						t->addTag(boolToString(user->j["spongebob"].get<bool>()));
						t->update();
						break;
					}
				}
				
			}
			else {
				for (auto& t : textboxes) {
					if (t->handleClick(pos)) {
						if (t->editable) {
							active_textbox = t;
							
							return TEXTBOX_SELECT;
						}
						if (t->tag == "favorites") {
							return FAVORITE_SEARCH;
						}
					}
				}
			}
		}

		if (icon.handleClick(pos)) {
			if (!active) {
				activate();
			}
			else {
				deactivate();
			}
		}
	};

	void SettingsView::saveData() {
		if (active_textbox->tag.size() > 0) {
			cout << "settings user setting <" << active_textbox->tag << "> To <" << active_textbox->getText() << ">" << endl;
			if (active_textbox->tag == "default_search") {
				user->j[active_textbox->tag] = active_textbox->getTags();
			}
			else {
				user->j[active_textbox->tag] = active_textbox->getText();
			}
			user->saveSettings();
			
		}
	}
