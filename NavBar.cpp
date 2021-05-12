#pragma once
#include "NavBar.hpp"


NavBar::NavBar(SDL_Renderer* r, int w, int h, std::map<std::string, std::string> f) :
	renderer(r),
	search_bar(r),
	img_count(r),
	width(w),
	height(h),
	fonts(f),
	top_bar(new SDL_Rect{ 0, 0, w, 100 }),
	fuckbox(new SDL_Rect{ 0, 0, w, 100 }, r)
	{

	//Image count text
	img_count.setFont("search.ttf", 32);
	img_count.autosize = false;
	img_count.setRect({ 50, 25, 400, 50 });
	img_count.setColor({ 255,255,255,255 });
	img_count.setBoxImage("Bar.png");
	

	//The current input text.
	search_bar.autosize = false;
	search_bar.activate();
	search_bar.setMargins(15, 0, 0, 0);
	search_bar.setFont("search.ttf", 50);
	search_bar.setRect({ 500, 15, 950, 70 });
	search_bar.enableUpperCase();
	search_bar.setColor({ 255,255,255,255 });
	search_bar.setBoxImage("Bar.png", 230);
	search_bar.update();

	//The box that covers up the text overflow
	fuckbox.setRect(new SDL_Rect { 500 - 474, 15, 513, 70 }); // 865 is the size of the bar
	fuckbox.setImage("inversebar.png", 255, 58, 205, 220);
	
		
	//Top Nav bar
	SDL_Color topbarcolor = { 58, 205, 220 };
	rects.push_back(&top_bar);




};

void NavBar::display() {
	SDL_SetRenderDrawColor(renderer, 58, 205, 220, 255);
	SDL_RenderFillRect(renderer, top_bar.rect); //Our top bar
	SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);
	search_bar.display();
	fuckbox.display();
	img_count.display();

};

void NavBar::update() {
	for (auto& r : rects) {
		r->updateMove();
	}
	search_bar.updateMovement();
	img_count.updateMovement();
	fuckbox.updateMove();
};

void NavBar::move(Vector2f offset, Vector2f velocity) {
	for (auto& r : rects) {
		r->move(offset, velocity);
	}

	fuckbox.move(offset, velocity);
	search_bar.move(offset, velocity);
	img_count.move(offset, velocity);
};

void NavBar::moveToPosition(Vector2f destination, Vector2f velocity) {
	for (auto& r : rects) {
		r->moveToPosition(destination, velocity);
	}
	int x_delta = destination.x - x_origin;
	int y_delta = destination.y - y_origin;

	fuckbox.move({ x_delta , y_delta }, velocity);
	search_bar.move({ x_delta , y_delta}, velocity);
	img_count.move({ x_delta , y_delta }, velocity);
	y_origin = destination.y;
	x_origin = destination.x;

};

int NavBar::handleClick(Vector2f mousepos) {
	if (search_bar.handleClick(mousepos)) {
		return 1;
	}
	return 0;

};

NavBar::~NavBar() {

};