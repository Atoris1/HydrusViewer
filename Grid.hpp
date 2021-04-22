#pragma once
#define NOMINMAX
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "HydrusCall.hpp"
#include "Image.hpp"
#include "Utils.hpp"
using namespace std;

class Grid {

private:
	vector<Image> images;
	Image* focusimg;
	vector<SDL_Rect> rects;
	SDL_Rect focusrect;
	vector<string> fileids;
	SDL_Texture* focustext;
	SDL_Renderer* renderer;
	HydrusCall hydrus;
	thread buildaheadimg;
	thread buildaheadtext;
	std::thread getFile;
	string tagstring;
	bool refresh = false;
	int fullfile = 0;
	int loadfile = 0;
	bool textrender = false;
	bool move = false;
	int yscreenpos = 0;
	bool thread_started = false;
	int minimg = 0; //First image to load textures for
	int maximg = 0; //Last image to load textures for
	int maxunloaded = 0; //Maximum amount of unloaded images shown
	int loaded_images = 0; //Maximum amount of images shown
	int load = 35; //The amount of images that will be loaded next cycle
	float dy = 0; //Varible for holding y screen acceleration
	int total_images = 0;;
	int const loadcount = 35;
	int minypos = 0;
	//Declare size of elements on screen
	const int xthumb = 200; //Thumbnail Width
	const int ythumb = 200; //Thumbnail Height
	const int xpad = xthumb / 4; //X-axis padding
	const int ypad = xthumb / 4; //Y-axis padding
	//Initilize x_rect_vect and y_rect_vect to be the starting position of our grid of rects
	int x_rect_vect = xpad;
	int y_rect_vect = ypad * 3;

	void BuildTextVect(int n);
	void BuildNextImages(int n);
	void BuildRectVect(int max, int& x, int& y, int offset = 0);
	int findBottomPos(int depth);
	void destroyImages();

public:
	Grid(SDL_Renderer* rend);
	void addVelocity(int d);
	void renderThumbs();
	void renderFile();
	void search(string tags);
	void update();
	bool presentFocus(int x, int y);
	bool requireRefresh();
	Image* GetFocusImage();
	void DeleteImageFromHydrus();
	~Grid();
};