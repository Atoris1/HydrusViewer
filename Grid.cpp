#pragma once
#define NOMINMAX
#include <thread>
#include <cstdlib>
#include "Grid.hpp"
#include "MovableRect.h"
using namespace std;

#define WIDTH 1920
#define HEIGHT 1080


Grid::Grid(SDL_Renderer* rend, int x, int y):hydrus(45869), renderer(rend), xthumb(x), ythumb(y), focusimg(NULL){
	xpad = 5; //X-axis padding
	ypad = 5; //Y-axis padding

	//Initilize x_rect_vect and y_rect_vect to be the starting position of our grid of rects
	x_rect_vect = xpad;
	y_rect_vect = 100 + ypad;

	//Get maximum images per row
	max_img_per_row = (WIDTH - xpad) / (xpad + xthumb);

	//Set hardcoded tags
	hard_tags.push_back("-medium:animated");
	hard_tags.push_back("-non-conventional");

};

void Grid::renderThumbs() {
	for (size_t k = minimg; k < maximg; k++) {

		//SDL_SetRenderDrawColor(renderer, 145, 145, 145, 255);
		//SDL_RenderFillRect(renderer, &image_boarder[k]);

		SDL_RenderCopy(renderer, images[k]->getThumbText(), NULL, &rects[k]);

		SDL_SetRenderDrawColor(renderer, 58, 205, 220, 255);
		SDL_RenderDrawRect(renderer, &rects[k]);
		SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);

	}
	for (size_t k = maximg; k < std::min((int)rects.size(),total_images); k++) {
		SDL_SetRenderDrawColor(renderer, 145, 145, 145, 255);
		SDL_RenderFillRect(renderer, &rects[k]);
	}
};

void Grid::destroyImages() {
	for (auto& image : images) {
		delete image;
	}
	images.clear();
};

void Grid::search(string tags) {
	//reset some variables;
	dy = 0;
	loaded_images = 0;
	thread_started = false;
	if (buildaheadimg.joinable()) { buildaheadimg.join(); }
	minimg = 0;
	maximg = 0;
	x_rect_vect = xpad;
	y_rect_vect = 100 + ypad;
	yscreenpos -= yscreenpos;

	//Clear all vectors
	destroyImages();
	rects.clear();
	fileids.clear();

	//Create query
	vector<string> query_string = createTagsFromString(tags);

	//Add our hard coded tags into our query
	for (auto& tag : hard_tags) {
		query_string.push_back(tag);
	}

	//Perform our search to get file_ids
	fileids = hydrus.query(query_string);

	//Set new limits
	total_images = fileids.size();
	minypos = findBottomPos(total_images);

	//Theses changes require a refresh to screen.
	refresh = true;
};

void Grid::search(std::vector<std::string> tags) {
	//reset some variables;
	dy = 0;
	loaded_images = 0;
	thread_started = false;
	if (buildaheadimg.joinable()) { buildaheadimg.join(); }
	minimg = 0;
	maximg = 0;
	x_rect_vect = xpad;
	y_rect_vect = 100 + ypad;
	yscreenpos -= yscreenpos;

	//Clear all vectors
	destroyImages();
	rects.clear();
	fileids.clear();

	//Create query
	vector<string> query_string = tags;

	//Add our hard coded tags into our query
	for (auto& tag : hard_tags) {
		query_string.push_back(tag);
	}

	//Perform our search to get file_ids
	fileids = hydrus.query(query_string);

	//Set new limits
	total_images = fileids.size();
	minypos = findBottomPos(total_images);

	//Theses changes require a refresh to screen.
	refresh = true;
};

void Grid::fileIdSearch(std::vector<std::string> f) {
	//reset some variables;
	dy = 0;
	loaded_images = 0;
	thread_started = false;
	if (buildaheadimg.joinable()) { buildaheadimg.join(); }
	minimg = 0;
	maximg = 0;
	x_rect_vect = xpad;
	y_rect_vect = 100 + ypad;
	yscreenpos -= yscreenpos;

	//Clear all vectors
	destroyImages();
	rects.clear();
	fileids.clear();

	fileids = f;

	//Set new limits
	total_images = fileids.size();
	minypos = findBottomPos(total_images);

	//Theses changes require a refresh to screen.
	refresh = true;
}

