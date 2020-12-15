#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <Commdlg.h>//must be after windows.h

#include "version.hpp"

bool inline isUpdateAvailable(void) {
	URLDownloadToFile(NULL, "https://raw.githubusercontent.com/Jaclav/TeamsListSorter/main/version.hpp",
	                  std::string(std::string(getenv("Temp")) + "\\version.txt").c_str(), 0, NULL);
	std::fstream file(std::string(getenv("Temp")) + "\\version.txt", std::ios::in);
	std::string line;

	//extract version
	getline(file, line);
	line = line.substr(line.find("\"") + 1, line.size());
	line = line.substr(0, line.find("\""));

	return line != version;
}

int main(int argc, char** argv) {
	system("chcp 65001 > nul");
	system("title Teams list sorter");
	system("cls");
	std::string name;

	std::cout << "Copyright (c) 2020 Jaclav strona: https://github.com/Jaclav/TeamsListSorter v" << version << "\n\n";

	if(isUpdateAvailable() && MessageBoxW(NULL, L"Nowa aktualizacja jest dostępna, czy ją pobrać?", L"Aktualizacja",
	                                      MB_YESNO | MB_APPLMODAL | MB_DEFAULT_DESKTOP_ONLY | MB_ICONQUESTION) == IDYES) {
		URLDownloadToFile(NULL, "https://raw.githubusercontent.com/Jaclav/TeamsListSorter/main/bin/TeamsListSorter.exe",
		                  std::string(std::string(getenv("Temp")) + "\\TeamsListSorter.exe").c_str(), 0, NULL);

		std::fstream file(std::string(getenv("Temp")) + "\\TeamsListSorter.bat", std::ios::out);
		file << "timeout /t 1\n";
		file << "del " << argv[0] << '\n';
		file << "move " << getenv("Temp") << "\\TeamsListSorter.exe " << argv[0] << '\n';
		file << "call " << argv[0] << '\n';
		file << "exit\n";
		file.close();

		system(std::string("start " + std::string(getenv("Temp")) + "\\TeamsListSorter.bat").c_str());
		return 0;
	}

	if(argc < 2) {
		tagOFNA ofn;
		char fileName[120];
		memset(&ofn, 0, sizeof(tagOFNA));

		ofn.lStructSize = sizeof(tagOFNA);
		ofn.lpstrFile = fileName;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 120;
		ofn.lpstrFilter = "Lista Teams *.csv\0*.csv\0";

		GetOpenFileName(&ofn);
		name = ofn.lpstrFile;
	}
	else {
		name = argv[1];
	}

	std::fstream file(name, std::ios::in);
	if(!file.good()) {
		std::cout << "Plik nie istnieje bądź nie można go otworzyć!\nAby wyjść z programu należy wcisnąć ENTER.\n";
		std::cin.get();
		return 0;
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

		if(buff[ctr].size() == 0) { //if line is empty, skip it
			buff.pop_back();
			ctr--;
			continue;
		}
	}
	file.close();

	//set surname as first, name as last and delete information about class
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

	std::cout << "Rzędów: " << buff.size() << '\n';
	std::cout << "Aby wyjść z programu należy wcisnąć ENTER.\n";
	std::cin.get();
	return 0;
}
