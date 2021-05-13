#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Tag.hpp"
#include "Utils.hpp"
#include <thread>
#include <chrono>
#include <map>
#include "LTimer.h"


class TextBox {
protected:
	SDL_Renderer* renderer;
	std::vector<Tag*> tags; //All words in textbox
	std::vector<SDL_Rect*> tag_rects; //Rectangles for all words
	std::map<int, int> rect_to_tag; //maps the index of the rectangle to the index of the tag that needs to be displayed inside
	SDL_Rect box_rect; //Rectangle of bounding box of textbox
	SDL_Rect cursor_rect; //Rectangle of the blinking text indicator

	std::vector<SDL_Rect*> all_rects; //Vector of pointers to all rectangles for ease of movement of all objects

	int font_size; //Size in pixels of the font
	TTF_Font* font; //Font pointer of the text in the box
	SDL_Color color; //Color of the words in the textbox
	SDL_Color bg_color; //Color of the textbox background
	SDL_Texture* texture; //Texture of the background



	LTimer timer; //Timer for determining when to flash the cursor box
	

	int max_tag_width = 1000; //Maximum length of words in box

	//Internal data varaibles
	int x_origin;
	int y_origin;
	

	int width;
	int height;

	int left_m;
	int top_m;
	int right_m;
	int bottom_m;

	int x_padding;
	int y_padding;

	//Movement physics
	float dy;
	float dx;
	Vector2f dest;

	
	bool upper = false; //Alows upper case letters to be entered.

	//Helper functions
	void moveWait(Vector2f destination, Vector2f velocity, int time = 0);

public:
	//Textbox properties
	bool autosize = true; // Allows the textbox to change it's vertical size depending on the size of it's contents.
	bool editable = false; //If the textbox is able to be clicked on and edited by the user.
	bool active = false; //If the textbox should indicate that the user can type inside it.
	bool box = true; //If the textbox should show it's local bounds.
	string tag; //Metadata container for identifing textboxes
	
	//Setup functions
	TextBox(SDL_Renderer* rend);
	void setRect(SDL_Rect rectangle);
	void setColor(SDL_Color);
	void setFont(const char* font_name, int s);
	void setPos(Vector2f pos);
	void setBoxImage(std::string path, int alpha = 255, int r = 255, int g = 255, int b = 255);
	void setMargins(int left = 0, int right = 0, int top = 0, int bottom = 0); // Sets the distance from the edge of the textbox the elements can be.
	void setPadding(int x = -1, int y = -1); //Sets the spacing between elements, x = horizontal spacing, y = vertical spacing, -1 for value indicates no change
	void setTags(std::vector<std::string> new_tags);
	void setTag(std::string new_tag);
	void setTagsString(std::string tag_string); //takes a string of words seperated by spaces
	void addTag(std::string tag);
	void remove(int n); //Removes n# of tags from the tag vector
	std::string pop_back(); //Removes the last tag from the vector and return the value


	//State editing funcitons
	void activate(); //Activates the textbox for intput
	void deactivate(); //Deacivates the textbox for input
	void enableUpperCase(); //When called allows the textbox to store uppercase letters

	//Moves to a distance from it's original position designated by the offset vector at a certain velocity, time is the delay before the movement is performed
	void move(Vector2f offset, Vector2f velocity, int time = 0); 
	//Moves to a destination vector at a certain velocity, time is the delay before the movement is performed.
	void moveToPosition(Vector2f destination, Vector2f velocity, int time = 0);

	//Use functions
	void display();	//Displays the textbox elements
	void update(); //Recalculates the positions of elements in the textbox
	void updateMovement(); //Updates the movement according to the destination and the velocity
	bool handleClick(Vector2f mouse_pos); //Returns true if the mouse_position was inside of the textbox
	void autoRect(); //Bro idk this does something really fucked up

	//Data reterival functions
	std::string getText();
	std::vector<SDL_Rect*> getRects();
	int getHeight() { return height; };
	int getWidth() { return width; };
	string getTagAtPos(Vector2f mouse_pos);
	vector<std::string> getTags();
	int size();
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	~TextBox();
};