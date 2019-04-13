#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

void proc(string code) {
    int length = code.length();
    cout << "fs << \"";
    for (int i = 0; i < length; i++) {
        if (code[i] == '\t') {
            for (int i = 0; i < 4; i++) cout << ' ';
        }
        else if (code[i] == '\"') cout <<"\\\"";
        else if (code[i] == '\\') cout <<"\\\\";
        else cout << code[i];
    }
    cout << "\" << endl;";
    cout << endl;
}

int main (int argc, char* argv[]) {
    try {
        if (argc < 2) throw "Arguments required.";
        ifstream ifs;
        ifs.open(argv[1]);
        if(ifs.fail()) throw "Unable to open file.";
        string line;
        while (!ifs.eof()) {
            getline(ifs, line);
            proc(line);
        }
        ifs.close();
    }
    catch (const char* errMsg) {
        cerr << errMsg << endl;
    }
    
    return 0;
}