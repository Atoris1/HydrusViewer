#pragma once
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class User {
private:

	void buildFile();

public:
	json j;

	User();

	void saveSettings();

	~User();
};