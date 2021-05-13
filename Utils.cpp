#pragma once

#include "Utils.hpp"
#include <thread>
#include <chrono>



std::vector<std::string> createTagsFromString(std::string& str) {
	std::vector<std::string> strings;
	std::string delim = " ";
	str = str + " ";
	std::cout << "Starting string <" << str << ">" << std::endl;

	size_t pos = 0;
	int lastpos = -1;
	while ((pos = str.find(" ", pos + 1)) != std::string::npos) {
		strings.push_back(str.substr(lastpos + 1, pos - lastpos-1));
		std::cout << "tag added <" << str.substr(lastpos + 1, pos - lastpos-1) << ">" << std::endl;
		lastpos = pos;
	}
	return strings;
};

bool rectangleColision(SDL_Rect& rect, Vector2f& pos) {
	if (pos.y >= rect.y && pos.y <= rect.y + rect.h) {
		if (pos.x >= rect.x && pos.x <= rect.x + rect.w) {
			return true;
		}
	}
	return false;
}

bool circleColision(SDL_Rect& rect, Vector2f& pos) {
	int radius = rect.w / 2;
	int x1 = pos.x;
	int x2 = rect.x + radius;
	int y1 = pos.y;
	int y2 = rect.y + radius;

	float distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

	return distance < radius;
};

#include "ViewImage.cpp"

void threaded_image_destruction(ViewImage* viewimage) {
	delete viewimage;
}


#include "Grid.hpp"
#include "TextBox.hpp"
#include "User.hpp"

bool vectContains(std::vector<int> &vect, int item) {
	if (std::find(vect.begin(), vect.end(), item) != vect.end()) {
		return true;
	}
	return false;
}




bool callImageDelete(TextBox* notification_box, Grid* grid, bool focus) {
	if (focus) {
		grid->GetFocusImage()->DeleteImageFromHydrus();
		notification_box->setTag("Delete succeeded!");
	}
	else {
		notification_box->setTag("Delete failed!");
	}
	notification_box->autoRect();
	notification_box->moveToPosition({ 1920 / 2 - 200, 1080 - 300 }, { -25,-25 }, 0); //move to mid screen
	notification_box->moveToPosition({ 1920 / 2 - 200, 1080 }, { 25, 25 }, 2); // move to bottom of screen after 2 sec delay
};

bool callImageSave(TextBox* notification_box, Grid* grid, bool focus, User* u) {
	if (focus) {
		grid->GetFocusImage()->saveImage(u->j["save_directory"].get<string>());
		notification_box->setTag("Image saved succeeded!");
	}
	else {
		notification_box->setTag("Save failed!");
	}
	notification_box->autoRect();
	notification_box->moveToPosition({ 1920 / 2 - 200, 1080 - 300 }, { -25,-25 }, 0); //move to mid screen
	notification_box->moveToPosition({ 1920 / 2 - 200, 1080 }, { 25, 25 }, 2); // move to bottom of screen after 2 sec delay
}



bool callImageFavorite(TextBox* notification_box, Grid* grid, bool focus, User* u) {
	int id = std::stoi(grid->GetFocusImage()->getId());
	if (focus) {
		std::vector<int> temp = u->j["favorites"].get<std::vector<int>>();
		std::vector<int>::iterator it = std::find(temp.begin(), temp.end(), id);

		int index = std::distance(temp.begin(), it);
		if (vectContains(temp, id)) {
	
			u->j["favorites"].erase(index);


			notification_box->setTag("Image Removed from favorites!");
		}
		else {
			u->j["favorites"].push_back(std::stoi(grid->GetFocusImage()->getId()));
			notification_box->setTag("Image Added to favorites!");
		}

		
		u->saveSettings();
		
	}
	else {
		notification_box->setTag("Image favorite failed!");
	}
	notification_box->autoRect();
	notification_box->moveToPosition({ 1920 / 2 - 200, 1080 - 300 }, { -25,-25 }, 0); //move to mid screen
	notification_box->moveToPosition({ 1920 / 2 - 200, 1080 }, { 25, 25 }, 2); // move to bottom of screen after 2 sec delay
}

void callBufferClear(std::vector<ViewImage**> viewimages) {
	for (auto& v : viewimages) {
		cout << "Buffer memory location is -> " << *v << endl;
		if (*v) {
			delete *v;
			*v = NULL;

		}
		cout << "Buffer memory location after clear is -> " << *v << endl;
	}
};

void callErrorMessage(TextBox* notification_box, string message) {};


std::vector<string> underscoresToSpaces(std::vector<string> input) {
	std::vector<string> temp = input;
	for (auto& tag : temp) {
		int pos = 0;
		while ((pos = tag.find("_", pos)) != string::npos) {
			tag.replace(pos, 1, " ");
			pos += 1;
		}
	}
	return temp;
}

string underscoresToSpaces(string input) {
	string temp = input;
	int pos = 0;
	while ((pos = temp.find("_", pos)) != string::npos) {
		temp.replace(pos, 1, " ");
		pos += 1;
	}
	return temp;
}

string spaceToUnderscore(string input) {
	string temp = input;
	int pos = 0;
	while ((pos = temp.find(" ", pos)) != string::npos) {
		temp.replace(pos, 1, "_");
		pos += 1;
	}
	return temp;


};