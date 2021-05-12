#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include <thread>
using namespace std;

class Image {
private:
	//Meta data
	std::string ext;
	std::string hash;
	std::string id;
	std::vector<std::string> tags;
	int size; //size in bytes


	//Image data 
	SDL_Texture* file_text;
	SDL_Texture* thumb_text;
	SDL_Surface* thumb_sur;
	SDL_Surface* file_sur;

	//Helper Functions
	void recalculateHash();
	void writeToFile(std::string str, std::string filename);
	bool fileExists(std::string path); //Check if our local file has been downloaded
	bool findCachedFile(string path);
	std::string getThumbPath();
	std::string getFilePath();
	

public:
	Image(std::string fileid, std::string type, std::vector<std::string> t, string h, int s);

	bool fullFileExists();


	void generateFile(); //Downloads the image data and metadate creates surface from this data
	void generateFileSur(); //creates surface from file data
	void generateThumb(); //Downloads the image data and metadate creates surface from this data
	void BuildFileTexture(SDL_Renderer* renderer); //Builds a Texture from the surface
	void BuildThumbTexture(SDL_Renderer* renderer); //Builds a Texture from the surface

	//Getters
	std::string getHash() { return hash; };
	std::string getExt() { return ext; };
	std::string getId() { return id; };
	std::vector<std::string> getTags();
	SDL_Texture* getFileText();
	SDL_Texture* getThumbText();

	void saveImage(std::string dir); //Saves the image to a directory as specified in the arguments.
	void DeleteImageFromHydrus(); //Tells hydrus to delete this image from the server.
	void DestroyFileTexture(); //Destroys our file texture outside of the destructor, allows for clearing GPU memory of uneeded data.
	void DestroyFileSurface(); //Emergeny call when you have created the surface but need to destroy it before a texture is made.
	~Image(); //Destroys our Surfaces, Textures and metadata
};