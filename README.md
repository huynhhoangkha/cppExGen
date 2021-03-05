# First notes
- This project was created to support the process of making single-source-file cpp excercises.
- It can run only on Windows. <br>
- It does not support Windows Power Shell, please use the Cmd

# Installation
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

# User's guide
During the process, you would be working with these files:
- `<containingFolderName>_testGen.cpp`
- `<containingFolderName>_sol.cpp`
- Gen-file: A text file written in a specific format. This file is used as materials to create pairs of `testcase` and `expect` files for the grading process. The gen-file can be created either manually or automatically. The `<containingFolderName>_testGen.cpp` is a sample source code that you can use to generate the gen-file automatically. The automatically generated gen-file is named `genFile.txt` by default.

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

Before learning how to use these sample sources, let's take a look on how does this tool generate testcases and test the solution source.
## Gen-file syntax
This tool offers a way to generate pairs of `testcase` and `expect` files using gen-files (explained earlier).<br>
Its syntax is as follow:
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
// ...
```
>*Note: The `content of the testcase` and the `content of the expected result` **MUST NOT** contain any comment. If you write any comments there, they will NOT be the treated as comments but plain texts :D*


>*Note: You had better be careful with any characters including non-graphical ones in the `content of the testcase` and the `content of the expected result` because they are plain texts and any redundancies are considered wrong.*
## Gen-file example
### Problem: Find the absolute value of integer numbers
- Input: A text file containing an integer number
- Expected output: A text file containing an integer whose the value which is the absolute value of the input. <br>
  
The content of the gen-file may be as follow:
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
## Generate testcases from gen file
In the directory containing the gen-file, run:
```
pf gen <gen file name>
```
After running the command above, there would be two folders named `testcases` and `expects` respectively. <br>
As mentioned earlier, each testcase has two parts: `content of the testcase` and `content of the expected result`. Those parts would be separated from each other and put into the corresponding folders. <br><br>
If you generate testcases using the example gen-file given in the previous section, in the `testcases` folder, there would be two files:
```
testcase000.txt
testcase001.txt
```
and in the `expects` folder, there would be:
```
expect000.txt
expect001.txt
```

## Implement the solution to the exercise
Okay, we have learned how to write a gen-file and generate testcases using the tool. Now, what remaining is to implement the solution source code and test it out.

### The `main` function
As explained earlier somewhere in this documentation =]]]], during the grading process, the tool compiles the solution source code into an executable file and then runs it to check the output. The testcase files in the `testcases` folder are used one by one to test the solution. So how does the executable file know the existence of these testcase files? The answer is, the tool scans the `testcases` folder to get a list of files whose the `"testcase"` prefix and then passes them **one by one** to the executable file when executing it using the following command-line syntax:
```
<executable solution file name> <testcase file name>
```
So now you understand my idea, right? The `testcase file name` would be a `c-string` and its address is held in `argv[1]`!<br>

If you are preparing an exercise, you will need to write a stuff of code that:
- Opens and reads the testcase file
- Passes the testcase to the function where you will write the solution, and of course don't forget to implement it! =)))) LOL
- Print out the output in the format specified by the exercise if you want to help students do that.
## Test a solution source code using the existing testcases
```
pf.exe test <source code file name>
```
This command will compile the solution source code and then run it with testcase files in the `testcases` folder, one file at a time. After executing the solution with each testcase file, the tool will compare the executing result with the content of the corresponding expect-file in the `expects` folder. If they completely match (including non-graphical characters), the tool will announce: `PASSED`. Or else, it will announce the differences between the executing result from the expected one.
> *Note: If the result is FAILED but you don't see anything different from the executing result and the expected one, there may be some non-graphical character redundancies. In this case, you should export the testing result into a log file and then compare the differences using any simple comparison tools.*

```
pf.exe test <source code file name> > log.txt
```
## Generate testcases and then test the solution source code using a single command
In the case you have a solution source file and you also have a gen-file already, you can generate testcases and then test the solution source code using only one command:
```
pf test <source code file name> <gen file name>
```
Of course, you can generate the testcases and then test the solution source code using the two previously introduced commands in sequence:
>`pf gen <gen file name>`

>`pf.exe test <source code file name>`


But using this combined command saves your time.

## Creating gen-file automatically using the `<containingFolderName>_testGen.cpp` sample source code
### Number of the testcases
In the `<containingFolderName>_testGen.cpp` file, find the following macro in the header part:
```
#define NUMBER_OF_TESTCASES 20
```
This is the number of testcases that you want to generate. You can re-define it if you want more or fewer testcases.

### Generate the gen-file
In the `main` function of the `<containingFolderName>_testGen.cpp` file, there is a `FOR loop`. The body of the loop is where you write your code to generate **ONE** testcase.<br>

After finished implementing the `<containingFolderName>_testGen.cpp` file, run the following command to generate a gen-file:
```
g++ -o testGen.exe <containingFolderName>_testGen.cpp
```

```
testGen
```

## Release the exercise frame
### The `Todo` blocks
In your solution source code, put the code segment that you want it to be replaced with `#TODO` in the `// BeginTodo` and `// EndTodo` comments pair as follow:
```
// BeginTodo
<The code segment you want to replace with #TODO>
// EndTodo
```

Finally, to generate a source frame, run:
```
pf out <solution source code> <auto testcase generating source code>
```
If there is no error, a file named `<containingFolderName>_init.cpp` would be generated. This file contains everything you wrote in the `<containingFolderName>_sol.cpp` except for segments that you wrapped in the `// BeginTodo` and `// EndTodo` comments pair. Give the `init` file to students together with the exercise description and they will do it!