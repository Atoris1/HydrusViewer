#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include <thread>
using namespace std;

class Image {
private:
	std::string ext;
	std::string hash;
	std::string id;
	SDL_Texture* file_text;
	SDL_Texture* thumb_text;
	std::vector<std::string> tags;
	int width;
	int height;
	void recalculateHash();
	void writeToFile(std::string str, std::string filename);
	bool fileExists(std::string path);
	bool findCachedFile(string path);

public:
	Image(std::string fileid, std::string type, std::vector<std::string> t, string h);
	void generateFile();
	void generateThumb();
	int getWidth() { return width; };
	int getHight() { return height; };
	std::string getHash() { return hash; };
	std::string getExt() { return ext; };
	std::string getId() { return id; };
	std::string getThumbPath();
	std::string getFilePath();
	void saveImage(std::string dir);
	std::vector<std::string> getTags();
	void setFileText(SDL_Texture* texture) ;
	void setThumbText(SDL_Texture* texture) ;
	SDL_Texture* getFileText();
	SDL_Texture* getThumbText();
	void BuildFileTexture(SDL_Renderer* renderer, int *flag);
	thread BuildFileTextureThread(SDL_Renderer* renderer, int *flag);
	thread getFileThread();
	void Destroy();
	void DeleteImageFromHydrus();
	~Image();
};