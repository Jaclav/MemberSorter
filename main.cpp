#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
	system("chcp 65001 > nul");

	if(argc < 2) {
		std::cout << "Należy podać ścieżkę pliku .csv!\n";
		system("pause");
		return -1;
	}

	std::fstream file(argv[1], std::ios::in);
	if(!file.good()) {
		std::cout << "Plik nie istnieje!\n";
		system("pause");
		return -1;
	}
	std::vector<std::string>buff;

	std::string line;
	getline(file, line);//Ignore information about columns
	getline(file, line);//Ignore information about teacher

	for(unsigned int ctr = 0; getline(file, line); ctr++) {
		buff.push_back("");

		//delete dots between chars
		for(unsigned int i = 0; i < line.size(); i ++) {
			if(line[i] != 0)
				buff[ctr].push_back(line[i]);
		}
	}
	file.close();

	for(unsigned int i = 0; i < buff.size(); i++) {
		if(buff[i][0] < 58) {
			buff[i] = buff[i].substr(buff[i].find(" ") + 1);
		}
		buff[i] = buff[i].substr(buff[i].find(" ") + 1) + "\t" + buff[i].substr(0, buff[i].find(" "));
	}

	std::sort(buff.begin(), buff.end());

	for(std::vector<std::string>::iterator it = buff.begin(); it != buff.end(); it++) {
		std::cout << *it << '\n';
	}

	std::cout << "Łącznie:" << buff.size() - 1 << '\n';
	system("pause");
	return 0;
}
