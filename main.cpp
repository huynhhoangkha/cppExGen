#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
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
	return 0;
}