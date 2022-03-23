 Blazerid: vmakkapa

 # Table of Contents

- [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [How does it work?](#how-does-it-work)
  - [How to](#how-to) 
  - [Usage](#usage)

## Overview

This basically helps to learn the UNIX signal handling.

## How does it work
$ ./a.out <command to run child process> [args for child process command...]
So the command which we supplied to forkexecvp.c will be executed in a child process using the function-call execvp.
We can interrupt/suspend the child process using SIGINT/SIGTSTP.
As the child process terminates normally, the parent process stops waiting for it.
We can terminate the parent process only using SIGQUIT (Control+/)
In some computer Ctrl + / may not work for SIGQUIT, so we can using kill command in those situations.
 
## How to

complie: 
gcc -wall lab9.c

run
./a.out sleep 10
