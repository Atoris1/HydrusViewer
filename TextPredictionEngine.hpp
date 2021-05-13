#pragma once
#include "Utils.hpp"
#include "json.hpp"

struct WeightedTag {
	std::string tag;
	int weight;
};


using json = nlohmann::json;

class TextPredictionEngine {
private:
	json j;
	std::map<std::string, std::vector<WeightedTag>> TextVectorMap;


public:
	TextPredictionEngine();
	std::vector<WeightedTag> getPrediction(std::string base, int n); //Gets a n number of predictions with 


};
