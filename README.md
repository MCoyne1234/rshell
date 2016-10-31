# rshell
CS 100: Assignment 2 

[![Build Status](https://travis-ci.org/coverxit/rshell.svg?branch=exec)](https://travis-ci.org/coverxit/rshell)

## License

This project is licensed under [GNU General Public License v3.0](https://github.com/coverxit/rshell/blob/master/LICENSE).

## Introduction

rshell is a command shell written in C++, invoking syscalls `fork`, `execvp`, `wait` etc., with the capacity of:

- Print a command prompt (`user@host: dir $`).
- Handle complex commands connected by `||`, `&&`, or `;`.

The command should have the following form:

```
cmd        = executable [ argumentList ] [ connector cmd ]
connector  = || or && or ;
```

Currently, we have the following built-in commands:

| Command      | Description                                                              |
|--------------|--------------------------------------------------------------------------|
| exit [code]  | Exit rshell with a given code. If `code` is not given, `0` is used.      |
| cd \<dir\>   | Change working directory to a given `dir`. Note that `dir` is required.  |

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
$ git checkout hw2
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

**4. Detect missing command before/after connector.**

For user input such as `ls -al &&`, rshell could detect the missing command.

- `ls -al &&` would print `rshell: bad syntax: missing command after &&`.
- `|| ls -al` would print `rshell: bad syntax: missing command before ||`.
- `ls &&&&&& echo oh` would print `rshell: bad syntax: missing command before &&`.
- `ls &&& echo what` would print `rshell: bad syntax: unexpected token &`.
- `ls &&| mkdir test` would print `rshell: bad syntax: unexpected token |`.

Currently, we treat single `&` and `|` as illegal symbol, that is

- `ls & echo oh` would print `rshell: bad syntax: unexpected token &`.

## Known Issues

**1. Cannot proceed commands or arguments in quotes.**

- `"ls"` would print `rshell: "ls": No such file or directory`
		
- `echo "This is a message in qutoes."` would print `"This is a message in quotes."`

- `echo "ls -al; mkdir test"` would be treated as `echo "ls -al` and `mkdir test"`.

**2. Cannot handle singals.**

Currently, rshell does not handle singals suchs as `Ctrl+C`.

**3. Cannot handle TABs.**

Currently, rshell does not recognize tabs, treating them the same as spaces.

## Valgrind Test

```
[rwu016@hammer rshell]$ valgrind --tool=memcheck bin/rshell
==1208== Memcheck, a memory error detector
==1208== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==1208== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==1208== Command: bin/rshell
==1208== 
rwu016@hammer.cs.ucr.edu: rshell $ git pull
remote: Counting objects: 4, done.
remote: Compressing objects: 100% (1/1), done.
remote: Total 4 (delta 3), reused 4 (delta 3), pack-reused 0
Unpacking objects: 100% (4/4), done.
From https://github.com/coverxit/rshell
   4b3e383..a337bef  exec       -> origin/exec
Updating 4b3e383..a337bef
Fast-forward
 src/main.cpp | 6 +++---
 1 file changed, 3 insertions(+), 3 deletions(-)
rwu016@hammer.cs.ucr.edu: rshell $ make clean
rm -f src/*.o
rm -rf bin
rwu016@hammer.cs.ucr.edu: rshell $ make
g++ -c -Wall -Werror -ansi -pedantic -o src/main.o src/main.cpp
mkdir -p bin
g++  -o bin/rshell src/main.o
rwu016@hammer.cs.ucr.edu: rshell $ exit
==1208== 
==1208== HEAP SUMMARY:
==1208==     in use at exit: 72,704 bytes in 1 blocks
==1208==   total heap usage: 241 allocs, 240 frees, 99,217 bytes allocated
==1208== 
==1208== LEAK SUMMARY:
==1208==    definitely lost: 0 bytes in 0 blocks
==1208==    indirectly lost: 0 bytes in 0 blocks
==1208==      possibly lost: 0 bytes in 0 blocks
==1208==    still reachable: 72,704 bytes in 1 blocks
==1208==         suppressed: 0 bytes in 0 blocks
==1208== Rerun with --leak-check=full to see details of leaked memory
==1208== 
==1208== For counts of detected and suppressed errors, rerun with: -v
==1208== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```