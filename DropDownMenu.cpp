#include "DropDownMenu.h"
#include "MovableRect.h"

DropDownMenu::DropDownMenu(SDL_Rect r, SDL_Renderer* rend): rect(new SDL_Rect(r), rend), renderer(rend), div(NULL) {
	rect.grow_in_place = false;
	active = false;
};

void DropDownMenu::setDiv(Div* d) {
	div = d;
	total_height = d->calculateSize().y;
	div->setBounds(new MovableRect(new SDL_Rect({ rect.rect->x,rect.rect->y,rect.rect->w,total_height })));
	div->updatePositions();
	cout << "total hight is : " << total_height << endl << endl;

}

void DropDownMenu::recalculateHeight() {
	total_height = div->calculateSize().y;
	div->setBounds(new MovableRect(new SDL_Rect({ rect.rect->x,rect.rect->y,rect.rect->w,total_height })));
	div->updatePositions();
	cout << "total hight is : " << total_height << endl << endl;
	int height_diff = total_height - rect.rect->h;
	rect.grow({ 0, height_diff }, 5);

};

void DropDownMenu::activate() {
	if (!active) {
		rect.grow({ 0,total_height }, 5);
		active = true;
	}


};

void DropDownMenu::deactivate() {
	if (active) {
		rect.grow({ 0,-total_height }, 5);
		active = false;
	}
	

};

void DropDownMenu::update() {
	rect.updateGrowth();
	rect.updateMove();
	div->updateMove();
};

void DropDownMenu::display() {
	rect.display();
	if (rect.rect->h >= total_height) {
		div->display();
	}
	
};