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
#include "Grid.hpp"
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080


Grid::Grid(SDL_Renderer* rend) :hydrus(45869){
	focusimg = NULL;
	renderer = rend;
};

void Grid::renderThumbs() {
	int width;
	int height;

	for (size_t k = minimg; k < maximg; k++) {

		SDL_SetRenderDrawColor(renderer, 145, 145, 145, 255);
		SDL_RenderFillRect(renderer, &rects[k]);
		SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);

		SDL_QueryTexture(images[k].getThumbText(), NULL, NULL, &width ,&height);
		rects[k].w = width;
		rects[k].h = height;

		SDL_RenderCopy(renderer, images[k].getThumbText(), NULL, &rects[k]);

	}
	for (size_t k = maximg; k < maxunloaded; k++) {
		SDL_SetRenderDrawColor(renderer, 145, 145, 145, 255);
		SDL_RenderFillRect(renderer, &rects[k]);
	}
};

void Grid::destroyImages() {
	for (auto& image : images) {
		image.Destroy();
	}
	images.clear();
};

void Grid::search(string tags) {
	//reset some variables;
	dy = 0;
	loaded_images = 0;
	thread_started = false;
	for (auto& rect : rects) {
		rect.y -= yscreenpos;
	};
	yscreenpos -= yscreenpos;
	destroyImages();
	fileids.clear();

	//Create query
	vector<string> query_string = createTagsFromString(tags);

	//create array of images
	Image* focusimg = NULL;

	//Perform a search with a tag
	query_string.push_back("-medium:animated");
	query_string.push_back("-non-conventional");
	fileids = hydrus.query(query_string);


	load = loadcount;
	if (fileids.size() < loadcount) {
		load = fileids.size();
	};
	minimg = 0;
	maximg = load;
	total_images = fileids.size();
	maxunloaded = load;

	//Build our images
	BuildNextImages(load);

	BuildTextVect(load);
	BuildRectVect(images.size(), x_rect_vect, y_rect_vect);
	loaded_images += load;
	cout << "image size after search : " << images.size() << endl;
	minypos = findBottomPos(total_images);
	refresh = true;
};

void Grid::update() {
	//Build more rects if getting close to having none
	if (minimg + 70 > rects.size()) {
		BuildRectVect(rects.size() + 35, x_rect_vect, y_rect_vect, yscreenpos);

	}

	//Make decicion to preload images.
	if (thread_started == false) {
		if (minimg + 70 > images.size() && fileids.size() != 0) {
			load = loadcount;
			if (fileids.size() < loadcount) {
				load = fileids.size();
			};
			buildaheadimg = std::thread(&Grid::BuildNextImages, this, load);
			thread_started = true;
		}
	}
	else {
		if (images.size() == loaded_images + load && buildaheadimg.joinable()) {
			buildaheadimg.join();
			buildaheadtext = std::thread(&Grid::BuildTextVect, this, load);
			buildaheadtext.join();
			loaded_images += load;
			thread_started = false;
			
			//recalculate loaded images
			int num_img_rows = std::ceil((-yscreenpos - ypad) / ((ythumb + ypad)));
			if (num_img_rows == 0) {
				minimg = 0;
			}
			minimg = ((num_img_rows) * 7);
			maximg = ((num_img_rows + 5) * 7);

			maxunloaded = std::max(maximg, loaded_images);
			maxunloaded = std::min(maxunloaded, total_images);
			maxunloaded = std::min(maxunloaded, (int)rects.size());
			maximg = std::min(maximg, loaded_images);
		}
	}



	//Physics and movement
	//Negative dy indicates down up postive dy indicates moving up
	if (-.5 > dy || dy > .5) {


		//Check if movement can occur.
		if (yscreenpos == 0 && dy > 0) {
			move = false;
			dy = 0;
		}
		else {
			move = true;
			if (yscreenpos == minypos ){
				//move = false;
				//dy = 0;
			}

			
		}

		//if calculation moved window beyond bounds reset bounds.
		//Top bound
		if (yscreenpos > 0) {
			for (auto& rect : rects) {
				rect.y -= yscreenpos;
			};
			yscreenpos -= yscreenpos;
			move = false;
		}
		//Bottom bound;
		else if (yscreenpos < minypos) {
			int diff = (yscreenpos - (minypos));
			for (auto& rect : rects) {
				rect.y -= diff;
			};
			yscreenpos -= diff;
			move = true;
		}

		//Move screen computation requires a screen update
		if (move) {
			for (auto& rect : rects) {
				rect.y += floor(dy);
			};
			yscreenpos += floor(dy);
			move = false;
		}

		int num_img_rows = std::ceil((-yscreenpos - ypad) / ((ythumb + ypad)));
		if (num_img_rows == 0) {
			minimg = 0;
		}
		minimg = ((num_img_rows) * 7);
		maximg = ((num_img_rows + 5) * 7);

		maxunloaded = std::max(maximg, loaded_images);
		maxunloaded = std::min(maxunloaded, total_images);
		maxunloaded = std::min(maxunloaded, (int)rects.size());
		maximg = std::min(maximg, loaded_images);

		//cout << "Minypos : " << minypos << endl;
		//cout << "rects size : " << rects.size() << endl;
		//cout << "images size : " << images.size() << endl;
		//cout << "minimg : " << minimg << endl;
		//cout << "maximg : " << maximg << endl;
		//cout << "max unloaded :" << maxunloaded << endl;;
		dy -= (dy / 25.f); //Scroll speed decay happens every loop
	};
	if (loadfile == 1) {
	
		 focusimg->BuildFileTexture(renderer, &fullfile);

		int imgx = 0;
		int imgy = 0;
		SDL_QueryTexture(focusimg->getFileText(), NULL, NULL, &imgx, &imgy);
		while (imgx > WIDTH-400) {
			imgx -= imgx / 10;
			imgy -= imgy / 10;
		}
		while (imgy > HEIGHT) {
			imgx -= imgx / 10;
			imgy -= imgy / 10;
		}
		int xpos = ((WIDTH) / 2) - (imgx / 2);
		if (xpos < 400) {
			int diff = 400 - xpos;
			xpos += diff;
		}

		focusrect = { xpos, ((HEIGHT - ypad * 2) / 2 - imgy / 2) + ypad * 2, imgx, imgy };
		focustext = focusimg->getFileText();

		loadfile = 0;
	}
}

