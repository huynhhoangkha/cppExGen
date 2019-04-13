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
#include<ctime>
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

void gen(int argc, char* argv[]) {
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

void build(int argc, char* argv[]){
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

void test(int argc, char* argv[]){
    if (argc < 3) throw string("Lack of arguments.");
    if (argc > 3) {
        char* genCommand = new char[strlen(argv[0]) + strlen("gen") + strlen(argv[3]) + 3];
        int idx = 0;
        strcpy(genCommand, argv[0]); idx += strlen(argv[0]);
        genCommand[idx++] = ' ';
        strcpy(genCommand + idx, "gen"); idx += strlen("gen");
        genCommand[idx++] = ' ';
        strcpy(genCommand + idx, argv[3]);
        system(genCommand);
        delete[] genCommand;
    }
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

            //Deallocate dynamic memory
            delete[] exRunCommand;

            //Read the exercise out
            ifstream exOuput;
            exOuput.open("exerciseResult.txt");
            string exerciseOuput;
            while (!exOuput.eof()) {
                getline(exOuput, line);
                exerciseOuput = exerciseOuput + line + '\n';
            }
            exOuput.close();

            //Read the solution out
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
            else {
                cout << "FAILED." << endl;
                cout << "Result: " << endl << exerciseOuput << "Expected: " << endl << solutionOuput;
            }
        }
    }
    cout << "\nPASSED: " << passCount << "/" << testCount << endl;
    ifs.close();
    system("del testcasesList.txt");
    system("del exerciseResult.txt");
	system("del exercise.exe");
}

void genAndTest(int argc, char* argv[]) {
    // Check for params
    if (argc < 4) throw string("Wrong syntax.");
    // Build the genCode
    char* buildGenCodeCommand = new char[strlen("g++ -o tgen ") + strlen(argv[3]) + 1];
    strcpy(buildGenCodeCommand, "g++ -o tgen ");
    strcpy(buildGenCodeCommand + strlen("g++ -o tgen "), argv[3]);
    system(buildGenCodeCommand);
    //cout << buildGenCodeCommand << endl;
    delete[] buildGenCodeCommand;
    // Run the genCode for genFile
    system("tgen.exe>genFile.txt");
    //cout << "tgen.exe>genFile.txt" << endl;
    // Run the gen command
    int idx = 0;
    char* testCommand = new char[strlen(argv[0]) + strlen("test") + strlen(argv[2]) + strlen("genFile.txt") + 4];
    strcpy(testCommand, argv[0]); idx += strlen(argv[0]);
    testCommand[idx++] = ' ';
    strcpy(testCommand + idx, "test"); idx+= strlen("test");
    testCommand[idx++] = ' ';
    strcpy(testCommand + idx, argv[2]); idx+= strlen(argv[2]);
    testCommand[idx++] = ' ';
    strcpy(testCommand + idx, "genFile.txt"); idx+= strlen("genFile.txt");
    // Run the test command
    system(testCommand);
    //cout << testCommand << endl;
    delete[] testCommand;
    //throw string("pause");
    system("del tgen.exe");
}

void syntaxAnnouce(int argc, char* argv[]) {
    cout << "These are possible syntax: " << endl;
    cout << argv[0] << " gen <gen file name>" << endl;
    cout << argv[0] << " gen <gen file name>" << endl;
    cout << argv[0] << " test <source code file name>" << endl;
    cout << argv[0] << " test <source code file name> <gen file name>" << endl;
    cout << argv[0] << " out <solution source code> <testcase generator source code>" << endl;
}

string getDirName() {
    string dirName = "";
    system("dir>__sysdir.txt");
    fstream fs;
    fs.open("__sysdir.txt", ios::in);
    string line;
    while (!fs.eof()) {
        getline(fs, line);
        if (line.find("Directory of") < string::npos) {
            int dirNameIndex;
            int lineLength = line.length();
            for (dirNameIndex = lineLength - 1; dirNameIndex > 0; dirNameIndex--) {
                if (line[dirNameIndex] == '\\') break;
            }
            dirName = line.substr(dirNameIndex + 1, line.length() - 1);
            break;
        }
    }
    fs.close();
    system("del __sysdir.txt");
    return dirName;
}

void produceOuput(int argc, char* argv[]){
    // Todo
    string dirName = getDirName();
    ifstream solFile;
    solFile.open(dirName + string("_sol.cpp"), ios::in);
    if (solFile.fail()) throw string("Can not open solution file.");
    ofstream iniFile;
    iniFile.open(dirName + "_ini.cpp", ios::out);
    string line;
    int lineCount = 0;
    bool inTodoBlock = false;
    while (!solFile.eof()) {
        getline(solFile, line);
        lineCount++;
        if (line.find(dirName + string("_sol.cpp")) != string::npos)
            line = string("    Lab ") + dirName.substr(0,2) + string(": ") + dirName + string("_ini.cpp");
        if (!inTodoBlock) {
            if (line.find("BeginTodo") != string::npos) inTodoBlock = true;
            else iniFile << line << endl;
        }
        else if (line.find("EndTodo") != string::npos) {
            int pos = 0;
            while (line[pos] != '/') {
                if (line[pos++] == '\t') for (int i = 0; i < 4; i++) iniFile << ' ';
                else iniFile << ' ';
            }
            iniFile << "#TODO" << endl << endl;
            inTodoBlock = false;
        }
    }
    iniFile.close();
    solFile.close();
}

