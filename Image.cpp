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


Image::Image(string fileid, string type, vector<string> t , string h, int s): id(fileid), ext(type), tags(t), hash(h), thumb_text(NULL), file_text(NULL), thumb_sur(NULL), file_sur(NULL), size(s+1000){
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
	if (!fileExists("files/")) {
		cout << "Downloading file" << endl;
		HydrusCall hydrus(45869);
		string output = hydrus.getFileById(id, size);
		writeToFile(output, ("files/" + id + ext));
	}

	//Preloading our surface for fast texture creation
	//generateFileSur();

}

void Image::generateThumb() {
	if (!fileExists("files/thumbs/")) {

		HydrusCall hydrus(45869);
		string output = hydrus.getThumbById(id);
		writeToFile(output, ("files/thumbs/" + id + ext));
	}

	//Preloading our surface for fast texture creation
	thumb_sur = IMG_Load((getThumbPath()).c_str());
}

std::string Image::getThumbPath() {
	if (!fileExists("files/thumbs/")) {
		cerr << "File does not exist for given path -> " << "files/thumbs/" + id + ext << endl;
	}
	return ("files/thumbs/"+ id +  ext);
}

std::string Image::getFilePath() {
	if (!fileExists("files/")) {
		cerr << "File does not exist for given path -> " << "files/" + id + ext << endl;
	}
	return ("files/" + id +  ext);
}

void Image::saveImage(std::string dir) {
	//TODO

	HydrusCall hydrus(45869);
	string output = hydrus.getFileById(id, size);
	writeToFile(output, (dir + "/" + id +  ext));
	return;
}

vector<string> Image::getTags() {
	return tags;
}

SDL_Texture* Image::getFileText(){
	if (file_text != NULL) {
		return file_text;
	}
	else {
		std::cerr << "Image tried get file texture while file texture was -> " << file_text << endl;
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

void Image::BuildFileTexture(SDL_Renderer* renderer) {
	cout << "Surface before creating texture is -> " << file_sur << endl;
	file_text = (SDL_CreateTextureFromSurface(renderer, file_sur));
	SDL_FreeSurface(file_sur);
	file_sur = NULL;
	cout << "texture created ! -> " << file_text << endl;
};

void Image::BuildThumbTexture(SDL_Renderer* renderer) {
	thumb_text = (SDL_CreateTextureFromSurface(renderer, thumb_sur));
	SDL_FreeSurface(thumb_sur);
	thumb_sur = NULL;
};

void Image::DestroyFileTexture() {
	if (file_text != NULL) {
		SDL_DestroyTexture(file_text);
	}
	file_text = NULL;
	if (file_sur != NULL) {
		SDL_FreeSurface(file_sur);
	}
	file_sur = NULL;
};

void Image::generateFileSur() {
	//Preloading our surface for fast texture creation
	file_sur = IMG_Load((getFilePath()).c_str());
	cout << "file surface after creation is -> " << file_sur << endl;

}

void Image::DestroyFileSurface() {
	if (file_sur != NULL) {
		SDL_FreeSurface(file_sur);
	}
	file_sur = NULL;
};

bool Image::fullFileExists() {
	ifstream file;

	file.open("files/" + id + ext);
	if (file) {
		return true;
	}
	else {
		return false;
	}

};

void Image::DeleteImageFromHydrus() {
	HydrusCall hydrus(45869);
	hydrus.deleteFile(hash);
};

Image::~Image() {
	if (thumb_text != NULL) {
		SDL_DestroyTexture(thumb_text);
	}
	if (file_text != NULL) {
		SDL_DestroyTexture(file_text);
	}
	if (thumb_sur != NULL) {
		SDL_FreeSurface(thumb_sur);
	}
	if (file_sur != NULL) {
		SDL_FreeSurface(file_sur);
	}

	cout << "Image destructor called sucessfully." << endl;
};

