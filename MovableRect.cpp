#include "MovableRect.h"
#include <math.h> 

MovableRect::MovableRect(SDL_Rect* source_rect) : rect(source_rect), dx(new float(0)), dy(new float(0)), dest({ 0,0 }) {
	inherit_velocity = false;
	if (rect) {
		dest_size = { rect->w, rect->h };
		position = { rect->x, rect->y };
		dest = { rect->x, rect->y };
		growth_rate = 1;
	}
};

void MovableRect::moveToPosition(Vector2f destination, Vector2f velocity) {
	if (!inherit_velocity) {
		if (dx) {
			free(dx);
		}
		if (dy) {
			free(dy);
		}
		dx = new float(velocity.x);
		dy = new float(velocity.y);
	}
	dest = destination;
}

void MovableRect::move(Vector2f offset, Vector2f velocity) {
	/*
	if (position.x != dest.x || position.y != dest.y) { 
		int x_diff = abs(position.x - dest.x);
		if (position.x < dest.x) {
			position.x += x_diff;
			rect->x += x_diff;
		}
		else {
			position.x -= x_diff;
			rect->x -= x_diff;
		}

		int y_diff = abs(position.y - dest.y);
		if (position.y < dest.y) {
			position.y += y_diff;
			rect->y += y_diff;
		}
		else {
			position.y -= y_diff;
			rect->y -= y_diff;
		}
	 }
	 */

	if (!inherit_velocity) {
		if (dx) {
			free(dx);
		}
		if (dy) {
			free(dy);
		}
		dx = new float(velocity.x);
		dy = new float(velocity.y);
	}
	dest = { position.x + offset.x, position.y + offset.y };

}

void MovableRect::updateMove() {

	if (*dx == 0 && *dy == 0) { return; }



	if (*dx > 0) {//If moving to the right	
		if (position.x < dest.x) {
			rect->x += *dx;
			position.x += *dx;
			if (!inherit_velocity) { *dx -= *dx / 30; }
			
		}
		else {
			int diff = position.x - dest.x;
			rect->x -= diff;
			position.x -= diff;
			if (!inherit_velocity) { *dx =0; }
		}
	}

	else if (*dx < 0) {//If moving to the left
		if (position.x > dest.x) {
			rect->x += *dx;
			position.x += *dx;
			if (!inherit_velocity) { *dx -= *dx / 30; }
		}
		else {
			int diff = position.x - dest.x;
			rect->x -= diff;
			position.x -= diff;
			if (!inherit_velocity) { *dx = 0; }
		}
	}

	if (*dy > 0) {//If moving up
		if (*dy < 4) { *dy = 0; };
		if (position.y < dest.y) {
			rect->y += floor(*dy);
			position.y += floor(*dy);
			if (!inherit_velocity) { *dy -= *dy / 30; }
		}
		else {
			int diff = position.y - dest.y;
			rect->y -= diff;
			position.y -= diff;
			if (!inherit_velocity) { *dy = 0; }
		}
	}

	else if (*dy < 0) {//If moving down
		if (*dy > -4) { *dy = 0; };
		if (position.y > dest.y) {
			rect->y += floor(*dy);
			position.y += floor(*dy);
			if (!inherit_velocity) { *dy -= *dy / 30; }
		}
		else {
			int diff = position.y - dest.y;
			rect->y -= diff;
			position.y -= diff;
			if (!inherit_velocity) { *dy = 0; }
		}
	}

};

bool MovableRect::handleClick(Vector2f mousepos, bool circle) {
	if (circle) {
		return circleColision(*rect, mousepos);
	}
	return (rectangleColision(*rect, mousepos));

};

void MovableRect::setRect(SDL_Rect* r) {
	if (rect) {
		delete rect;
	};
	rect = r;
	dest_size = { rect->w, rect->h };
	position = { rect->x, rect->y };
	dest = { rect->x, rect->y };
};

void MovableRect::setPos(Vector2f v) {
	rect->x = v.x;
	rect->y = v.y;
	position.x = v.x;
	position.y = v.y;
	dest.x = v.x;
	dest.y = v.y;
};

void MovableRect::inheritVelocity(float* x, float* y) {
	dx = x;
	dy = y;
	inherit_velocity = true;
	return;
};

void MovableRect::removeVelocity() {
	dx = NULL;
	dy = NULL;
	inherit_velocity = false;
};

void MovableRect::grow(Vector2f offset, int interval) {
	if ((rect->w != dest_size.x || rect->h != dest_size.y)) {
		growth_rate = std::abs(rect->w - dest_size.x);
		updateGrowth();
	}


	dest_size.x = rect->w + offset.x;
	dest_size.y = rect->h + offset.y;



	growth_rate = interval;

	if (growth_rate % 2 != 0) {
		growth_rate++; //DO not let people use a uneven growthrate.
	}

	

};

void MovableRect::updateGrowth() {
	if (rect->w == dest_size.x && rect->h == dest_size.y) { return; }

	cout << "before movement w,h is -> " << rect->w << "," << rect->h << endl;


	x_movement = updateSizeInt(&rect->w, dest_size.x);
	y_movement = updateSizeInt(&rect->h, dest_size.y);

	if (grow_in_place) {
		rect->x -= x_movement / 2;
		rect->y -= y_movement / 2;
	}




	cout << "New w,h is -> " << rect->w << "," << rect->h << endl;
	//cout << "moved <" << x_movement/2 << "> left" << endl;
	//cout << "moved <" << y_movement/2 << "> up" << endl;

	/*Fuck trying to fix this, if you move a shape on an uneven number it's your fault just move it an even number holy shit
	I tried thinking about how to fix this and it involves a state machine with 3 states for each direction the state will hold if 
	the shape needs to be offset the next iteration IF there is an uneven step again.

	Hell you can move things to an uneven coordinate just for the love of god don't use a fucking non-even step time
	
	
	if (x_movement % 2 != 0) {
		if (x_offset == LEFT) {
			rect->x--;
		}
		else if (x_offset == RIGHT) {
			//Do nothing
		}
	}

	
	if (y_movement % 2 != 0) {
		if (y_offset == LEFT) {
			rect->y--;
		}
		else if (y_offset == RIGHT) {
			//Do nothing
		}
	}
	*/

	

};

int MovableRect::updateSizeInt(int* c, int& d) {
	int temp = NOMOVEMENT;
	if (*c != d) {
		if (*c < d) {
				*c += growth_rate;
				temp = growth_rate;
			if (*c >= d) {
				temp = temp - (*c - d); // IDK DUDE
				*c = d;
			}
		}
		else {
			if (*c - growth_rate < 0) {
				*c = 0;
			}
			else {
				*c -= growth_rate;
				temp = -growth_rate;
			}
			if (*c <= d || *c <= 0) {
				temp = temp - (*c - d); //IDK DUDE
				*c = d;
			}
		}
	}
	return temp;
}

//Statement does nothing but needed incase of drawing an empty rectangle
void MovableRect::display(bool box) {
	cout << "no display is being called" << endl;
};

int MovableRect::getWidth() {
	return rect->w;
}

int MovableRect::getHeight() {
	return rect->h;
}

MovableRect::~MovableRect() {
	if (!inherit_velocity) {
		if (dx) {
			free(dx);
		}
		if (dy) {
			free(dy);
		}
	}
	delete rect;
};