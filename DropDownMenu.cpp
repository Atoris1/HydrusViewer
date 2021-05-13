#include "DropDownMenu.h"
#include "MovableRect.h"
#include <algorithm>

DropDownMenu::DropDownMenu(SDL_Rect r, SDL_Renderer* rend): 
	rect(new SDL_Rect(r), rend),
	bottom_img(new SDL_Rect(r), rend),
	top_img(new SDL_Rect(r), rend),
	renderer(rend), div(NULL)
	{

	rect.grow_in_place = false;
	bg_color = { 25,96,104,210 };
	top_img.setImage("rect.png", 235, 55, 55, 55);
	bottom_img.setImage("dropdown_bottom.png", 235, 55, 55, 55);
	active = false;
	dest = { 0,0 };
	
};

void DropDownMenu::setColor(SDL_Color c) {
	rect.setAlphaMod(c.a);
	rect.setColorMod({ c.r, c.g, c.b });
	rect.updateColor();
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
	int height_diff = total_height - dest.y;
	cout << "Growing by: " << height_diff << endl;
	
	if (active) {
		rect.grow({ 0, height_diff }, speed);
		dest.y += height_diff;
	}
	
	
	

};

void DropDownMenu::activate() {
	if (!active) {
		cout << "activating growing by " << total_height << endl << endl;
		rect.grow({ 0,total_height }, speed);
		dest.y += total_height;
		active = true;
	}


};

void DropDownMenu::deactivate() {
	if (active) {
		cout << "deactivating shrinking by " << total_height << endl << endl;
		rect.grow({ 0,-total_height }, speed);
		dest.y += -total_height;
		active = false;
	}
	

};

void DropDownMenu::update() {
	rect.updateGrowth();
	rect.updateMove();
	div->updateMove();
	bottom_img.rect->y = std::max(rect.rect->y + rect.rect->h - bottom_size, rect.rect->y);
	bottom_img.rect->h = std::min(bottom_size, rect.rect->h);
	top_img.rect->y = rect.rect->y;
	top_img.rect->h = rect.rect->h - bottom_size;
	if (top_img.rect->h < 0) {
		top_img.rect->h = 0;
	}
};

void DropDownMenu::display() {

	//bottom_img.rect->x = rect.rect->x;



	bottom_img.display();
	top_img.display();

	//rect.display();
	if (rect.rect->h >= total_height) {
		div->display();
	}
	
};

bool DropDownMenu::enabled() {
	return active;
}