#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <SDL.h>
#include "Image.hpp"
#include "MovableRect.h"

class ViewImage {
private:
	Image* image;
	SDL_Renderer* renderer;
	MovableRect* rect;

	int screen_width;
	int screen_height;
	int tag_offset = 400;
	bool fullfile;
	bool fulltexture;
	std::mutex full_lock;
	std::thread t;
	int img_width;
	int img_height;
	int x_origin;
	int y_origin;

	void CalculateSize() {
		if (image->getFileText() != NULL) {
			cout << "File texture already built at location -> " << image->getFileText() << endl;
			SDL_QueryTexture(image->getFileText(), NULL, NULL, &img_width, &img_height);
			fullfile = true;
		}
		else {
			cout << "File texture is not found using thumbnail texture instead which is -> " << image->getThumbText() << endl;
			SDL_QueryTexture(image->getThumbText(), NULL, NULL, &img_width, &img_height);

			//Upscaling our thumbnail to fit the window size
			img_width = img_width * 3;
			img_height = img_height * 3;
		}


		img_width = img_width * 4;
		img_height = img_height * 4;



		float scaleFactor;

		float screenAspect = (float)screen_width / (float)screen_height;
		float rectAspect = (float)img_width / (float)img_height;


		if (screenAspect > rectAspect) {
			scaleFactor = (float)screen_height / (float)img_height;
		}
		else {
			scaleFactor = (float)screen_width / (float)img_width;
		}


		img_width = (float)img_width * scaleFactor;
		img_height = (float)img_height * scaleFactor;




		//finding centered x origin of screen
		x_origin = ((screen_width) / 2) - (img_width / 2);
		//Finding centered y origin of screen.
		y_origin = ((screen_height) / 2 - img_height / 2);
	};

public:
	ViewImage(Image* i, SDL_Renderer* r) : image(i), renderer(r), fullfile(false), fulltexture(false), rect(NULL){
		screen_width = 1920;
		screen_height = 1080;
		rect = new MovableRect(new SDL_Rect({ 0, 0, 0, 0 }));
		
		full_lock.unlock();


		CalculateSize();
		//Tell our image to open a thread and start building it's file texture

		//If our full file does not exist open a thread to download it
		//if (!i->fullFileExists()) {
		if (!fullfile) {
			t = std::thread(&ViewImage::BuildFile, this); //image->generateFile(); //This call can be ran in the background.
		}
	}	

	void exitView(int direction) {
		rect->setRect(new SDL_Rect({ x_origin, y_origin, img_width, img_height }));
		if (direction > 0) {
			rect->moveToPosition({ 1920+img_width, 0 }, { 70, 0 });
		}
		if (direction < 0){
			rect->moveToPosition({ 0 - img_width, 0 }, { -70, 0 });
		}
	}

	void enterView(int direction) {
		if (direction > 0) {
			rect->setRect(new SDL_Rect({ 0-img_width, rect->rect->y, img_width, img_height }));
			rect->moveToPosition({ x_origin, y_origin }, { 70, 70 });
		}
		if (direction < 0) {
			rect->setRect(new SDL_Rect({ img_width+x_origin, rect->rect->y, img_width, img_height }));
			rect->moveToPosition({ x_origin, y_origin }, { -70, 70 });
		}
		if(direction == 0) {
			rect->setRect(new SDL_Rect({ x_origin, y_origin, img_width, img_height }));
		}

	}

	void Display() {

		if (fulltexture) {
			SDL_RenderCopy(renderer, image->getFileText(), NULL, rect->rect);
		}
		else {
			SDL_RenderCopy(renderer, image->getThumbText(), NULL, rect->rect);
	
		}
	};

	void Update() {

		
		if (fullfile && !fulltexture) {
			image->BuildFileTexture(renderer); //This call cannot be ran in the background
			fulltexture = true;
		}
		rect->updateMove();

	}

	~ViewImage() {
		
		//delete rect;


		if (full_lock.try_lock()) {
			if (t.joinable()) {
				t.join();
				cout << "Thread joined after taking lock" << endl;
			}
		}
		else {
			cout << "Lock is locked and attempting to join thread and disassemble images" << endl;
			if (t.joinable()) {
				t.join();
				cout << "Thread joined after locked lock" << endl;
			}
			else {
				cout << "Thread cannot be joined for some reason and lock has been locked" << endl;
			}
		}
		
		if (fulltexture) {
			image->DestroyFileTexture();
		}
		if (fullfile) {
			image->DestroyFileSurface();
		}

		cout << "Image deconstructed !" << endl;
		
		

	}

	void BuildFile() {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		cout << "TRYING LOCK FUCK" << endl;
		if (full_lock.try_lock()) {
			cout << "Actually building file after 1 second delay" << endl << endl << endl;
			image->generateFile();
			image->generateFileSur();
			fullfile = true;
			full_lock.unlock();
		}
		else {
			cout << "Ignoring file after 1 seconds delay" << endl << endl << endl;
		}
		return;
	};
};