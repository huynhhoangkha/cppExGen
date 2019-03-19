#include<iostream>
#include<cstring>
#include<fstream>
#include<string>
using namespace std;

int timeout = 0;

void build(char* argv[]){
    //Generate exercise build command
    const char* gppEx = "g++ -o exercise ";
    char* exerciseBuildCommand = new char[strlen(gppEx) + strlen(argv[1]) + 1];
    strcpy(exerciseBuildCommand, gppEx);
    strcpy(exerciseBuildCommand + strlen(gppEx), argv[1]);
    //Perform build statements
    system(exerciseBuildCommand);
    //Deallocate dynamic memory
    delete[] exerciseBuildCommand;
}

void test(char* argv[]){
    const char* exerciseFileName = "exercise.exe";
    const char* testcasesDir = "testcases";
    const char* exerciseExecuteResult = "exerciseResult.txt";
    const char* solutionExecuteResult = "solutionResult.txt";
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

            //Run the solution and get the result
            idx = 0;
            char* solRunCommand = new char[strlen(argv[2]) + strlen(testcasesDir) + line.length() + strlen(solutionExecuteResult) +  3];
            strcpy(solRunCommand, argv[2]);
            idx += strlen(argv[2]);
            solRunCommand[idx++] = ' ';
            strcpy(solRunCommand + idx, testcasesDir);
            idx += strlen(testcasesDir);
            solRunCommand[idx++] = '/';
            strcpy(solRunCommand + idx, testcaseFileName.c_str());
            idx += testcaseFileName.length();
            solRunCommand[idx++] = '>';
            strcpy(solRunCommand + idx, solutionExecuteResult);
            system(solRunCommand);

            //Deallocate dynamic memory
            delete[] exRunCommand;
            delete[] solRunCommand;


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
            ifstream solOuput;
            solOuput.open("solutionResult.txt");
            string solutionOuput;
            while (!solOuput.eof()) {
                getline(solOuput, line);
                solutionOuput = solutionOuput + line + '\n';
            }
            solOuput.close();

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
    system("del solutionResult.txt");
}

void clean(){
    system("del exercise.exe");
}

int main(int argc, char* argv[]) {
    try {
        //Check for arguments
        if (argc < 3) throw "Lack of arguments. The syntax is:<exerciseSourceCode> <solutionExecutable>";
        build(argv);
        test(argv);
        clean();
    }
    catch (const char* errMsg){
        cerr << errMsg << endl;
    }
    return 0;
}