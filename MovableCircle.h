#pragma once
#include "MovableRect.h"
class MovableCircle :
	public MovableRect
{

	bool handleClick(Vector2f mousepos) {
		return circleColision(*rect, mousepos);
	};

};

