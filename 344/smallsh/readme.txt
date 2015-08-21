readme.txt for smallsh.c
CS344-400
Program 3: smallsh

INSTRUCTIONS

Compile with:
gcc -o smallsh smallsh.c

Or alternatively, use the provided makefile to make then run:
make
./smallsh

Description: Small shell in C. Runs command line instructions. Works similar to
	the bash shell. Supports three built-in commands: exit, cd, and status.
	Also supports comments, beginning with the # character.
	General syntax of the command line:
		command [arg1 arg2 ...] [< input_file] [> output_file] [&]
Grading notes: The built-in commands do not set any exit status (as stated in
	the program instructions after describing the status command), but I have
	included commented-out lines that will set the exit status if needed.
	Background commands should have their standard input redirected from
	/dev/null, otherwise, they would actually try to wait for input. You have
	to press enter afterwards, which is the same action needed in bash (tested
	on eos-class).
	smallsh should allow you to run shell scripts.
	The status command deals with only foreground commands. However, both
	foreground and background processes that are terminated by a signal will
	have a message printed out. When a background process is started, the pid
	is printed out, and when it terminates, the pid and exit status are printed
	out, but just before prompting for a new command.
	CTRL-C only terminates foreground processes, but not the shell nor
	background processes.
	There's not much error checking in the syntax of the command line, which I
	took to mean that the number of supplied arguments should be correct
	each time. However, there is some checking for exit (no arguments allowed),
	and some checking for cd (no or one argument allowed).
	The exit command terminates any leftover background processes with SIGTERM.
	Built-in commands don't support input/output redirection (as stated in the
	assignment requirements).
	Blank lines and comments (beginning with #) are ignored.
	With help from: http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/