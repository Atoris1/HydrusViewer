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
	fuckbox(new SDL_Rect{ 0, 0, w, 100 }, r),
	ddm({ 625,85,700,0}, r),
	TPE()
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

	TextBox* t;
	
	for (int i = 0; i < 5; i++) {
		t = new TextBox(renderer);
		t->setRect({ 0, 0, 550, 20 });
		t->setColor({ 255,255,255,220 });
		t->setMargins(15, 15, 15, 0);
		//t->setBoxImage("button.png");
		t->setFont("search.ttf", 30);
		t->addTag(spaceToUnderscore("Penis music :)"));
		t->addTag("1");
		t->box = false;
		textboxes.push_back(t);
	}







	div = new Div(new MovableRect(new SDL_Rect({100,100,600,400})), renderer);
	div->setMargins(25, 20, 10, 50);
	
	for (auto& tt : textboxes) {
		div->addRect(tt);
	}
	
	
	ddm.setDiv(div);


};

void NavBar::updatePredicictions(string base) {
	int index = 0;

	std::vector<WeightedTag> predictions = TPE.getPrediction(base, 5);

	cout << "Prediction sizes : " << predictions.size() << endl;
	cout << "DDM is active ? : " << ddm.enabled() << endl;

	for (int i = 0; i < predictions.size(); i++) {
		textboxes[i]->pop_back();
		textboxes[i]->pop_back();
		textboxes[i]->addTag(predictions[i].tag);
		textboxes[i]->addTag(to_string(predictions[i].weight));
		textboxes[i]->update();
	}

	div->showElement(0, predictions.size());
	div->hideElement(predictions.size(), 5 - predictions.size());

	if (predictions.size() != 0) {
		ddm.recalculateHeight();
	}


	if (predictions.size() == 0) {
		ddm.deactivate();
	}
	else {
		ddm.activate();
	}

};



void NavBar::display() {
	SDL_SetRenderDrawColor(renderer, 58, 205, 220, 255);
	SDL_RenderFillRect(renderer, top_bar.rect); //Our top bar
	SDL_SetRenderDrawColor(renderer, 58, 58, 58, 255);
	search_bar.display();
	fuckbox.display();
	img_count.display();
	ddm.display();
};

void NavBar::update() {
	for (auto& r : rects) {
		r->updateMove();
	}
	search_bar.updateMovement();
	img_count.updateMovement();
	fuckbox.updateMove();
	ddm.update();
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
	if (ddm.enabled()) {
		for (auto& t : textboxes) {
			if (t->handleClick(mousepos)) {
				search_bar.pop_back();
				active_tag = t->getText();
				search_bar.addTag(spaceToUnderscore(t->getTags()[0]));
				cout << "adding tag <" << t->getTags()[0] << ">" << endl;
				search_bar.update();

				return 2;
			}
		}
	}
	



	if (search_bar.handleClick(mousepos)) {
		return 1;
	}
	else {
		ddm.deactivate();
		return 0;
	}
	

};

NavBar::~NavBar() {

};