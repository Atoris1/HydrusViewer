#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "Utils.hpp"

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