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
Get MinGW from [sourceforge.net](https://sourceforge.net/projects/mingw/)
```
### How to add an environment variable in Windows 10?
```
See article [guide](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/) for guides
```