#define WIN32_LEAN_AND_MEAN
#pragma once

#include <chrono>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <regex>
#include <sstream>
#include <vector>
#include <thread>
#include "Image.hpp"
#include "HydrusCall.hpp"
#include <typeinfo>
#include <algorithm>



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
using namespace std;

std::string HydrusCall::host = "";
std::string HydrusCall::api_key = "";
User* HydrusCall::user = NULL;


HydrusCall::HydrusCall(int p) {
	port = p;
	int iResult;

	//cout << "host is : " << host << endl;
	//cout << "key is : " << api_key << endl;

	//Init winsock
	WSAData wsaData;
	WORD DDLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DDLLVersion, &wsaData) != 0) {
		cout << "Socket initilzation failure" << endl;
	}

	//Find addr info
		//Create Socket
	struct addrinfo* result = NULL;
	struct addrinfo hints;

	std::ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;



	// Resolve the server address and port
	iResult = getaddrinfo(host.c_str(), "45869", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
	}

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

};

HydrusCall::HydrusCall(User* u) {
	host = u->j["host"].get<string>();
	api_key = u->j["api-key"].get<string>();
	user = u;

	int iResult;


	//Init winsock
	WSAData wsaData;
	WORD DDLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DDLLVersion, &wsaData) != 0) {
		cout << "Socket initilzation failure" << endl;
	}

	//Find addr info
	//Create Socket
	struct addrinfo* result = NULL;
	struct addrinfo hints;

	std::ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	SOCKET ConnectSocket = INVALID_SOCKET;
	


	while (ConnectSocket == INVALID_SOCKET) {
		while (ptr == NULL) {
			// Resolve the server address and port
			iResult = getaddrinfo(host.c_str(), "45869", &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed: %d\n", iResult);
				WSACleanup();

			}

			// Attempt to connect to the first address returned by
			// the call to getaddrinfo
			ptr = result;
			if (ptr == NULL) {
				cerr << "Host not at location <" << host.c_str() << "> found! \nPlease enter a new host : " << endl;
				std::cin >> host;
				WSACleanup();
				if (WSAStartup(DDLLVersion, &wsaData) != 0) {
					cout << "Socket initilzation failure" << endl;
				}
				user->j["host"] = host;
				user->saveSettings();
			}
		}
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		cout << "Attempting connection to socket (20 seconds)" << endl;
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			cout << "Host is invalid : " << host << endl;
			WSACleanup();
			ptr = NULL;
			host = "invalid";
			ConnectSocket = INVALID_SOCKET;
			if (WSAStartup(DDLLVersion, &wsaData) != 0) {
				cout << "Socket initilzation failure" << endl;
			}
		}
	}



	cout << "socket connection established" << endl;
};

//Creates a socket object
SOCKET HydrusCall::createSocket() {
	int iResult;

	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	//Check for socket creation
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	return ConnectSocket;
}

//Connects to a socket object
void HydrusCall::connectToSocket(SOCKET* s) {

	int iResult = connect(*s, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "Connection to socket failed" << endl;
		cerr << "!Warning fatal error socket connection has failed" << endl;
		system("pause");
		closesocket(*s);
		*s = INVALID_SOCKET;
	}
	return;
}

//Sends a get/post request to currently connected web socket.
void HydrusCall::sendRequest(SOCKET socket, const char* msg) {

	//Check for an active socket connection
	if (socket == INVALID_SOCKET) {
		printf("No server connected to socket, could not send message!\n");
		WSACleanup();
		return;
	}

	//Send data to open socket.
	int iResult = send(socket, msg, (int)strlen(msg), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "cannot send data" << endl;
	};
	//printf("Bytes Sent: %ld\n", iResult);

};

//Receives the data from a socket and outputs it into a string unformatted after a get/post request.
std::string HydrusCall::receiveData(SOCKET socket, int size) {
	//Result variable
	int iResult;

	//Create buffer for receiving data
	char szBuffer[DEFAULT_BUFLEN];
	cout << "Mallocing " << size << " data." << endl;
	char* data = (char*)malloc(sizeof(char)* size);
	if (data == NULL) {
		cout << "MALLOC FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl << endl << endl << endl;
	}
	memset(&szBuffer, 0, DEFAULT_BUFLEN);
	int totalbytes = 0;
	// Receive until the peer closes the connection
	int i= 0;
	do {
		iResult = recv(socket, szBuffer, DEFAULT_BUFLEN, 0);
		//printf("recieve result is : %d\n", iResult);
		if (iResult > 0) {
				memcpy(data + totalbytes,  &szBuffer, iResult);
			totalbytes += iResult;
		}
		else if (iResult == 0) {
			//printf("Connection closed\n");
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
		}
		

		//On first data connection get the content size and realloc
		if (totalbytes == iResult) {
			string s(szBuffer, iResult);
			int new_size = getContentLength(s);
			free(data);
			data = NULL;
			cout << "Data memory location after free : " << (void*)data << endl;
			data = (char*)malloc(sizeof(char) * new_size);
			cout << "Re-malloced to size : " << new_size << endl;
			cout << "Data memory location : " << (void*)data << endl;
			//Copying buffer into new pointer
			memcpy(data, &szBuffer, iResult);
		}

		memset(&szBuffer, 0, DEFAULT_BUFLEN);
		i++;
	} while (iResult > 0);
	cout << "totaldata : " << totalbytes << endl;
	//std::this_thread::sleep_for(std::chrono::seconds(2));
	data[totalbytes+1] = NULL;
	string output(data, totalbytes);

	free(data);
	//Return string with data
	return output;
};