bool Grid::presentFocus(int mousex, int mousey) {

	for (int i = 0; i < rects.size(); i++) {
		if (mousey >= rects[i].y && mousey <= rects[i].y + ythumb) {
			if (mousex >= rects[i].x && mousex <= rects[i].x + xthumb) {
				cout << "FOund rectangle" << endl;
				dy = 0;
				//TODO if a user clicks on an unloaded image preload the image.
				if (i >= images.size()) {
					focusimg = &images[0];
				}
				else {
					focusimg = &images[i];
				}
				
				int imgx;
				int imgy;
				if (focusimg->getFileText() != NULL) {
					cout << "texture already built : " << focusimg->getFileText() << endl;
					SDL_QueryTexture(focusimg->getFileText(), NULL, NULL, &imgx, &imgy);
					focustext = focusimg->getFileText();
					fullfile = 1;
				}
				else {
					SDL_QueryTexture(focusimg->getThumbText(), NULL, NULL, &imgx, &imgy);
					fullfile = 0;
				}
					imgx = imgx * 10;
					imgy = imgy * 10;
					while (imgx > WIDTH-400) {
						imgx -= imgx / 10;
						imgy -= imgy / 10;
					}
					while (imgy > (HEIGHT - ypad * 2)) {
						imgx -= imgx / 10;
						imgy -= imgy / 10;
					}
					int xpos = ((WIDTH) / 2) - (imgx / 2);
					if (xpos < 400) {
						int diff = 400 - xpos;
						xpos += diff;
					}
					focusrect = { xpos, ((HEIGHT - ypad * 2) / 2 - imgy / 2) + ypad * 2, imgx, imgy };
					//
					//getFile = (focusimg->BuildFileTextureThread(renderer, &fullfile));
	

				return true;
			}
		}
	}
	return false;
}

void Grid::renderFile() {
	if (fullfile) {
		//cout << "getFileText()" << focusimg->getFileText() << endl;
		//cout << "renderer" << renderer << endl;
		//cout << "focusrect" << &focusrect << endl;
		//cout << "images.size() : " << images.size() << endl;
		SDL_RenderCopy(renderer, focustext , NULL, &focusrect);
	}
	else {
		SDL_RenderCopy(renderer, focusimg->getThumbText(), NULL, &focusrect);
		loadfile = 1;
		//getFile.join();	//Defeats the purpose of multithreading

	}
}

Image* Grid::GetFocusImage() {
	return focusimg;
};

void Grid::addVelocity(int d) {
	dy += d;
};

bool Grid::requireRefresh() {
	if (refresh) {
		refresh = false;
		return true;
	}
	return (abs(dy) > .5) || (images.size() > maximg);
};

void Grid::BuildNextImages(int n) {
	vector<string> newids;
	for (int i = 0; i < n; i++) {
		string temp = fileids.back();
		newids.push_back(temp);
		fileids.pop_back();
	}
	//create array of images
	vector<Image> newimages = hydrus.createImages(newids);

	for (auto& image : newimages) {
		image.getThumbPath();
		images.push_back(image);
		
	}

}
void Grid::BuildTextVect(int n) {
	int size = images.size();

	for (size_t i = size - n; i < size; i++) {
		SDL_Surface* sur = IMG_Load((images[i].getThumbPath()).c_str());
		images[i].setThumbText(SDL_CreateTextureFromSurface(renderer, sur));
		SDL_FreeSurface(sur);
	}
	cout << "built " << n << " Textures" << endl;
}
void Grid::BuildRectVect(int max, int& x, int& y, int offset) {
	size_t size = rects.size();
	for (int i = rects.size(); i < max; i++) {
		if (x < 1700) {
			SDL_Rect temprect = { x, y + offset, xthumb, ythumb };
			rects.push_back(temprect);
			x += xthumb + xpad;
		}
		else {
			i--;
			x = xpad;
			y += ythumb + ypad;
		}
	}
	cout << "Done building " << max - size << " rectangles" << endl;
}
int Grid::findBottomPos(int depth) {
	int maxperrow = (WIDTH / (xthumb + xpad));
	int maxrows = floor(depth / maxperrow);
	if (depth % maxperrow != 0) { maxrows++; }
	return std::min(-((maxrows * (ypad + ythumb)) + ypad * 3 - HEIGHT), 0);
}

void Grid::DeleteImageFromHydrus() {

}

Grid::~Grid() {
	destroyImages();
};
