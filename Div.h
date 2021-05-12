#pragma once
#include "MovableRect.h"
#include "MovableImage.h"
#include "RadioButton.h"
#include "TextBox.hpp"
#include <map>
#include <algorithm>

enum itemState {
	RECTANGLE = 0,
	DIV = 1,
	RECTANGLE_IMG = 2,
	TEXTBOX = 3,
	RADIOBUTTON = 4,
	
};


enum visableState {
	HIDDEN = 0,
	VISABLE = 1,



};

//Order type, visable, index
struct obj_metadata {
	int type;
	int visable;
	int index;


};

class Div{
private:
	MovableRect* bounds;
	MovableRect scroll_bar;
	MovableRect scroll_bar_box;


	//Allowed div object containers
	std::vector<MovableImage*> images;
	std::vector<MovableRect*> rects;
	std::vector<Div*> divs;
	std::vector<RadioButton*> radios;
	std::vector<TextBox*> textboxes;

	SDL_Renderer* renderer;

	int total_w;
	int total_h;

	MovableRect origin_box;

	int total_elements;

	int max_width;

	int x_padding;

	int left_m;
	int top_m;
	int right_m;
	int bottom_m;
	SDL_Color color;

	bool scrollbar_enable;
	bool render_bg = false;

	//Vector contains data in this order
	//Order type, visable, index
	std::map<int, obj_metadata> type_map;

	//void calculatePos(int* cw, int *ch, int* ph);

public:
	Div(MovableRect* re, SDL_Renderer* r);
	void setRects(std::vector<MovableImage*> r);
	void addRect(MovableImage* r);
	void addRect(MovableRect* r);
	void addRect(TextBox* r);
	void addRect(RadioButton* r);
	void addDiv(Div* d);
	void hideElement(int n, int m = 1); //Hides the n'th - m'th element in the div from view
	void showElement(int n, int m);
	void setBounds(MovableRect* r);
	void setPos(Vector2f pos);
	void scroll(int velocity);
	void move(Vector2f offset, Vector2f velocity);
	void display(bool box = false);
	void updateMove();
	void setMargins(int left = 0, int right = 0, int top = 0, int bottom = 0);
	Vector2f calculateSize();
	void updatePositions();
	int size();
};

