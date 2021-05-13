#pragma once
#define NOMINMAX
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <mutex>
#include <thread>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "HydrusCall.hpp"
#include "Image.hpp"
#include "Utils.hpp"

using namespace std;
/*
The grid class contains all the data and logic for creating a grid of images
It handles all movement physics and all rectangle data for images.
The grid class does contain images but these images only know their textures not their rectangle positions.

	@Invariant Images >= 0, fileids >= 0, rects >= 0, image_boarder >= 0
*/


class Grid {

private:
	//Globals
	SDL_Renderer* renderer; //Renderer needed for drawing
	HydrusCall hydrus; //Websocket api calls, required for passing to images to gather data from Hydrus

	//Grid variables
	vector<string> fileids; //List of all images to be loaded
	vector<Image*> images; // Vector of all images
	vector<SDL_Rect> rects; //Vector of all image rectanlge positions
	vector<SDL_Rect> image_boarder; //Vector of all image border rectangle positions
	
	//Focus image variables
	Image* focusimg;
	int focus_index;
	
	//Threading variables
	thread buildaheadimg;
	thread buildaheadtext;
	std::thread getFile;
	
	//Query variables
	vector<string> hard_tags;

	//Constants
	int static const loadcount = 45;

	//???
	std::mutex g_lock;

	bool refresh = false;
	bool move = false;
	int yscreenpos = 0;

	int minimg = 0; //First image to load textures for
	int maximg = 0; //Last image to load textures for
	float dy = 0; //Varible for holding y screen acceleration
	int total_images = 0;;
	int minypos = 0;
	int max_img_per_row; //maximum number of images that can fit in a single row

	//Image loading/cache variables
	int loaded_images = 0; //Maximum amount of images shown
	int load = 35; //The amount of images that will be loaded next cycle
	
	bool thread_started = false;

	//Declare size of elements on screen
	int xthumb; //Thumbnail Width
	int ythumb; //Thumbnail Height
	int xpad; //X-axis padding
	int ypad; //Y-axis padding



	//Initilize x_rect_vect and y_rect_vect to be the starting position of our grid of rects
	int x_rect_vect = xpad;
	int y_rect_vect = ypad * 3;

	void BuildTextVect(int n);
	void BuildNextImages(int n);
	void BuildRectVect(int max, int& x, int& y, int offset = 0);
	void FitRectsToImages(int load);
	int findBottomPos(int depth);
	void destroyImages();

public:
	Grid(SDL_Renderer* rend, int x, int y);
	void addVelocity(int d);
	void renderThumbs();
	void search(string tags);
	void search(std::vector<std::string > tags);
	void fileIdSearch(std::vector<std::string> f);
	void update();
	bool HandleClick(Vector2f pos, SDL_Rect max_hit_area);
	bool requireRefresh();
	int FileCount();
	Image* GetFocusImage(int offset = 0);
	void DeleteImageFromHydrus();
	~Grid();
};