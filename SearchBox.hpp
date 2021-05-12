#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Tag.hpp"
#include "TextBox.hpp"


class SearchBox : public TextBox {
protected:

public:
	SearchBox(SDL_Renderer* rend);
	void update();

};
