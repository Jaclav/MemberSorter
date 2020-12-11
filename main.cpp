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

	std::cout << "Copyright (c) 2020 Jaclav Strona: https://github.com/Jaclav/TeamsListSorter\n";

	if(argc < 2) {
		tagOFNA ofn;

		const int maxInt = 100;

		char fileName[maxInt];

		memset(&ofn, 0, sizeof(tagOFNA));

		ofn.lStructSize = sizeof(tagOFNA);
		ofn.lpstrFile = fileName;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = maxInt;
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

		//delete dots between chars
		for(unsigned int i = 0; i < line.size(); i ++) {
			if(line[i] != 0) {
				//convert polish characters to ASCII
				switch((unsigned int)line[i]) {
					case  4294967283: {//ó
						buff[ctr].push_back('o');
						break;
					}
					case 4294967251: {//Ó
						buff[ctr].push_back('O');
						break;
					}
					case 1: {//ą
						if(line[i - 1] == 4) { //Ą
							buff[ctr].pop_back();
							buff[ctr].push_back('A');
							break;
						}
						else if(line[i - 1] == 5) { //ą
							buff[ctr].pop_back();
							buff[ctr].push_back('a');
							break;
						}
					}
					case 6: {//ć
						buff[ctr].push_back('C');
						i++;
						break;
					}
					case 7: {//ć
						buff[ctr].push_back('c');
						i++;
						break;
					}
					case 24: {//Ę
						buff[ctr].push_back('E');
						i++;
						break;
					}
					case 25: {//ę
						buff[ctr].push_back('e');
						i++;
						break;
					}
					case 65: {//Ł
						if(line[i + 1] == 1) {
							buff[ctr].push_back('L');
							i++;
						}
						else {
							buff[ctr].push_back(line[i]);
						}
						break;
					}
					case 66: {//ł
						if(line[i + 1] == 1) {
							buff[ctr].push_back('l');
							i++;
						}
						else {
							buff[ctr].push_back(line[i]);
						}
						break;
					}
					case 68: {//ń
						if(line[i + 1] == 1) {
							buff[ctr].push_back('n');
							i++;
						}
						else {
							buff[ctr].push_back(line[i]);
						}
						break;
					}
					case 90: {
						if(line[i + 1] == 1) {
							buff[ctr].push_back('S');
							i++;
						}
						else {
							buff[ctr].push_back('Z');
							i++;
						}
						break;
					}
					case 91: {//ś
						buff[ctr].push_back('s');
						i++;
						break;
					}
					case 121: {//Ź
						if(line[i + 1] == 1) {
							buff[ctr].push_back('Z');
							i++;
							break;
						}
						else {
							buff[ctr].push_back(line[i]);
							break;
						}
					}
					case 122: {//ź
						buff[ctr].push_back('z');
						i++;
						break;
					}
					case 123: {//Ź
						if(line[i + 1] == 1) {
							buff[ctr].push_back('Z');
							i++;
							break;
						}
					}
					case 124: {//ż
						buff[ctr].push_back('z');
						i++;
						break;
					}
					default: {
						buff[ctr].push_back(line[i]);
						break;
					}
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

	std::cout << "Rekordów: " << buff.size() - 1 << '\n';
	system("pause");
	return 0;
}
