# First note
- This project was created to support the process of making single-source-file cpp excercises.
- It can run only on Windows. <br>

# How to install this tool?
**Step 1**: Download or clone this repository, put the folder into where you have executing permission. <br>
**Step 2**: Get the absolute path to the containing folder of the executable file, in this case, the `pf.exe` <br>
**Step 3**: Add the path you've got from the step above into the `path` environment variable. If you don't know how, refer to this guide:
```
https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/
```
**Step 4:** Check the installation: Open a cmd session, and execute the following command:
```
pf
```
The result should be:
```
Lack of arguments.
These are possible syntax:
pf gen <gen file name>
pf test <source code file name>
pf test <source code file name> <gen file name>
pf out <solution source code> <testcase generator source code>
```
If the executable file can't run, rebuilding it may help:
```
g++ -o gradeByExe gradeByExe.cpp
```
```
g++ -o gradeByCode gradeByCode.cpp
```
```
g++ -o pf checker.cpp
```
If you still get an error, please check if you've installed the g++ compiler properly or not =))) <br>
### MinGW download link
```
https://sourceforge.net/projects/mingw/
```

# How does this program work?
During the process, you would be working with these files:
- `<containingFolderName>_testGen.cpp`
- `<containingFolderName>_sol.cpp`
- Gen-file: A text file written in a specific format. This file is used as materials to create pairs of `testcase` and `expect` files for the grading process. The gen-file can be created either manually or automatically. The `<containingFolderName>_testGen.cpp` is a sample source code that you can use to generate the gen-file automatically. The automatically generated gen-file is named `genFile.txt` by default.

## Gen-file syntax
```
// Some comments
testcase<testcaseName>:
<Content of the testcase>
expect:
<Content of the expected result>
#end
// Some comment

testcase<testcaseName>:
<Content of the testcase>
expect:
<Content of the expected result>
#end
// Some comments

...
```
*Note: The `content of the testcase` and the `content of the expected result` **MUST NOT** contain any comment. If you write any comments there, they will NOT be the treated as comments but plain texts :D*
## Gen-file example
### Problem: Find the absolute value of integer numbers
- Input: A text file containing an integer number
- Expected output: A text file containing an integer whose the value which is the absolute value of the input. <br>
  
The conten of the gen-file may be as follow:
```
//--------------
testcase000:
-25
expect:
25
#end
//--------------

//--------------
testcase001:
1
expect:
1
#end
//--------------
```

## Initialize an exercise
**Step 1**: Create a containing folder and name it with the name of your exercise <br>
**Step 2**: Open a cmd session <br>
**Step 3**: Navigate into the containing folder created in step 1 <br>
**Step 4**: Run the following command <br>
```
pf init
```
After executing the command above, there should be two files generated
```
<containingFolderName>_testGen.cpp
<containingFolderName>_sol.cpp
```

The file `<containingFolderName>_testGen.cpp` is sample source code that you may use to generate testcases automatically and the file `<containingFolderName>_sol.cpp` is a template of the solution to the exercise.<br>

## Generate testcases from gen file
```
checker.exe gen <gen file name>
```
## Test the source code using the existing testcases
```
checker.exe test <source code file name>
```
## Generate testcases and then test the source code
```
checker.exe test <source code file name> <gen file name>
```
## Release the final outputs
```
checker.exe out <solution source code> <testcase generator source code>
```
## Syntax for replacing #TODO
```
// BeginTodo
<The source code you want to replace with #TODO>
// EndTodo
```