void generateSamples (int argc, char* argv[]) {
	string testGenFileName;
    string solFileName;
    if (argc == 2) {
        fstream fs;
        //Get the directory name
        string dirName = getDirName();
        //Got directory name, create new files base on the dir name
        //Generate the sol file
        fs.open(dirName + string("_sol.cpp"), ios::out);
        fs << "/**" << endl;
        fs << "    Faculty of Computer Science and Engineering" << endl;
        fs << "    Ho Chi Minh City University of Technology" << endl;
        fs << "    Programming fundamentals - spring 2019" << endl;
        fs << "    " << "Lab " << dirName.substr(0,2) << ": " << dirName << "_sol.cpp" << endl;
        fs << "    @author CSE - HCMUT" << endl;
        time_t now = time(NULL);
        fs << "    @version 1.0 " << ctime(&now) << endl;
        fs << "*/" << endl;
        fs << "#include <iostream>" << endl;
        fs << "#include <fstream>" << endl;
        fs << "#include <string>" << endl;
        fs << "#include <cstring>" << endl;
        fs << "#include <iomanip>" << endl;
        fs << "#include <math.h>" << endl;
        fs << "#include <cctype>" << endl;
        fs << "#define FILENAME \"" << dirName << "_sol.cpp\"" << endl;
        fs << "using namespace std;" << endl;
        fs << "// Begin implementation" << endl;
        fs << "" << endl;
        //Code check section
        fs << "bool codeCheck() {" << endl;
        fs << "    // Define the forbiddenKeyword[]" << endl;
        fs << "    const char* forbiddenKeyword[] = {\"strcmp\", \"string \"};" << endl;
        fs << "    // Open file" << endl;
        fs << "    fstream ifs;" << endl;
        fs << "    ifs.open(FILENAME, ios::in);" << endl;
        fs << "    // Get file size" << endl;
        fs << "    ifs.seekg(0, ifs.end);" << endl;
        fs << "    int fileSize = ifs.tellg();" << endl;
        fs << "    ifs.seekg(0, ifs.beg);" << endl;
        fs << "    // Read file into fileContent array" << endl;
        fs << "    char* fileContent = new char[fileSize];" << endl;
        fs << "    ifs.read(fileContent, fileSize);" << endl;
        fs << "    // Close the file" << endl;
        fs << "    ifs.close();" << endl;
        fs << "    // Truncate the irrelevant code" << endl;
        fs << "    *strstr(fileContent, \"bool codeCheck() {\") = '\\0';" << endl;
        fs << "    char* todoSegment = strstr(fileContent ,\"// Begin implementation\");" << endl;
        fs << "    // Check the code" << endl;
        fs << "    int numberOfForbiddenKeyword = sizeof(forbiddenKeyword) / sizeof(const char*);" << endl;
        fs << "    for (int i = 0; i < numberOfForbiddenKeyword; i++) { if (strstr(todoSegment, forbiddenKeyword[i])) return false; }" << endl;
        fs << "    // Tidy the dynamic mem" << endl;
        fs << "    delete[] fileContent;" << endl;
        fs << "    return true;" << endl;
        fs << "}" << endl;
        fs << "" << endl;
        //End code check section
        fs << "int main(int argc, char* argv[]) {" << endl;
        //Section: Code check call
        fs << "    if (codeCheck() == false) {" << endl;
        fs << "        cout << \"Forbidden-keyword rule violation.\" << endl;" << endl;
        fs << "        return -1;" << endl;
        fs << "    }" << endl;
        //Endsection: Code check call
        fs << "    // Section: read testcase" << endl;
        fs << "    ///Student may comment out this section for local testing" << endl;
        fs << "    if (argc < 2) return 0;" << endl;
        fs << "    ifstream fileIn;" << endl;
        fs << "    try {" << endl;
        fs << "        fileIn.open(argv[1]);" << endl;
        fs << "        if (fileIn.fail()) throw \"Failed to open file.\";" << endl;
        fs << "        string line;" << endl;
        fs << "        int lineCount = 0;" << endl;
        fs << "        while (!fileIn.eof()){" << endl;
        fs << "            lineCount++;" << endl;
        fs << "            getline(fileIn, line);" << endl;
        fs << "            cout << line << endl;" << endl;
        fs << "        }" << endl;
        fs << "        fileIn.close();" << endl;
        fs << "    }" << endl;
        fs << "    catch (const char* errMsg){" << endl;
        fs << "        cerr << errMsg;" << endl;
        fs << "    }" << endl;
        fs << "    // Endsection: read testcase" << endl;
        fs << "    //------------------------------------" << endl;
        fs << "    return 0;" << endl;
        fs << "}" << endl;
        fs.close();
        // Generate the testGenFileName
        fs.open(dirName + string("_testGen.cpp"), ios::out);
        fs << "/**" << endl;
        fs << "    Programming fundamentals - spring 2019" << endl;
        fs << "    Testcase generator for Lab " << dirName.substr(0,2) << ": Question " << dirName << endl;
        fs << "    @author Huynh Hoang Kha" << endl;
        fs << "    @version 1.0 " << ctime(&now);
        fs << "*/" << endl;
        fs << "#pragma (warning: disable 4996)" << endl;
        fs << "#include<iostream>" << endl;
        fs << "#include<iomanip>" << endl;
        fs << "#include<time.h>" << endl;
        fs << "#include<cstring>" << endl;
        fs << "#include<string>" << endl;
        fs << "#include<cctype>" << endl;
        fs << "using namespace std;" << endl;
        fs << endl;
        fs << "class Integer {" << endl;
        fs << "    int value;" << endl;
        fs << "    static int width;" << endl;
        fs << "public:" << endl;
        fs << "    Integer(int _value, int _width = Integer::width) {" << endl;
        fs << "        this->value = _value;" << endl;
        fs << "        this->width = _width;" << endl;
        fs << "    }" << endl;
        fs << "    int getValue() { return this->value; }" << endl;
        fs << "    void setValue(int _value) { this->value = _value; }" << endl;
        fs << "    int getWidth() { return this->width; }" << endl;
        fs << "    void setWidth(int _width) { this->width = _width; }" << endl;
        fs << "    friend ostream& operator<<(ostream& out, Integer& obj);" << endl;
        fs << "    int operator= (int _value) { return this->value = _value; }" << endl;
        fs << "    Integer operator+(Integer &rightHandSide) {    return Integer(this->value + rightHandSide.value); }" << endl;
        fs << "    Integer operator+(int rightHandSide) { return Integer(this->value + rightHandSide); }" << endl;
        fs << "    template <typename T>" << endl;
        fs << "    T operator+(T rightHandSide) { return this->value + rightHandSide; }" << endl;
        fs << "};" << endl;
        fs << "" << endl;
        fs << "int Integer::width = 3;" << endl;
        fs << "" << endl;
        fs << "ostream& operator<<(ostream& out, Integer& obj) {" << endl;
        fs << "    string ostring = to_string(obj.value);" << endl;
        fs << "    while (ostring.length() < Integer::width) ostring.insert(ostring.begin(), '0');" << endl;
        fs << "    out << ostring;" << endl;
        fs << "    return out;" << endl;
        fs << "}" << endl;
        fs << endl;
        fs << "int main() {" << endl;
        fs << "    Integer count = 0;" << endl;
        fs << "    for (int i = 1; i <= 20; i++) {" << endl;
        fs << "        cout << \"//=======================\" << endl;" << endl;
        fs << "        count = count + 1;" << endl;
        fs << "        cout << \"testcase\" << count << \":\" << endl;" << endl;
        fs << "        //-----------------------------------" << endl;
        fs << "        //Section: Generate the testcase" << endl;
        fs << "        " << endl;
        fs << "        //Endsection: Generate the testcase" << endl;
        fs << "        //-----------------------------------" << endl;
        fs << "        cout << \"expect:\" << endl;" << endl;
        fs << "        //-----------------------------------" << endl;
        fs << "        //Section: Generate the expect" << endl;
        fs << "        " << endl;
        fs << "        //Endsection: Generate the expect" << endl;
        fs << "        //-----------------------------------" << endl;
        fs << "        cout << \"#end\" << endl;" << endl;
        fs << "        cout << \"//=======================\";" << endl;
        fs << "        if (i < 20) cout << endl;" << endl;
        fs << "    }" << endl;
        fs << "    return 0;" << endl;
        fs << "}" << endl;
        fs.close();
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) throw string("Lack of arguments.");
        if (strcmp(argv[1], "gen") == 0) {
            if (argc < 3) throw string("Lack of arguments.");
            gen(argc, argv);
        }
        else if (strcmp(argv[1], "test") == 0) {
            if (argc < 3) throw string("Lack of arguments.");
            build(argc, argv);
            test(argc, argv);
        }
        else if (strcmp(argv[1], "out") == 0) {
            if (argc < 3) throw string("Lack of arguments.");
            genAndTest(argc, argv);
            produceOuput(argc, argv);
        }
        else if (strcmp(argv[1], "init") == 0) {
            generateSamples(argc, argv);
        }
        else throw string("Unknown syntax.");
    }
    catch (string errMsg) {
        cerr << errMsg << endl;
        syntaxAnnouce(argc, argv);
    }
    return 0;
}