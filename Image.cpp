#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Image.hpp"
#include "HydrusCall.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <thread>

using namespace std;


Image::Image(string fileid, string type, vector<string> t , string h): id(fileid), ext(type), tags(t), hash(h), thumb_text(NULL), file_text(NULL){
	//TODO
}

bool Image::findCachedFile(string path) {
	ifstream file;

	file.open(path + id + ".png");
	if (file) {
		ext = ".png";
		return true;
	}
	file.open(path + id + ".jpg");
	if (file) {
		ext = ".jpg";
		return true;
	}
	
	if (ext == "") {
		return false;
	}
	return true;
};

void Image::recalculateHash() {
	//TODO recalculate the images hash value.
}

void Image::writeToFile(string str, string filename) {
	//Open file for writing
	ofstream file;
	file.open(filename, ios::binary);

	//Convert string to char[] blob
	int size = str.size() + 1;
	char* data;
	data = (char*)malloc(size * sizeof(char));
	str.copy(data, size);

	//Write to file and close
	file.write(data, size - 1);
	file.close();

	//Free outstanding memory
	free(data);
}

bool Image::fileExists(string path) {
	ifstream file;

	file.open(path + id + ext);
	if (file) {
		return true;
	}
	else {
		return false;
	}
}

void Image::generateFile() {
	//TODO

	//IF IMAGE EXISTS DON"T
	if (!fileExists("files/")) {
		cout << "Downloading file" << endl;
		HydrusCall hydrus(45869);
		string output = hydrus.getFileById(id);
		writeToFile(output, ("files/" + id + ext));
		return;
	}
}

void Image::generateThumb() {
	//TODO

	if (!fileExists("files/thumbs/")) {

		HydrusCall hydrus(45869);
		string output = hydrus.getThumbById(id);
		writeToFile(output, ("files/thumbs/" + id + ext));
	}
}

std::string Image::getThumbPath() {
	//TODO
	generateThumb();
	return ("files/thumbs/"+ id +  ext);
}

std::string Image::getFilePath() {
	//TODO
	generateFile();
	return ("files/" + id +  ext);
}

void Image::saveImage(std::string dir) {
	//TODO

	HydrusCall hydrus(45869);
	string output = hydrus.getFileById(id);
	writeToFile(output, (dir + "/" + id +  ext));
	return;
}

vector<string> Image::getTags() {
	return tags;
}

void Image::setFileText(SDL_Texture* texture) {
	file_text = texture;
};

void Image::setThumbText(SDL_Texture* texture) {
	thumb_text = texture;
};

SDL_Texture* Image::getFileText(){
	cout << "file text is : " <<  file_text << endl;
	if (file_text != NULL) {
		return file_text;
	}
	else {
		std::cerr << "Image tried get file texture while file texture was -> " << file_text;
		return file_text;
	}
};

SDL_Texture* Image::getThumbText() {
	if (thumb_text != NULL) {
		return thumb_text;
	}
	else {
		std::cerr << "Image tried get file texture while thumb texture was -> " << thumb_text;
		return thumb_text;
	}
};

void Image::BuildFileTexture(SDL_Renderer* renderer, int* flag) {
	SDL_Surface* sur = IMG_Load((getFilePath()).c_str());
	setFileText(SDL_CreateTextureFromSurface(renderer, sur));
	SDL_FreeSurface(sur);
	*flag = 1;
};

thread Image::BuildFileTextureThread(SDL_Renderer* renderer, int* flag) {
	return std::thread(&Image::BuildFileTexture, this, renderer, flag);
};

thread Image::getFileThread() {
	return std::thread(&Image::getFilePath, this);
};

void Image::Destroy() {
	if (thumb_text != NULL) {
		SDL_DestroyTexture(thumb_text);
	}
	if (file_text != NULL) {
		SDL_DestroyTexture(file_text);
	}
}

void Image::DeleteImageFromHydrus() {
	HydrusCall hydrus(45869);
	hydrus.deleteFile(hash);
};

Image::~Image() {
};

