#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <SDL.h>

using namespace std;




class TextBox;
class Grid;
class User;
class ViewImage;

//Struct containing an x and y coordinate or velocity vector.
struct Vector2f {
	int x;
	int y;
};


//Enum defining the return value from the Settings handleClick() function
enum setting_state {
	NOCLICK = 0,
	DOWNLOAD = 1,
	FAVORITE = 2,
	DELETE_IMAGE = 3,
	UNFAVORITE = 4,
	TEXTBOX_SELECT = 5,
	FAVORITE_SEARCH = 6,
};




std::vector<std::string> createTagsFromString(std::string& str);

bool rectangleColision(SDL_Rect& rect, Vector2f& pos);
bool circleColision(SDL_Rect& rect, Vector2f& pos);

void threaded_image_destruction(ViewImage* viewimage);
bool vectContains(std::vector<int>& vect, int item);
void callErrorMessage(TextBox* notification_box, string message);


bool callImageDelete(TextBox* notification_box, Grid* grid, bool focus);
bool callImageSave(TextBox* notification_box, Grid* grid, bool focus, User* u);
bool callImageFavorite(TextBox* notification_box, Grid* grid, bool focus, User* u);
void callBufferClear(std::vector<ViewImage**> viewimages);

string underscoresToSpaces(string input);
std::vector<string> underscoresToSpaces(std::vector<string> input);
string spaceToUnderscore(string input);