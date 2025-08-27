#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include "Token.hpp"

using std::cout, std::cerr, std::cin;
using std::vector;
using std::istringstream, std::ifstream, std::ios;

void Run(string line);
void RunPrompt();
void RunFile(string filename);

boolean hadError = false;


int main(int argc, char* argv[]){
	if (argc>2){ cerr << "ERROR, usage: " << argv[0] << " <script>\n"; exit(EXIT_FAILURE); } 

	if (argc == 2) RunFile(argv[1]);
	else	       RunPrompt();
	
}

void report(int line, string where, string msg){
	cerr << "[line "<< line << "] Error" << where << ": " << msg << "\n";
	hadError = true;
}

void error (int line, string msg){
	report(line, "", msg);
}


void Run(string line){
	Scanner scanner = new Scanner(line);
	vector<Token> tokens = scanner.scanTokens();

	for (Token tok: tokens){
		cout << to_string(tok) << "\n";
	}
}


void RunFile(string filename){
	ifstream file;
	file.open(filename, ios::in);

	if (!file.is_open()){
		cerr << "Unable to open file: " << filename << "\n";
		exit(EXIT_FAILURE);
	}
	cout << "Opened file: " << filename << "\n";

	string line;
	while(getline(file, line)){
		Run(line);
		if (hadError) exit(EXIT_FAILURE);
	}
}

void RunPrompt(){
	// print prompt, take user input
	cout << "Beginning prompt mode, type a blank line to signal the end of your input." << "\n";
	
	for (;;){
		cout << "> ";
		string line; 
		getline(cin, line);
		if (line.empty()) break;
		Run(line);
		hadError = false;
	}
}
