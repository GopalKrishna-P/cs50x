# Implementing basic shell commands in C
#### Video Demo:  <https://www.youtube.com/watch?v=Rg2KZv28zFQ>
#### Description:


### HOW TO RUN:

```
gcc shell.c -o shell

./shell
```

### COMMANDS IMPLEMENTED:
```
(file_path)$ cd <directory_name>
```
changes the current directory to the specific directory
<directory_name> can be absolute or relative path w.r.t currrent directory

```
(file_path)$ ls <directory_name>
```
lists information about files in the given directory
if no <directory_name> is provided, lists files in the present directory
<directory_name> can be absolute or relative path w.r.t currrent directory

```
(file_path)$ rm [-f -r -v] <files/directories> ...
```
-v  print the name of the deleted files
-r  recursively deletes the contents of the directory
-f  forcibly deletes the files without prompt

```
(file_path)$ rmexcept <list_of_files>
```
deletes all the contents of the current directory except the specified files.
(does not remove directories if they are not empty)

```
(file_path)$ <program_name> < <input_file> > <output_file> m
```
<input_file>, <output_file>, m are optional
takes input from <input_file>
takes output from <output_file>
stops the execution on <program_name> if time exceeds 'm' seconds.

```
(file_path)$ history n
```
lists the previous n recent number of commands
if no n is provided prints whole history

```
(file_path)$ issue n
```
runs the nth command in the history

```
(file_path)$ clear
```
clears the terminal screen

```
(file_path)$ exit
```
exits the shell

## Implementation Details

shell.c has three functions and a main function

is_directory(): tells whether a path is a directory or not
recu_del(): deletes all contents of a directory in depth first search pattern
kill_child(): kills the child if time limit exceeds
every command is executed by a child process except 'cd' and 'issue n'

## Lesson taken from this Project

It’s easy to view yourself as “not a real programmer.” There are programs out there that everyone uses, and it’s easy to put their developers on a pedestal. Although developing large software projects isn’t easy, many times the basic idea of that software is quite simple. Implementing it yourself is a fun way to show that you have what it takes to be a real programmer. So, this was an attempt to write a simplistic Unix shell in C, in the hopes to learn the basics.

A shell does three main things in its lifetime.

Initialize: In this step, a typical shell would read and execute its configuration files. These change aspects of the shell’s behavior.
Interpret: Next, the shell reads commands from stdin (which could be interactive, or a file) and executes them.
Terminate: After its commands are executed, the shell executes any shutdown commands, frees up any memory, and terminates.

By executing the program via ./shell we Initialize the shell. In the main() function we have a infinite while loop where user input is expected in every iteration. This cmd supplied by the user is then Interpreted, If the command is to terminate the program i.e exit/quit we break the while loop and return.

Now, we’re really at the heart of what a shell does. Starting processes is the main function of shells. So writing a shell means that you need to know exactly what’s going on with processes and how they start. That’s why I’m going to take us on a short diversion to discuss processes in Unix.

There are only two ways of starting processes on Unix. The first one (which almost doesn’t count) is by being Init. You see, when a Unix computer boots, its kernel is loaded. Once it is loaded and initialized, the kernel starts only one process, which is called Init. This process runs for the entire length of time that the computer is on, and it manages loading up the rest of the processes that you need for your computer to be useful.

Since most programs aren’t Init, that leaves only one practical way for processes to get started: the fork() system call. When this function is called, the operating system makes a duplicate of the process and starts them both running. The original process is called the “parent”, and the new one is called the “child”. fork() returns 0 to the child process, and it returns to the parent the process ID number (PID) of its child. In essence, this means that the only way for new processes is to start is by an existing one duplicating itself.

This might sound like a problem. Typically, when you want to run a new process, you don’t just want another copy of the same program – you want to run a different program. That’s what the exec() system call is all about. It replaces the current running program with an entirely new one. This means that when you call exec, the operating system stops your process, loads up the new program, and starts that one in its place. A process never returns from an exec() call (unless there’s an error).

With these two system calls, we have the building blocks for how most programs are run on Unix. First, an existing process forks itself into two separate ones. Then, the child uses exec() to replace itself with a new program. The parent process can continue doing other things, and it can even keep tabs on its children, using the system call wait().











