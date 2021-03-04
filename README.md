### How does this program work?



### Initialize an exercise:
Step 1: Create a containing folder and name it with the name of your exercise
Step 2: Navigate into the containing folder created in step 1
Step 3: Open a cmd session
Step 4: Run the following command
```
pf init
```
After executing the command above, there should be two files generated
```
<containingFolderName>_testGen.cpp
<containingFolderName>_sol.cpp
```

The file `<containingFolderName>_testGen.cpp` is a template of a program that you may use to generate testcases automatically and the file `<containingFolderName>_sol.cpp` is a template of the solution to the exercise.

### Generate testcases from gen file
```
checker.exe gen <gen file name>
```
### Test the source code using the existing testcases
```
checker.exe test <source code file name>
```
### Generate testcases and then test the source code
```
checker.exe test <source code file name> <gen file name>
```
### Release the final outputs
```
checker.exe out <solution source code> <testcase generator source code>
```
### Syntax for replacing #TODO
```
// BeginTodo
<The source code you want to replace with #TODO>
// EndTodo
```
### If your machine lacks of some *.dll, please clone this code and re-compile
```
g++ -o gradeByExe gradeByExe.cpp
```
```
g++ -o gradeByCode gradeByCode.cpp
```
```
g++ -o pf checker.cpp
```
### MinGW download link
```
https://sourceforge.net/projects/mingw/
```
### How to add an environment variable in Windows 10?
```
https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/
```