#pragma once
#include "User.hpp"
#include <filesystem>
#include "Utils.hpp"

User::User() {
	string filename = "user_data.json";
	string temp;
	string buffer;
	
	namespace fs = std::filesystem;
	fs::create_directories("Saved");
	fs::create_directories("files/thumbs");



	ifstream ifile;
	ifile.open(filename);
	if (ifile) {
		cout << "settings file exists" << endl;
		ifile.close();
	}
	else {
		cout << "settings file doesn't exist" << endl;
		buildFile();
	}

	fstream afile;
	afile.open(filename);
	while (getline(afile, buffer)) {
		temp += buffer;
	}
	//cout << "Json dump" << endl;
	//cout << temp << endl;
	j = json::parse(temp);

	std::filesystem::create_directories(j["save_directory"].get<string>());

	cout << "Json dump end" << endl;
};

void  User::buildFile() {
	std::string host;
	std::string api_key;


	std::fstream file1;
	file1.open("host.txt", std::ios::in);
	if (!file1) {
		host = "12.35.49.22";
	}
	else {
		std::getline(file1, host);
	}
	file1.close();
	file1.open("key.txt", std::ios::in);
	if (!file1) {
		api_key = "974de46c1cc09019910b6373a91577a2c0f20542fe99fba740c439fad7bbe05c";
	}
	else {
		std::getline(file1, api_key);
	}


	ofstream file("user_data.json");
	j = {
	{"name", "User"},
	{"favorites", json::array()},
	{"api-key", api_key},
	{"host", host},
	{"window_width", 1920},
	{"window_height", 1080},
	{"save_directory", "saved_images"},
	{"spongebob", false},
	{"default_search", json::array({"pecorepo"})}
	};

	file << j.dump(4);
	cout << j.dump(4);
	file.close();
};



void User::saveSettings() {
	std::filesystem::create_directories(j["save_directory"].get<string>());

	ofstream file("user_data.json");
	file << j.dump(4);
};


User::~User() {
	
};