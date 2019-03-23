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