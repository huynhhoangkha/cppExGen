/*
	Faculty of Computer Science and Engineering
	Ho Chi Minh City University of Technology
	Programming fundamentals - spring 2019
	Lab 3: 0300.cpp
	Exercise requirement summary:
	
	@author CSE - HCMUT
	@version 1.0 //2019
*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
using namespace std;

int main(int argc, char* argv[]) {
	// Section: read testcase
	///Student may comment out this section for local testing
	if (argc < 2) return 0;
	ifstream fileIn;
	try {
		fileIn.open(argv[1]);
		if (fileIn.fail()) throw "Failed to open file.";
		string line;
		int lineCount = 0;
		while (!fileIn.eof()){
			lineCount++;
			getline(fileIn, line);
			cout << line << endl;
		}
		fileIn.close();
	}
	catch (const char* errMsg){
		cerr << errMsg;
	}
	// Endsection: read testcase
	return 0;
}