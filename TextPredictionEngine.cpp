#include "TextPredictionEngine.hpp"


TextPredictionEngine::TextPredictionEngine() {
	string filename = "data.csv";
	string buffer;

	ifstream ifile;
	ifile.open(filename);
	if (ifile) {
		cout << "Tag prediction engine has found it's data" << endl;
	}
	else {
		cerr << "Text prediction engine has no data returning" << endl;
		return;
	}

	





	while (std::getline(ifile, buffer)) {
		//cout << "Buffer is -> " << buffer << endl;

		string temp = buffer;

		int startpos = buffer.find(",") + 1;
		int endpos;
		temp = buffer.substr(startpos, buffer.size() - startpos);

		endpos = buffer.substr(startpos, buffer.size() - startpos).find(",") + startpos + 1;

		string key = buffer.substr(startpos, endpos - startpos -1 );
		string count_str = buffer.substr(endpos, buffer.size() - endpos);

		/*
		cout << "startpos is ->" << startpos << endl;
		cout << "key is ->" << key << endl;
		cout << "endpos is ->" << endpos << endl;
		cout << "count string is ->" << count_str << endl;
		*/

		int count = stoi(count_str);


		string s0(1, buffer[0]);
		if (TextVectorMap.find(s0) == TextVectorMap.end()) {
			//Not found create empty array
			TextVectorMap[s0] = std::vector<WeightedTag>();
		}
		

		//cout << "Adding data : " << key << "," << count << " To vector : " << s0 << endl;
		TextVectorMap[s0].push_back({ key, count });
		
	}

};

std::vector<WeightedTag> TextPredictionEngine::getPrediction(std::string base, int n) {
	int index = 0;
	string l0(1, base[0]);

	std::vector<WeightedTag> temp;

	if (TextVectorMap.find(l0) == TextVectorMap.end()) {
		cout << "character not found <" << l0 << ">" << endl;
		return temp;
	}
	else {
		for (int i = 0; i < TextVectorMap[l0].size(); i++) {
			if (index >= n) { break; }
			if (TextVectorMap[l0][i].tag.size() >= base.size()) {
				if (TextVectorMap[l0][i].tag.substr(0, base.size()) == base) {
					temp.push_back(TextVectorMap[l0][i]);
					index++;					
				}
			}	
		}
	}
	return temp;
}