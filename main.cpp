#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <Commdlg.h>//must be after windows.h

int main(int argc, char** argv) {
	system("chcp 65001 > nul");
	std::string name;

	std::cout << "Copyright (c) 2020 Jaclav strona: https://github.com/Jaclav/TeamsListSorter wersja: " << __DATE__ << " " << __TIME__ << '\n';

	if(argc < 2) {
		tagOFNA ofn;
		char fileName[100];
		memset(&ofn, 0, sizeof(tagOFNA));

		ofn.lStructSize = sizeof(tagOFNA);
		ofn.lpstrFile = fileName;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 100;
		ofn.lpstrFilter = "Lista Teams *.csv\0*.csv\0";
		ofn.nFilterIndex = 1;

		GetOpenFileName(&ofn);
		name = ofn.lpstrFile;
	}
	else {
		name = argv[1];
	}

	std::fstream file(name, std::ios::in);
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

		for(unsigned int i = 0; i < line.size(); i ++) {
			if(line[i] != 0) {//delete dots between chars
				//convert polish characters to ASCII
				if((unsigned int) line[i] == 4294967283) { //ó
					buff[ctr].push_back('o');
				}
				else if((unsigned int) line[i] == 4294967251) { //Ó
					buff[ctr].push_back('O');
				}
				else if(line[i] == 1 && line[i - 1] == 4) { //Ą
					buff[ctr].pop_back();
					buff[ctr].push_back('A');
				}
				else if(line[i] == 1 && line[i - 1] == 5) { //ą
					buff[ctr].pop_back();
					buff[ctr].push_back('a');
				}
				else if(line[i] == 6) { //Ć
					buff[ctr].push_back('C');
					i++;
				}
				else if(line[i] == 7) { //ć
					buff[ctr].push_back('c');
					i++;
				}
				else if(line[i] == 24) { //Ę
					buff[ctr].push_back('E');
					i++;
				}
				else if(line[i] == 25) { //ę
					buff[ctr].push_back('e');
					i++;
				}
				else if(line[i] == 65 && line[i + 1] == 1) { //Ł
					buff[ctr].push_back('L');
					i++;
				}
				else if(line[i] == 66 && line[i + 1] == 1) { //ł
					buff[ctr].push_back('l');
					i++;
				}
				else if(line[i] == 67 && line[i + 1] == 1) { //Ń
					buff[ctr].push_back('N');
					i++;
				}
				else if(line[i] == 68 && line[i + 1] == 1) { //ń
					buff[ctr].push_back('n');
					i++;
				}
				else if(line[i] == 90 && line[i + 1] == 1) { //Ś
					buff[ctr].push_back('S');
					i++;
				}
				else if(line[i] == 91) { //ś
					buff[ctr].push_back('s');
					i++;
				}
				else if((line[i] == 121 || line[i] == 123) && line[i + 1] == 1) { //Ź or Ż
					buff[ctr].push_back('Z');
					i++;
				}
				else if(line[i] == 122 || line[i] == 124) { //ź or ż
					buff[ctr].push_back('z');
					i++;
				}
				else {
					buff[ctr].push_back(line[i]);
				}
			}
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

	std::cout << "Rzędów: " << buff.size() - 1 << '\n';
	std::cout << "Aby wyjść z programu należy wcisnąć ENTER.\n";
	std::cin.get();
	return 0;
}
