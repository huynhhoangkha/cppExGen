/***************************************************/
//Testcases generator and code tester for PE2019
//Code by Huynh Hoang Kha 11-03-2019
/***************************************************/
#include<iostream>
#include<cstring>
#include<string>
#include<fstream>
#include<windows.h>
#include<vector>
using namespace std;

enum GenFileReadingState {READ_TESTCASE_NUMBER, READ_INPUT, READ_EXPECT};

//Check if the directory existence (Windows only)
bool dirExists(const std::string& dirName_in) {
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;
    return false; 
}

//Function help check if a string exist in a vector or not
bool exist(string stringToFind, vector<string> vectorOfString) {
    for (int i = 0; i < vectorOfString.size(); i++)
        if (vectorOfString[i].compare(stringToFind.c_str()) == 0) return true;
    return false;
}

void gen(char* argv[]) {
    if (!dirExists(string("testcases"))) system("mkdir testcases");
    if (!dirExists(string("expects"))) system("mkdir expects");
    //Open gen file
    ifstream genFile;
    genFile.open(argv[2], ios::in);
    if (genFile.fail()) throw string("Generation input file error. Cannot open.");
    //Init some variables for reading
    int lineCount = 0;
    int contentLineCount = 0;
    string line;
    GenFileReadingState state = READ_TESTCASE_NUMBER;
    vector<string> testcaseList;
    testcaseList.clear();
    fstream fs;
    //Reading loop
    while (!genFile.eof()) {
        lineCount++;
        getline(genFile, line);
        switch (state) {
            case READ_TESTCASE_NUMBER:
                if (line.length() == 0 || line.substr(0, 2).compare("//") == 0) continue;
                if (line.substr(0, 8).compare("testcase") || line.back() != ':') {
                    string errMsg = string("Syntax error at line ") + to_string(lineCount)
                        + string (". Expected: \"testcase<notation>:\"");
                    throw errMsg;
                }
                line.pop_back();
                if (exist(line, testcaseList)){
                    string errMsg = string("Testcase name re-defined at line ")
                        + to_string(lineCount);
                    throw errMsg;
                }
                testcaseList.push_back(line);
                contentLineCount = 0;
                state = READ_INPUT;
                fs.open(string("testcases/") + testcaseList.back() + string(".txt"), ios::out);
                break;
            case READ_INPUT:
                if (line.compare("expect:") == 0) {
                    if (contentLineCount == 0) {
                        string errMsg = string("Error at line ") + to_string(lineCount) + string(": Empty input");
                        throw errMsg;
                    }
                    contentLineCount = 0;
                    state = READ_EXPECT;
                    fs.close();
                    string expectFileName = testcaseList.back();
                    expectFileName = string("expects/expect") + expectFileName.substr(8, expectFileName.length() - 8) + string(".txt");
                    fs.open(expectFileName, ios::out);
                    continue;
                }
                if (contentLineCount++ > 0) fs << endl;
                fs << line;
                break;
            case READ_EXPECT:
                if (line.compare("#end") == 0) {
                    if (contentLineCount == 0) {
                        string errMsg = string("Error at line ") + to_string(lineCount) + string(": Empty expect field");
                        throw errMsg;
                    }
                    contentLineCount = 0;
                    state = READ_TESTCASE_NUMBER;
                    fs.close();
                    continue;
                }
                if (contentLineCount++ > 0) fs << endl;
                fs << line;
                break;
            default:
                break;
        }
    }
    if (state != READ_TESTCASE_NUMBER) throw string("Error: Not complete testcase at line ") + to_string(lineCount);
    genFile.close();
}

void build(char* argv[]){
    //Generate build command
    const char* gppEx = "g++ -o exercise ";
    char* exerciseBuildCommand = new char[strlen(gppEx) + strlen(argv[2]) + 1];
    strcpy(exerciseBuildCommand, gppEx);
    strcpy(exerciseBuildCommand + strlen(gppEx), argv[2]);
    //Perform build statements
    system(exerciseBuildCommand);
    //Deallocate dynamic memory
    delete[] exerciseBuildCommand;
}

void test(char* argv[]){
    const char* exerciseFileName = "exercise.exe";
    const char* testcasesDir = "testcases";
    const char* exerciseExecuteResult = "exerciseResult.txt";
    //Scan for testcase folder
    system("dir testcases>testcasesList.txt");
    ifstream ifs;
    ifs.open("testcasesList.txt");
    string line;
    string testcaseFileName;
    int passCount = 0;
    int testCount = 0;
    while(!ifs.eof()){
        getline(ifs, line);
        int pos = -1;
        if ((pos = line.find("testcase")) != string::npos && line.find(".txt") != string::npos){
            testCount++;
            //Get the testcase file name in this line
            testcaseFileName = line.substr(pos, line.length() - pos);
            cout << "-------------------" << testcaseFileName << "-------------------" << endl;
            int idx = 0;

            //Run the exercise and get the result
            char* exRunCommand = new char[strlen(exerciseFileName) + strlen(testcasesDir) + line.length() + strlen(exerciseExecuteResult) +  3];
            strcpy(exRunCommand, exerciseFileName);
            idx += strlen(exerciseFileName);
            exRunCommand[idx++] = ' ';
            strcpy(exRunCommand + idx, testcasesDir);
            idx += strlen(testcasesDir);
            exRunCommand[idx++] = '/';
            strcpy(exRunCommand + idx, testcaseFileName.c_str());
            idx += testcaseFileName.length();
            exRunCommand[idx++] = '>';
            strcpy(exRunCommand + idx, exerciseExecuteResult);
            system(exRunCommand);
            cout << exRunCommand << endl;

            //Deallocate dynamic memory
            delete[] exRunCommand;

            //Read the exercise output
            ifstream exOuput;
            exOuput.open("exerciseResult.txt");
            string exerciseOuput;
            while (!exOuput.eof()) {
                getline(exOuput, line);
                exerciseOuput = exerciseOuput + line + '\n';
            }
            exOuput.close();

            //Read the solution output
            ifstream expectFile;
            string expectFileName(testcaseFileName);
            expectFileName = string("expects/expect") + expectFileName.substr(8, expectFileName.length() - 12) + string(".txt");
            expectFile.open(expectFileName);
            if (expectFile.fail()) {
                string errMsg = string("Could not open ") + expectFileName;
                throw errMsg;
            }
            string solutionOuput;
            while (!expectFile.eof()) {
                getline(expectFile, line);
                solutionOuput = solutionOuput + line + '\n';
            }
            expectFile.close();

            //Compare
            if (strcmp(exerciseOuput.c_str(), solutionOuput.c_str()) == 0) {
                cout << "PASSED." << endl;
                passCount++;
            }
            else cout << "FAILED." << endl;
            cout << "Result: " << endl << exerciseOuput << endl << "Expected: " << endl << solutionOuput << endl;
        }
    }
    cout << "\nPASSED: " << passCount << "/" << testCount << endl;
    ifs.close();
    system("del testcasesList.txt");
    system("del exerciseResult.txt");
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) throw string("Lack of arguments. Syntax for:\nTestcases generation: gen <TestGenFile>\nCode testing: test <sourceCodeFile>\n");
        if (strcmp(argv[1], "gen") == 0) gen(argv);
        else if (strcmp(argv[1], "test") == 0) {
            build(argv);
            test(argv);
        }
        else throw string("Option out of range. Syntax for:\nTestcases generation: gen <TestGenFile>\nCode testing: test <sourceCodeFile>\n");
    }
    catch (string errMsg) {
        cerr << errMsg << endl;
    }
    return 0;
}