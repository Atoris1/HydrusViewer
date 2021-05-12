#define WIN32_LEAN_AND_MEAN
#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <json.hpp>
#include "User.hpp"

class Image;

using namespace std;
using json = nlohmann::json;

class HydrusCall {
private:
	static const int DEFAULT_BUFLEN = 8192;
	int port;
	static string host;
	static string api_key;
	static User* user;
	struct addrinfo* ptr;
	json j;

	SOCKET createSocket();
	void connectToSocket(SOCKET* s);
	void sendRequest(SOCKET socket, const char* msg);
	string receiveData(SOCKET socketint, int size = 1000000);
	void formatJsontoImage(string& str);
	vector<string> grabExt(std::string& str);

public:
	HydrusCall(int p);
	HydrusCall(User* u);
	vector<string> query(vector<string> tags);
	std::vector<Image*> createImages(std::vector<std::string> strings);
	string getFileById(string id, int size = 1000000);
	string getThumbById(string id);
	void deleteFile(string hash);

	~HydrusCall();
};