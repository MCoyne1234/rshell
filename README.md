# rshell
CS 100: Assignment 3 

[![Build Status](https://travis-ci.org/coverxit/rshell.svg?branch=master)](https://travis-ci.org/coverxit/rshell)

## License

This project is licensed under [GNU General Public License v3.0](https://github.com/coverxit/rshell/blob/master/LICENSE).

## Introduction

rshell is a command shell written in C++, invoking syscalls `fork`, `execvp`, `wait` etc., with the capacity of:

- Print a command prompt (`user@host: dir $`).
- Handle complex commands connected by `||`, `&&`, or `;`.
- Support precedence operators `(` and `)`.

The command should have the following form:

```
cmd        = [ ( ] executable [ argumentList ] [ connector cmd ] [ ) ] [ connector cmd ]
connector  = || or && or ;
```

Currently, we have the following built-in commands:

| Command              | Description                                                              |
|----------------------|--------------------------------------------------------------------------|
| exit [code]          | Exit rshell with a given code. If `code` is not given, `0` is used.      |
| cd \<dir\>           | Change working directory to a given `dir`. Note that `dir` is required.  |
| test [flag] \<path\> | Test if a given `path` exists/is a dir/file based on `flag`.             |

We have also applied **Doxygen** syntax in our comment, which could generate nice documentation automatically by running `doxygen` commands.

## Class Structure

- `CmdBase`, the abstract base class for commands.
	- `CmdBinary`, the base class for binary commands.
		- `CmdAnd`, the class for operator &&.
		- `CmdOr`, the class for operator ||.
	- `CmdSequence`, the class for command sequence sparaterd by semi-colon (;).
	- `CmdUnary`, the class for single (unary) command.
- `CmdParser`, the class for parsing user input into command tree.
- `Executor`, the class for executing command.

## Installation

Run the following commands to install:

```
$ git clone https://github.com/coverxit/rshell.git
$ cd rshell
$ git checkout hw3
$ make
$ bin/rshell
```

## Functionality

**1. Print prompt with username, hostname and working directory.**

rshell is able to print a prompt with the form `user@host: dir $`.

**2. Print detailed error when execution failed.**

When an error in execution occurred, rshell could print corrsponding information.

- Nonexistent commands. `foo` would print `rshell: foo: No such file or directory`.
- Failed execution. `cd /foo` would print `rshell: cd: /foo: No such file or directory`.

**3. Execute combinations of operators.**

rshell could handle commands such as

```
ls -al; echo hello && mkdir test || echo world; git status
```

Assume that directory `tests` exists, then executing above commands would print

```
Shindo@Shindos-MacBook-Pro.local: rshell $ ls -al; echo hello && mkdir tests || echo world; git status
total 136
drwxr-xr-x  13 Shindo  staff    442 10 31 02:34 .
drwx------+ 11 Shindo  staff    374 10 31 01:47 ..
drwxr-xr-x  13 Shindo  staff    442 10 31 02:33 .git
-rw-r--r--   1 Shindo  staff    286 10 24 23:44 .gitignore
-rw-r--r--   1 Shindo  staff     27 10 29 21:55 .travis.yml
-rw-r--r--   1 Shindo  staff  35141 10 24 18:23 LICENSE
-rwxr-xr-x   1 Shindo  staff    473 10 24 23:44 Makefile
-rw-r--r--@  1 Shindo  staff   3925 10 31 02:34 README.md
drwxr-xr-x  10 Shindo  staff    340 10 31 02:33 src
drwxr-xr-x   7 Shindo  staff    238 10 31 02:33 tests
hello
mkdir: cannot create directory ‘tests’: File exists
word	
On branch exec
Your branch is up-to-date with 'origin/exec'.
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   README.md

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	bin/

no changes added to commit (use "git add" and/or "git commit -a")
```

**4. Support changing precedence by using `(` and `)`.**

The parentheses `()` operators can be used to chagne the precedence of the returns of commands, connectors and chains of connectors. For example,

```
$ echo a && echo b || echo c && echo d
```

would print

```
a
b
d
```

With parentheses, we can change the precedence of connectors

```
$ (echo a && echo b) || (echo c && echo d)
```

that would print

```
a
b
```


**5. Detect bad syntax.**

For user input such as `ls -al &&`, rshell could detect the bad syntax.

- `ls -al &&` would print `rshell: bad syntax: unexpected token '&&'`.
- `|| ls -al` would print `rshell: bad syntax: unexpected token '||'`.
- `ls &&&&&& echo oh` would print `rshell: bad syntax: unexpected token '&&'`.
- `ls &&& echo what` would print `rshell: bad syntax: unexpected token ' ', near '&'`.
- `ls &&| mkdir test` would print `rshell: bad syntax: unexpected token ' ', near '|'`.
- `((((echo test)` would print `rshell: bad syntax: missing closing ')'`.
- `(echo test)))` would print `rshell: bad syntax: unexpected token ')'`.
- `(echo test)()` would print `rshell: bad syntax: unexpcted token '('`.
- `no-connector (echo test)` would print `rshell: bad syntax: unexpected token '('`.
- `(echo test) no-connector` would print `rshell: bad syntax: unexpected token 'no-conector'`.
- `echo "no closing` would print `rshell: bad syntax: missing closing '"'`.
- `echo "mix use'` would print `rshell: bad syntax: missing closing '"'`.

Currently, we treat single `&` and `|` as illegal symbol, that is

- `ls & echo oh` would print `rshell: bad syntax: unexpected token ' ', near '&'`.

## Known Issues

**1. Cannot handle signals.**

Currently, rshell does not handle signals such as `Ctrl+C`.

**2. Cannot handle TABs.**

Currently, rshell does not recognize tabs, treating them the same as spaces.

**3. Cannot handle arrow buttons.**

Currently, rshel cannot recognize arrow buttons such as `↑`.

## Valgrind Test

```
[rwu016@hammer rshell]$ valgrind --tool=memcheck bin/rshell
==5017== Memcheck, a memory error detector
==5017== Copyright (C) 2002-2012, and GNU GPL'd, by Julian Seward et al.
==5017== Using Valgrind-3.8.1 and LibVEX; rerun with -h for copyright info
==5017== Command: bin/rshell
==5017== 
rwu016@hammer.cs.ucr.edu: rshell $ ls -al
总用量 80
drwxr-xr-x  6 rwu016 csmajs  4096 11月 18 14:33 .
drwx--x--x 17 rwu016 csmajs  4096 11月  9 18:59 ..
drwxr-xr-x  2 rwu016 csmajs  4096 11月 18 14:33 bin
drwxr-xr-x  8 rwu016 csmajs  4096 11月 18 14:33 .git
-rw-r--r--  1 rwu016 csmajs   307 11月 18 14:20 .gitignore
-rw-r--r--  1 rwu016 csmajs 35141 11月  2 16:20 LICENSE
-rwxr-xr-x  1 rwu016 csmajs   473 11月  2 16:20 Makefile
-rw-r--r--  1 rwu016 csmajs  6509 11月  2 16:20 README.md
drwxr-xr-x  2 rwu016 csmajs  4096 11月 18 14:33 src
drwxr-xr-x  3 rwu016 csmajs  4096 11月 18 14:21 tests
-rw-r--r--  1 rwu016 csmajs    27 11月  2 16:20 .travis.yml
rwu016@hammer.cs.ucr.edu: rshell $ [ -f .travis.yml ] && echo "path exists"
(True)
path exists
rwu016@hammer.cs.ucr.edu: rshell $ make clean
rm -f src/*.o
rm -rf bin
rwu016@hammer.cs.ucr.edu: rshell $ make
g++ -c -Wall -Werror -ansi -pedantic -o src/main.o src/main.cpp
mkdir -p bin
g++  -o bin/rshell src/main.o
rwu016@hammer.cs.ucr.edu: rshell $ exit  
==5017== 
==5017== HEAP SUMMARY:
==5017==     in use at exit: 150 bytes in 4 blocks
==5017==   total heap usage: 324 allocs, 320 frees, 123,413 bytes allocated
==5017== 
==5017== LEAK SUMMARY:
==5017==    definitely lost: 0 bytes in 0 blocks
==5017==    indirectly lost: 0 bytes in 0 blocks
==5017==      possibly lost: 0 bytes in 0 blocks
==5017==    still reachable: 150 bytes in 4 blocks
==5017==         suppressed: 0 bytes in 0 blocks
==5017== Rerun with --leak-check=full to see details of leaked memory
==5017== 
==5017== For counts of detected and suppressed errors, rerun with: -v
==5017== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```