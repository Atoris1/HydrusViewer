#pragma once
#include "RadioButton.h"


RadioButton::RadioButton(SDL_Renderer* r, Vector2f o, int s) : renderer(r), ball(new SDL_Rect{o.x, o.y - s/6, s, s }, r), bar(new SDL_Rect{ o.x, o.y, s*2, s - s/3}, r), origin(o), size(s), ghost_ball(new SDL_Rect{ o.x-s/2, o.y - s / 6 - s/2, s*2, s*2 }, r) {
	ball.setImage("radio_ball.png");
	ghost_ball.setImage("ghost_ball.png", 0);
	bar.setImage("radio_bar.png");
	active_bool = false;

};

int RadioButton::getWidth() {
	return size * 2;
};
int RadioButton::getHeight() {
	return size ;
};

void RadioButton::setPos(Vector2f pos) {

	if (!active()) {
		ball.setPos({ pos.x ,pos.y  });
		bar.setPos({pos.x, pos.y + size / 6 });
		ghost_ball.setPos({ pos.x - size/2 ,pos.y - size / 2 });
	}
	else {
		ball.setPos({ pos.x+size ,pos.y });
		bar.setPos({ pos.x, pos.y + size / 6 });
		ghost_ball.setPos({ pos.x +size - size / 2 ,pos.y - size / 2 });
	
	}
	origin = pos;

}

bool RadioButton::active() {
	return active_bool;
};

bool RadioButton::handleClick(Vector2f mouse_pos) {
	if (ball.handleClick(mouse_pos, true)) {
		if (!active()) {
			enable();
		}
		else {
			disable();
		}

		return true;
	}
	else {

		return false;
	}
	
};

void RadioButton::display() {




	bar.display();
	ball.display();
	ghost_ball.display();
	
};

void RadioButton::updateMovement() {
	ball.updateMove();
	bar.updateMove();
	ball.updateColor();
	bar.updateColor();
	ball.updateGrowth();
	ghost_ball.updateMove();
	ghost_ball.updateColor();
	ghost_ball.updateGrowth();
};

void RadioButton::move(Vector2f offset, Vector2f velocity) {
	ball.move(offset, velocity);
	ghost_ball.move(offset, velocity);
	bar.move(offset, velocity);
}

void RadioButton::setState(bool state) {
	if (state) {
		if (!active()) {
			ball.move({ size,0 }, { size / 10, 0 });
			ghost_ball.move({ size,0 }, { size / 10, 0 });
		}

		ball.setColorMod({ 40,230,40 }, 8);
		bar.setColorMod({ 140,230,140 }, 8);




		ghost_ball.setAlphaMod(180);
		ghost_ball.updateColor();
		ghost_ball.setAlphaMod(0, 2);
		ghost_ball.setColorMod({ 140,230,140 }, 8);
		active_bool = true;
	}
	else {
		if (active()) {
			ball.move({ -size,0 }, { -size / 10, 0 });
			ghost_ball.move({ -size,0 }, { -size / 10, 0 });
		}

		ball.setColorMod({ 230,40,40 }, 8);
		bar.setColorMod({ 230,140,140 }, 8);

		ghost_ball.setAlphaMod(180);
		ghost_ball.updateColor();
		ghost_ball.setAlphaMod(0, 2);
		ghost_ball.setColorMod({ 230,140,140 }, 8);

		active_bool = false;
	}
};

void RadioButton::disable() {
	if (active()) {
		ball.move({ -size,0 }, { -size / 10, 0 });
		ghost_ball.move({ -size,0 }, { -size / 10, 0 });
		ghost_ball.grow({ -50,-50 }, 50);
		ghost_ball.updateGrowth();
		ghost_ball.grow({ 50,50 }, 1);
	
	}

	ball.setColorMod({ 230,40,40 }, 8);
	bar.setColorMod({ 230,140,140 }, 8);

	ghost_ball.setAlphaMod(180);
	ghost_ball.updateColor();
	ghost_ball.setAlphaMod(0, 2);
	ghost_ball.setColorMod({ 230,140,140 }, 8);

	active_bool = false;
};

void RadioButton::enable() {
	if (!active()) {
		ball.move({ size,0 }, { size / 10, 0 });
		ghost_ball.move({ size,0 }, { size / 10, 0 });
		ghost_ball.grow({ -50,-50 }, 50);
		ghost_ball.updateGrowth();
		ghost_ball.grow({ 50,50 }, 1);
	}

	ball.setColorMod({ 40,230,40 }, 8);
	bar.setColorMod({ 140,230,140 }, 8);
	


	
	ghost_ball.setAlphaMod(180);
	ghost_ball.updateColor();
	ghost_ball.setAlphaMod(0, 2);
	ghost_ball.setColorMod({ 140,230,140 }, 8);
	active_bool = true;
};