//Formats a get requests json data into just the binary blob containing image data.
void HydrusCall::formatJsontoImage(std::string& str) {

	if (str.size() == 0) {
		return;
	}
	//Regex search for splice point
	smatch m;
	regex e("Content-Length: (\\d*)");
	std::regex_search(str, m, e);

	//set string equal to substring that is starting from the index of the cut to the number of characters after the cut
	str = str.substr(str.size() - stoi(m.str(1)), stoi(m.str(1)));

}

int HydrusCall::getContentLength(std::string& str) {
	if (str.size() == 0) {
		return 0;
	}
	//Regex search for splice point
	smatch m;
	regex e("Content-Length: (\\d*)");
	std::regex_search(str, m, e);
	return (stoi(m.str(1)) + 1000);
};


//Grabs extenion from unparsed json
vector<string> HydrusCall::grabExt(std::string& str) {
	vector<string> extensions;
	if (str.size() == 0) {
		//TODO ERROR HANDLING
	}

	//Regex search for splice point
	smatch m;
	regex e("t\":");


	for (auto it = std::sregex_iterator(str.begin(), str.end(), e); it != std::sregex_iterator(); ++it)
	{
		extensions.push_back(str.substr(it->position()+ 8, 4));
	}

	return extensions;
	
}

vector<string> HydrusCall::query(vector<string> tags) {

	cout << "query tags are :\n";
	//Create query string
	string query;
	query = "%5B";
	for (auto& tag : tags) {
		cout << "<" << tag<< "> ";
		query = query + "%22" + tag + "%22" + "%2C";
	}
	cout << endl;
	query = query.substr(0, query.size() - 3);
	query = query + "%5D";



	//Pos is the position in the string to start the search
	size_t pos= 0;
	while ((pos = query.find(" ", pos)) != string::npos) {
		query.replace(pos, 1, "%20");
		pos += 3;
	}
	pos = 0;
	while ((pos = query.find("_", pos)) != string::npos) {
		query.replace(pos, 1, "%20");
		pos += 3;
	}

	pos = 0;
	while ((pos = query.find(":", pos)) != string::npos) {
		query.replace(pos, 1, "%3A");
		pos += 3;
	}


	//Put together query with get request
	query = "GET /get_files/search_files?tags=" + query + "&Hydrus-Client-API-Access-Key=" + api_key + " /\r\n\r\n";

	cout << "query \n" << query << endl;

	//Convert string into an array char
	int size = query.size();
	char* data;
	data = (char*)malloc(size * sizeof(char));
	query.copy(data, size);


	//Creating and connecting to socket
	SOCKET socket = createSocket();
	connectToSocket(&socket);

	//Send request and receivce output to a string
	sendRequest(socket, data);
	string output = receiveData(socket);

	cout << "output : \n" << output << endl;

	//Format return string
	formatJsontoImage(output);
	output = output.substr(14, output.size()-16);

	cout << "output : \n" << output << endl;


	if (output.size() <= 0) {
		cout << "ERROR String does not exist" << endl;
	};
	//Format results into a vector
	string tmp;
	vector<string>fileIds;
	stringstream ss(output);
	while (getline(ss, tmp, ',')) {
		if (tmp[0] == ' ') {
			tmp = tmp.substr(1, tmp.size() - 1);
		}
		fileIds.push_back(tmp);
	}

	std::reverse(fileIds.begin(), fileIds.end());

	cout << " Number of files for search is : " << fileIds.size() << endl;
	free(data);
	closesocket(socket);
	return fileIds;
}
	