void Grid::update() {
	//Build more rects if getting close to having none
	if (minimg + 70 > rects.size()) {
		BuildRectVect(rects.size() + loadcount, x_rect_vect, y_rect_vect, yscreenpos);

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
		if ((int)images.size() == (loaded_images + load) && g_lock.try_lock()) {
			g_lock.unlock();
			buildaheadimg.join();
			BuildTextVect(load);
			
			
			//Take our new images and fit them to rectangles.
			FitRectsToImages(load);

			loaded_images += load;
			load = 0;
			thread_started = false;

			//recalculate loaded images
			int num_img_rows = std::ceil((-yscreenpos - ypad) / ((ythumb + ypad)));
			if (num_img_rows == 0) {
				minimg = 0;
			}
			minimg = ((num_img_rows) *max_img_per_row);
			maximg = ((num_img_rows + 5) * max_img_per_row);

			maximg = std::min(maximg, loaded_images);
			refresh = true;
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
			for (auto& rect : image_boarder) {
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
			for (auto& rect : image_boarder) {
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
			for (auto& rect : image_boarder) {
				rect.y += floor(dy);
			};
			yscreenpos += floor(dy);
			move = false;
		}

		int num_img_rows = std::ceil((-yscreenpos - ypad) / ((ythumb + ypad)));
		if (num_img_rows == 0) {
			minimg = 0;
		}
		minimg = ((num_img_rows) *max_img_per_row);
		maximg = ((num_img_rows + 5) * max_img_per_row);

		maximg = std::min(maximg, loaded_images);

		//cout << "Minypos : " << minypos << endl;
		//cout << "rects size : " << rects.size() << endl;
		//cout << "images size : " << images.size() << endl;
		//cout << "minimg : " << minimg << endl;
		//cout << "maximg : " << maximg << endl;
		//cout << "max unloaded :" << maxunloaded << endl;;
		dy -= (dy / 25.f); //Scroll speed decay happens every loop
	};
}


//This function sets the focus image based on a mouse position
//Returns true if an image was found returns false otherwise
bool Grid::HandleClick(Vector2f pos, SDL_Rect max_hit_area) {
	dy = 0;

	if (!(pos.x >= max_hit_area.x && pos.x <= max_hit_area.x + max_hit_area.w)) {
		return false;
	}
	if (!(pos.y >= max_hit_area.y && pos.y <= max_hit_area.y + max_hit_area.h)) {
		return false;
	}
		
	for (int i = minimg; i < std::min((int)rects.size(), total_images); i++) {
		if (pos.y >= rects[i].y && pos.y <= rects[i].y + rects[i].h) {
			if (pos.x >= rects[i].x && pos.x <= rects[i].x + rects[i].w) {
				//TODO if a user clicks on an unloaded image preload the image.

				if (i >= loaded_images) {
					return false;
				}
				else {
					focusimg = images[i];
					focus_index = i;
				}
				return true;
			}
		}
	}
	return false;
}


Image* Grid::GetFocusImage(int offset) {
	
	if ((focus_index+offset) < 0 || (focus_index + offset) > (loaded_images-1)) {
		
	}
	else {
		focus_index += offset;
	}

	focusimg = images[focus_index];
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
	g_lock.lock();
	cout << "starting image build" << endl;
	vector<string> newids;
	for (int i = 0; i < n; i++) {
		string temp = fileids.back();
		newids.push_back(temp);
		fileids.pop_back();
	}

	//Create new hydrus client so the thread has it's own.
	

	//create array of images

	vector<Image*> newimages = hydrus.createImages(newids);
	//skipping creaton of local variable to use directly in loop

	for (auto& image : newimages) {
		images.push_back(image);
		image->generateThumb();
	}
	g_lock.unlock();
}
void Grid::BuildTextVect(int n) {
	int size = images.size();
	for (size_t i = size - n; i < size; i++) {
		images[i]->BuildThumbTexture(renderer);
	}
	cout << "built " << n << " Textures" << endl;
}
void Grid::BuildRectVect(int max, int& x, int& y, int offset) {
	size_t size = rects.size();
	for (int i = rects.size(); i < max; i++) {
		if (x < 1700) {
			SDL_Rect temprect = { x, y + offset, xthumb, ythumb };
			rects.push_back(temprect);
			image_boarder.push_back(temprect);
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
void Grid::FitRectsToImages(int load) {
	int width;
	int height;
	int x_origin;
	int y_origin;

	for (size_t k = loaded_images; k < (loaded_images+load); k++) {
		SDL_QueryTexture(images[k]->getThumbText(), NULL, NULL, &width, &height);
		rects[k].w = width;
		rects[k].h = height;

		x_origin = rects[k].x;
		y_origin = rects[k].y;

		if (width < xthumb) {
			x_origin += (xthumb - width) / 2;
		}
		if (height < ythumb) {
			y_origin += (ythumb - height) / 2;
		}

		rects[k].x = x_origin;
		rects[k].y = y_origin;


	}

};
int Grid::findBottomPos(int depth) {
	int maxrows = floor(depth / max_img_per_row);
	if (depth % max_img_per_row != 0) { maxrows++; }
	return std::min(-((maxrows * (ypad + ythumb)) + 100 + ypad - HEIGHT), 0);
}

int Grid::FileCount() {
	return total_images;
}

void Grid::DeleteImageFromHydrus() {

}

Grid::~Grid() {
	destroyImages();
};
