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

class Image;

using namespace std;
using json = nlohmann::json;

class HydrusCall {
private:
	static const int DEFAULT_BUFLEN = 8192;
	int port;
	string host;
	string api_key;
	struct addrinfo* ptr;
	json j;

	SOCKET createSocket();
	void connectToSocket(SOCKET* s);
	void sendRequest(SOCKET socket, const char* msg);
	string receiveData(SOCKET socket);
	void formatJsontoImage(string& str);
	vector<string> grabExt(std::string& str);

public:
	HydrusCall(int p);
	vector<string> query(vector<string> tags);
	std::vector<Image> createImages(std::vector<std::string> strings);
	string getFileById(string id);
	string getThumbById(string id);
	void deleteFile(string hash);
	~HydrusCall();
};