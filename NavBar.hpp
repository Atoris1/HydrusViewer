#pragma once
#include<SDL.h>
#include "Utils.hpp"
#include "TextBox.hpp"
#include "SearchBox.hpp"
#include "MovableImage.h"
#include "DropDownMenu.h"
#include "TextPredictionEngine.hpp"

class NavBar {
private:
	std::vector<MovableRect*> rects;

	
	SDL_Renderer* renderer;
	std::vector<TextBox*> textboxes;

	MovableRect top_bar;
	TextPredictionEngine TPE;

	std::map<std::string, std::string> fonts;

	int width;
	int height;

	int x_origin;
	int y_origin;

	Div* div;

	std::string active_tag;

	MovableImage fuckbox;

public:
	DropDownMenu ddm;
	SearchBox search_bar;
	TextBox img_count;



	NavBar(SDL_Renderer* r, int w, int h, std::map<std::string, std::string> f);
	void display();
	void update();
	void move(Vector2f offset, Vector2f velocity);
	void moveToPosition(Vector2f destination, Vector2f velocity);
	int handleClick(Vector2f mousepos);
	void updatePredicictions(string base);
	//std::string getActiveTag();
	~NavBar();
};