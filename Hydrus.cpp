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
#include "RenderWindow.hpp"
#include "TextBox.hpp"
#include "TagBox.cpp"

using namespace std;

const int debug = 1;
const int pecorepo = 0;

#define WIDTH 1920
#define HEIGHT 1080
#define LOADCOUNT 35
//Declare size of elements on screen
const int xthumb = 200; //Thumbnail Width
const int ythumb = 200; //Thumbnail Height
const int xpad = xthumb / 4; //X-axis padding
const int ypad = xthumb / 4; //Y-axis padding



int main(int argc, char* argv[]) {


	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}
	TTF_Init();

	RenderWindow window("Virus", WIDTH, HEIGHT);
	SDL_Renderer* renderer = window.getRenderer();

	//Create the grid and link it with the renderer
	Grid grid(renderer);

	//UI BUILIDNG
	//The current input text.

	TextBox search_bar(renderer);
	TagBox tagbox(renderer);
	search_bar.setFont("arial.ttf", 50);
	search_bar.addTag("pecorepo");
	search_bar.autoRect();

	tagbox.setFont("arial.ttf", 28);
	tagbox.setColor({ 255, 255, 255 });
	tagbox.setRect({ 0, ypad * 2, WIDTH / 4, HEIGHT });


	SDL_Color topbarcolor = { 58, 205, 220 };
	SDL_Rect topbarrect = { 0, 0, WIDTH, ypad * 2 };
	SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);



	//Start a default grid search
	grid.search("pecorepo");

	string tag_text = "pecorepo";
	bool textrender = false; //variable for determining to rerender text data;
	bool focus = false; //Varible for determining to show a single large image.
	bool focus_refresh = false;
	bool move = false; //Varible for determining if move logic should occur
	bool update = true; //Varible for deciding if the screen should be updated to reflect changes.
	bool quit = false; //Varible for exiting the program
	while (!quit)
	{
		SDL_Event e;
		//Update at every call of update
		if (grid.requireRefresh() || focus_refresh) {
			SDL_RenderClear(renderer);
			grid.renderThumbs();
			focus_refresh = false;
		}

		if (focus) {
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			grid.renderFile();
			tagbox.display();
			tagbox.updateMovement();
		}
		

		//Ui elements to keep refreshed
		SDL_SetRenderDrawColor(renderer, 58, 205, 220, 255);
		SDL_RenderFillRect(renderer, &topbarrect);
		SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);
		search_bar.display();
		SDL_RenderPresent(renderer);
		
	

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
					if (focus) {
						tagbox.addVelocity(10);
					}
					else {
						grid.addVelocity(10);
					}
				}
				else if (e.wheel.y < 0) // scroll wheel down scrol down
				{
					if (focus) {
						tagbox.addVelocity(-10);
					}
					else {
						grid.addVelocity(-10);
					}
				}
				break;

			case SDL_TEXTINPUT:
				//Not copy or pasting

				if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V' ))){}
				{
					
					if (e.text.text[0] != ' ') {
						if (tag_text == " ") { tag_text.pop_back(); }
						tag_text += e.text.text;
						search_bar.remove(1);
						search_bar.addTag(tag_text);
			
					}
					else {
						if (tag_text != "") {
							search_bar.remove(1);
							search_bar.addTag(tag_text);
							tag_text = "";
							search_bar.addTag(" ");
						}
						
					}
					textrender = true;

				}
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE)
				{
					
					if (tag_text.size() > 1) {
						tag_text.pop_back();
						search_bar.remove(1);
						search_bar.addTag(tag_text);
						textrender = true;
					}
					else {
						if (search_bar.size() > 1) {
							search_bar.remove(1);
							tag_text = search_bar.pop_back();
							search_bar.addTag(tag_text);

							textrender = true;
						}
						else if (search_bar.size() == 1) {
							search_bar.remove(1);
							tag_text = " ";
							search_bar.addTag(tag_text);
						}
	
					
					}
					cout << "tag_text state -> " << tag_text << endl;
				}

				//Load new set of images
				else if (e.key.keysym.sym == SDLK_RETURN) {
					if (tag_text == " ") {
						search_bar.pop_back();
					};

					grid.search(search_bar.getText());

					if (tag_text == " ") {
						search_bar.addTag(tag_text);
					};
					update = true;
					focus = false;
				}

				else if (e.key.keysym.sym == SDLK_DELETE) {
					if (focus) {
						grid.GetFocusImage()->DeleteImageFromHydrus();
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				int mousex;
				int mousey;
				SDL_GetMouseState(&mousex, &mousey);
				cout << "Mouse X : " << mousex << endl;
				cout << "Mouse Y : " << mousey << endl;
				if (e.button.button == SDL_BUTTON_LEFT) {
					

					if (focus) {
						string selected_tag = tagbox.getTagAtPos(mousex, mousey);
						if (selected_tag != "") {
							search_bar.addTag(selected_tag);
							tag_text = search_bar.pop_back();
							search_bar.addTag(tag_text);
						}
					}
					else {
						
						if (grid.presentFocus(mousex, mousey)) {

							vector<string> tags = grid.GetFocusImage()->getTags();
							string tagstring = "";
							int limit = 350;
							limit = std::min(limit, (int)tags.size());

							for (int i = 0; i < limit; i++) {
								tagstring += (tags[i]);
								tagstring += "\n";
							}

							tagbox.setTags(grid.GetFocusImage()->getTags());
							cout << "tagbox size -> " << tagbox.size() << endl;
							tagbox.update();
							textrender = true;
							focus = true;

						}
						else {
							focus = false;
						}
					}	
				}

				else {
					focus = false;
					focus_refresh = true;
				}

				break;
			}
		}

		if (textrender) {
			
			search_bar.update();
			search_bar.autoRect();
			textrender = false;
		}

		grid.update();
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