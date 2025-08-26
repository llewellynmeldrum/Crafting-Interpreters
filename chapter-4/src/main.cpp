#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include "Token.hpp"
#define readUntilDelimiter(iss, buf, delim) getline(iss, buf, delim)
#define containsOnlyWhiteSpace(str) str.find_first_not_of(' ')==str.npos
using std::cout, std::cerr, std::cin;
using std::vector;
using std::istringstream;
using std::ifstream;
using std::ios;

void BeginScriptingMode();
void BeginFileMode(string filename);

void run(string line);

int main(int argc, char* argv[]){
	Token* t = new Token();
	cout << to_string(*t) << "\n";
	if (argc>2){
		cerr << "ERROR, usage: " << argv[0] << " <script>\n";
		exit(EXIT_FAILURE);
	} else if (argc == 2){
		// file input 
		string filename = argv[1];
		BeginFileMode(filename);
	} else { // argc==1
		// start scripting mode
		BeginScriptingMode();
	}

}

vector<string> getTokens(string line, char delim, bool skipEmpty){
	istringstream iss(line); // converts string to 'input stream'
	vector<string> tokens;

	string tok;
	while (readUntilDelimiter(iss, tok, ' ')){
		if (skipEmpty && containsOnlyWhiteSpace(tok)) continue;
		else tokens.push_back(tok);
	}


	return tokens;
}

void run(string line){
	cout << "Running line:" << line << "\n";
	vector<string> tokens = getTokens(line, ' ', true);
	printf("tokens (%lu):\n",tokens.size());
	for (int i = 0 ; i<tokens.size(); i++){
		printf("\t[%d]: ", i);
		cout << tokens.at(i) << "\n";
	}
}

void BeginScriptingMode(){
	// print prompt, take user input
	cout << "Beginning prompt mode, type a blank line to signal the end of your input." << "\n";
	
	for (;;){
		cout << "> ";
		string line; 
		getline(cin, line);
		if (line.empty()) break;
		run(line);
	}
}

void BeginFileMode(string filename){
	ifstream file;
	file.open(filename, ios::in);

	if (!file.is_open()){
		cerr << "Unable to open file: " << filename << "\n";
		exit(EXIT_FAILURE);
	}
	cout << "Opened file: " << filename << "\n";


	string line;
	while(getline(file, line)){
		run(line);
	}
}
