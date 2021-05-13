#define NOMINMAX
#pragma once
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
#include "MovableRect.h"
#include "HydrusCall.hpp"
#include "Image.hpp"
#include "Utils.hpp"
#include "Grid.hpp"
#include "RenderWindow.hpp"
#include "TextBox.hpp"
#include "SearchBox.hpp"

#include "SettingsView.hpp"

#include "NavBar.hpp"
#include "User.hpp"
#include <cctype>
#include <sstream>
#include "ViewImage.cpp"
#include "TextPredictionEngine.hpp"


using namespace std;

const int debug = 1;

#define WIDTH 1920
#define HEIGHT 1080


//Declare size of elements on screen
const int xthumb = 200; //Thumbnail Width
const int ythumb = 200; //Thumbnail Height
const int xpad = xthumb / 4; //X-axis padding
const int ypad = xthumb / 4; //Y-axis padding





int main(int argc, char* argv[]) {

	User u;
	TextPredictionEngine TPE;
	HydrusCall a (&u);

	std::map<std::string, std::string> fonts;
	fonts["search"] = "search.ttf";
	if (u.j["spongebob"].get<bool>()) {
		fonts["tags"] = "sponge.otf";
	}
	else {
		fonts["tags"] = "arial.ttf";
	}
	fonts["default"] = "arial.ttf";

	int const velocity = 25;


	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}
	TTF_Init();


	RenderWindow window("Korone Virus", WIDTH, HEIGHT);
	SDL_Renderer* renderer = window.getRenderer();


	//Views

	//Create the grid and link it with the renderer
	Grid grid(renderer, xthumb, ythumb);
	NavBar topbar(renderer, WIDTH, HEIGHT, fonts);
	
	


	
	
	//Tag text
	//TODO move into ViewImage class!
	TextBox tagbox(renderer);
	tagbox.setFont(fonts["tags"].c_str(), 28);
	tagbox.setColor({ 255, 255, 255 });
	tagbox.setRect({ 0, 100, 400, HEIGHT+4000 });
	tagbox.setMargins(20, 20, 20, 20);
	tagbox.setPadding(1000, 38);


	//Notification popup box.
	TextBox test(renderer);
	test.setFont(fonts["default"].c_str(), 32);
	test.autosize = false;
	test.setColor({ 255,255,255,255 });
	test.setRect({ WIDTH/2 - 200, HEIGHT, 400, 100 });
	test.setBoxImage("Bar.png", 230);
	test.setTag("Lmao");
	test.setMargins(10, 10, 10, 10);
	


	//Fullscreen image
	ViewImage* large_image = NULL;

	ViewImage* prev_img_buffer = NULL;
	ViewImage* next_img_buffer = NULL;

	//The FORBIDDEN	double pointer
	//We use this because we want to know what pointer each of our items points to
	//So we don't care about where the image is pointing to right now we want to always know where our image is pointing to
	//If we used single pointers these values would never change they would always point to NULL
	//But double pointers allow us to see the memory value that hold the memory address of a ViewImage so we can dynamicly
	//see where we are pointing to.
	std::vector<ViewImage**> view_img_buffers = {&large_image, &prev_img_buffer, &next_img_buffer};
	

	//Settings view
	SettingsView settings(renderer, &u);

	//Start a default grid search
	std::string result = "";
	for (auto& tag : u.j["default_search"]) {

		result += tag;
		result += " ";
	};


	
	grid.search(result);

	topbar.search_bar.setTagsString(result);
	topbar.search_bar.update();


	topbar.img_count.setTag("Results found : " + to_string(grid.FileCount())); //Set our result count
	topbar.update();
	topbar.img_count.autoRect();    //Center our result count.





	string tag_text = "";    //Search bar text buffer.
			
	bool textrender = false;			//variable for determining to rerender text data;
	bool focus = false;					//Varible for determining to show a single large image.
	bool force_grid_refresh = false;	
	bool move = false;					//Varible for determining if move logic should occur
	bool quit = false;					//Varible for exiting the program


	TextBox* activeTextBox = &topbar.search_bar;


	//Current time start time
	Uint32 startTime = 0;
	float averageFPS = 0;
	int frame_count = 0;

	Uint32 last_tick_time = 0;
	Uint32 frame_delta = 0;


	//In memory text stream
	std::stringstream timeText;

	TextBox time_box(renderer);
	time_box.setRect({ 100, 200, 400, 200 });
	time_box.setMargins(20, 20, 20, 20);
	time_box.addTag("Frame delta : ");
	time_box.addTag("0 ms");
	time_box.update();

	averageFPS++;



	while (!quit)
	{
		SDL_Event e;

		
		Uint32 tick_time = SDL_GetTicks();
		frame_delta = tick_time - last_tick_time;
		last_tick_time = tick_time;

		//time_box.pop_back();
		//time_box.addTag(to_string(frame_delta));
		//time_box.update();


		//Calling all display methods here
		if (grid.requireRefresh() || force_grid_refresh) {
			SDL_RenderClear(renderer);
			grid.renderThumbs();
			force_grid_refresh = false;
		}

		if (focus) {
			SDL_RenderClear(renderer);
			large_image->Display(); //Displays our current large_image
			if (prev_img_buffer) {
				prev_img_buffer->Display();
			}
			if (next_img_buffer) {
				next_img_buffer->Display();
			}
			tagbox.display();		//Will need to call update later
		}

		
		//Display every single frame 
		topbar.display();
		settings.display();
		test.display();
		//time_box.display(true);

		//Present our renderer to our window
		SDL_RenderPresent(renderer);


		//Calling all of our updates at the end of the frame.
		
		grid.update();
		settings.update();
		test.updateMovement();
		topbar.update();



		if (focus) {
			large_image->Update();
			if (prev_img_buffer) {
				prev_img_buffer->Update();
			}
			if (next_img_buffer) {
				next_img_buffer->Update();
			}
			tagbox.updateMovement();
		}



		//It runs until the number of events to be polled gets to zero
		while (SDL_PollEvent(&e) > 0)
		{
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_MOUSEWHEEL:
				if (e.wheel.y > 0) // scroll wheel up, scroll up
				{
					if (settings.isActive()) {
						settings.scroll(velocity/2);
					}
					else {
						if (focus) {
							tagbox.moveToPosition({ 0,100 }, { velocity, velocity });
						}
						else {
							grid.addVelocity(10);
						}
					}
				}

				else if (e.wheel.y < 0) // scroll wheel down scrol down
				{
					if (settings.isActive()) {
						settings.scroll(-velocity/2);
					}
					else {
						if (focus) {
							tagbox.move({ 0,-100000 }, { -velocity, -velocity });
							
						}
						else {
							grid.addVelocity(-10);
						}
					}
				}
				break;

			case SDL_TEXTINPUT:
				tag_text = activeTextBox->pop_back(); //Get our tag_data


				//Not copy or pasting
				if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V' ))){}
				{

					if (e.text.text[0] == ' ' && tag_text != " ") {
						activeTextBox->addTag(tag_text);
						tag_text = "";
					}
					else if (tag_text == " ") {
						//do nothing if they press space again.
					}
					else {
						tag_text += (char)(*e.text.text);
						//tag_text += (char)tolower(*e.text.text);
					}

					if (tag_text != " ") {
						std::vector<WeightedTag> temp;
						topbar.updatePredicictions(underscoresToSpaces(tag_text));
						topbar.ddm.activate();
					}
					



					
					activeTextBox->addTag(tag_text); //push back onto the tag stack
					textrender = true;
				}


				activeTextBox->update();
				settings.updateDiv();
				break;

			case SDL_KEYDOWN:
				//Handles the backspace input
				if (e.key.keysym.sym == SDLK_BACKSPACE)
				{
					tag_text = activeTextBox->pop_back(); //Get our tag_data
					if (tag_text == "") { //if our tag is empty get the next tag
						tag_text = activeTextBox->pop_back();
					} 
					
					else {// if our tag is not empty remove the last character
						tag_text = tag_text.substr(0, tag_text.size() - 1);
					}

					if (tag_text != " ") {
						std::vector<WeightedTag> temp;
						topbar.updatePredicictions(underscoresToSpaces(tag_text));
						topbar.ddm.activate();
					}



					activeTextBox->addTag(tag_text); //push back onto the tag stack
					textrender = true;
					activeTextBox->update();
				}

				//Load new set of images on enter key hit
				else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {

					if (activeTextBox == &topbar.search_bar) {
						grid.search(topbar.search_bar.getText());
						topbar.img_count.setTag("Results found : " + to_string(grid.FileCount()));
						topbar.img_count.autoRect();
						callBufferClear(view_img_buffers);
						focus = false;
					}
					if (activeTextBox == settings.getActiveTextbox()) {
						settings.saveData();
					}

				}

				//Handle the delete key being pressed
				else if (e.key.keysym.sym == SDLK_DELETE) {
					callImageDelete(&test, &grid, focus);
				}

				//Handle the escape key being pressed to reset the search bar.
				else if (e.key.keysym.sym == SDLK_ESCAPE) {
					while (activeTextBox->size() > 0) {
						activeTextBox->pop_back();
					}
					activeTextBox->addTag("");
					activeTextBox->update();
				}

				//Handle the left arrow key being pressed
				else if (e.key.keysym.sym == SDLK_LEFT) {
					if (focus) {
						if (grid.GetFocusImage(0) != grid.GetFocusImage(-1)) {
							settings.setImage(grid.GetFocusImage());


							//If going left we send our current image to the right and our previous image to the right;

							//If our next image is not null delete it and then replace it with our current image
							if (next_img_buffer != NULL) {
								std::thread(threaded_image_destruction, next_img_buffer).detach();
							}
							next_img_buffer = large_image;

							//If the previous image is in the buffer just make it the large image
							if (prev_img_buffer != NULL) {
								large_image = prev_img_buffer;
								prev_img_buffer = NULL;
							}
							else {
								large_image = new ViewImage(grid.GetFocusImage(), renderer);
							}
							

							//Tell our images to move accordingly
							next_img_buffer->exitView(1);
							large_image->enterView(1);

							
							tagbox.moveToPosition({ 0, 100 }, { 0, 50 });
							tagbox.setTags(underscoresToSpaces(grid.GetFocusImage()->getTags()));
							
						}
					}
				}

				//Handle the right arrow key being pressed
				else if (e.key.keysym.sym == SDLK_RIGHT) {
					if (focus) {
						//Disgusting compiler weirdness here requiring a temp variable due to the nature of getfocusImage changing the foucs image.
						Image* temp = grid.GetFocusImage(0);
						if (temp != grid.GetFocusImage(1)) {
							cout << "right image slide being called" << endl;
							settings.setImage(grid.GetFocusImage());
							//If going right we send our current image to the left and our previous image to the left;

							//If our previous image is not null delete it and then replace it with our current image
							if (prev_img_buffer != NULL) {
								std::thread(threaded_image_destruction, prev_img_buffer).detach();
							}
							prev_img_buffer = large_image;


							//If the next image is in the buffer just make it the large image
							if (next_img_buffer != NULL) {
								large_image = next_img_buffer;
								next_img_buffer = NULL;
							}
							else {
								large_image = new ViewImage(grid.GetFocusImage(), renderer);
							}


							//Tell our images to move accordingly
							prev_img_buffer->exitView(-1);
							large_image->enterView(-1);


							tagbox.moveToPosition({ 0, 100 }, {0, 50});
							tagbox.setTags(underscoresToSpaces(grid.GetFocusImage()->getTags()));
							
						}
					}
				}

				else if (e.key.keysym.sym == SDLK_LCTRL) {
					if (focus) {
						tagbox.moveToPosition({ -400, 0 }, { -velocity, 0 });
					}
					topbar.moveToPosition({ 0,-100 }, { -velocity+10, -velocity+10 });
					settings.deactivate();
				}
				break;


			//Mouse input
			case SDL_MOUSEBUTTONUP:
				int mousex;
				int mousey;
				SDL_GetMouseState(&mousex, &mousey);
				Vector2f mouse_pos = { mousex, mousey };

				startTime = SDL_GetTicks();



				if (e.button.button == SDL_BUTTON_LEFT) {

					
					int s_state;
					s_state = settings.handleClick(mouse_pos);

					switch (s_state) {
					case (DOWNLOAD):
						callImageSave(&test, &grid, focus, & u);
						break;
					case (DELETE_IMAGE):
						callImageDelete(&test, &grid, focus);
						break;
					case(FAVORITE):
						callImageFavorite(&test, &grid, focus, &u);
						break;
					case(TEXTBOX_SELECT):
						activeTextBox->deactivate();
						activeTextBox = settings.getActiveTextbox();
						activeTextBox->activate();
						break;
					case(FAVORITE_SEARCH):
						cout<<"running search" << endl;
						if (activeTextBox == &topbar.search_bar) {
							std::vector<string> query;
							for (auto& id : u.j["favorites"].get<vector<int>>()) {
								query.push_back(to_string(id));
							}
							grid.fileIdSearch(query);
							topbar.img_count.setTag("Results found : " + to_string(grid.FileCount()));
							topbar.img_count.autoRect();
							callBufferClear(view_img_buffers);
							focus = false;
						}
						break;
					}
					
					s_state = topbar.handleClick(mouse_pos);
					switch (s_state) {
					case(0):
						break;
					case (1):
						activeTextBox->deactivate();
						activeTextBox = &topbar.search_bar;
						activeTextBox->activate();
						break;
					case(2):
						break;
					}


					

					SDL_Rect max_search_area{ 0, 100, 1920, 1080 };

					if (settings.isActive()) {
						max_search_area.w -= 400;
					}


					
					topbar.moveToPosition({ 0, 0}, { velocity-10, velocity-10 });



					//If not on the focus panel find an image to focus
					if (!focus) {
						if (grid.HandleClick(mouse_pos, max_search_area)) {
							settings.setImage(grid.GetFocusImage());
							settings.activateImageOptions();
							if (large_image != NULL) {
								delete large_image;
							}
							large_image = new ViewImage(grid.GetFocusImage(), renderer);
							large_image->enterView(0);
							tagbox.setPos({ 0, 100 });
							tagbox.setTags(underscoresToSpaces(grid.GetFocusImage()->getTags()));
							for (auto& tag : grid.GetFocusImage()->getTags()) {
								cout << tag << endl;
							}
							focus = true;
						}
					}

					//If on the focus screen add look for a tag to add to the search bar.
					else {
						string selected_tag = spaceToUnderscore(tagbox.getTagAtPos(mouse_pos));
						if (selected_tag != "") {
							tag_text = topbar.search_bar.pop_back();
							topbar.search_bar.addTag(selected_tag);
							topbar.search_bar.addTag(tag_text);
							textrender = true;
						}

						
						tagbox.moveToPosition({ 0, ypad * 2 }, { velocity, 0 });
						



					}	

				}

				//On right click to exit focus view
				else {
					settings.deactivateImageOptions();
					settings.deactivate();
					
					callBufferClear(view_img_buffers);

					
					focus = false;
					force_grid_refresh = true;
					test.moveToPosition({ WIDTH / 2 - 200, HEIGHT }, {0,22 });

				}
				break;
			}
			

		}
		//Conditional updates
		if (textrender) {

			topbar.search_bar.update();
			textrender = false;
		}


	}

	// Destroy the window & renderer
	window.CleanUp();

	//SDL Cleanup
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();


	cout << "end" << endl;
	return 0;
};