std::vector<Image*> HydrusCall::createImages(std::vector<std::string> strings) {
	vector<Image*> images;
	cout << "Gathering image metatdata" << endl;
	if (strings.size() == 0)
	{
		cout << "No files to create" << endl;
		return images;
	}
	
	//Create query string
	string query;
	query = "%5B";
	for (auto& s : strings) {
		query = query + s  + "%2C";
	}
	query = query.substr(0, query.length() - 3);
	query = query + "%5D";

	//Put together query with get request
	query = "GET /get_files/file_metadata?file_ids=" + query + "&detailed_url_information=false&Hydrus-Client-API-Access-Key=" + api_key + " / \r\n\r\n";

	//Convert string into an array char
	int size = query.size();
	char* data;
	data = (char*)malloc(size * sizeof(char));
	query.copy(data, size);

	//Creating and connecting to socket
	SOCKET socket = createSocket();
	connectToSocket(&socket);

	//Send request and receivce output to a string
	sendRequest(socket, data);
	string output = receiveData(socket);
	free(data);

	formatJsontoImage(output);

	cout << "parsing json" << endl;
	j = json::parse(output);
	cout << "Done parsing Json" << endl;
	

	//Document d;
	//d.Parse(output.c_str());
	//cout << d["metadata"][0]["service_names_to_statuses_to_display_tags"]["my tags"]["0"].GetArray.() << endl;


	for (int i = 0; i < strings.size(); i++) {
		//cout << j["metadata"][i]["size"].get<int>() << endl << endl;;
		//cout << j["metadata"][i]["service_names_to_statuses_to_display_tags"]["my tags"] << endl << endl;
		//cout << j["metadata"][i]["service_names_to_statuses_to_display_tags"]["my tags"]["0"] << endl << endl;
		vector<string> tags;


		if (!j["metadata"][i]["service_names_to_statuses_to_display_tags"]["my tags"]["0"].is_null()) {
			tags = j["metadata"][i]["service_names_to_statuses_to_display_tags"]["my tags"]["0"].get<std::vector<std::string>>();
		}

		for (auto& tag : tags) {
			int pos = 0;
			while ((pos = tag.find(" ", pos)) != string::npos) {
				tag.replace(pos, 1, "_");
				pos += 1;
			}
		}
		std::sort(tags.begin(), tags.end());

		images.push_back(new Image(to_string(j["metadata"][i]["file_id"].get<int>()), j["metadata"][i]["ext"].get<std::string>(), tags, j["metadata"][i]["hash"].get<string>(), j["metadata"][i]["size"].get<int>()));
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	j.clear();

	cout << "Done gathering image metadata" << endl;

	return images;
}

string HydrusCall::getFileById(string id, int size) {
	//Creating and connecting to socket
	SOCKET socket = createSocket();
	connectToSocket(&socket);

	//TODO create message to send
	string query = "GET /get_files/file?file_id=" + id +"&Hydrus-Client-API-Access-Key=" + api_key + " / \r\n\r\n";

	//Convert string into an array char
	int size_temp = query.size();
	char* data;
	data = (char*)malloc(size_temp * sizeof(char));
	query.copy(data, size_temp);

	//Send request and receivce output to a string
	sendRequest(socket, data);
	string output = receiveData(socket, size);
	free(data);
		
	//Format image data accordingly
	formatJsontoImage(output);

	closesocket(socket);
	return output;
}

string HydrusCall::getThumbById(string id) {
	//Creating and connecting to socket
	SOCKET socket = createSocket();
	connectToSocket(&socket);

	//TODO create message to send
	string query = "GET /get_files/thumbnail?file_id=" + id + "&Hydrus-Client-API-Access-Key=" + api_key + " / \r\n\r\n";

	//Convert string into an array char
	int size = query.size();
	char* data;
	data = (char*)malloc(size * sizeof(char));
	query.copy(data, size);

	//Send request and receivce output to a string
	sendRequest(socket, data);
	string output = receiveData(socket);
	free(data);

	//Format image data accordingly
	formatJsontoImage(output);

	closesocket(socket);
	return output;
}

void HydrusCall::deleteFile(string hash) {

	json j;
	 j["hash"] = hash;
	 j["Hydrus-Client-API-Access-Key"] = api_key;


	string query = "POST /add_files/delete_files? HTTP/1.1\r\n";
	query += "Content-Type: application/json\r\n";
	query += "Content-Length: " + to_string(j.dump().length())+ "\r\n\r\n";
	query += j.dump();

	cout << query << endl;

	//Convert string into an array char
	int size = query.size();
	char* data;
	data = (char*)malloc(size * sizeof(char));
	query.copy(data, size);


	//Creating and connecting to socket
	SOCKET socket = createSocket();
	connectToSocket(&socket);

	//Send request and receivce output to a string
	sendRequest(socket, data);
	//string output = receiveData(socket);


	//Format return string
	//cout << "Deleted image with message :" << endl << output << endl;

};


HydrusCall::~HydrusCall(){
	freeaddrinfo(ptr);
	WSACleanup();
}
