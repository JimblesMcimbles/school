/******************************************************************************
smallsh.c
CS344-400
Program 3: smallsh
Small shell in C. Runs command line instructions. Works similar to the bash
	shell. Supports three built-in commands: exit, cd, and status. Also
	supports comments, beginning with the # character.
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
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_CHARS 2048			// maximum 2048 characters per line
#define MAX_ARGS 512			// maximum 512 arguments
#define TOK_DELIM " \t\r\n\a"	// for strtok delimiters

// the struct CmdFlags holds various status flags for the input command, such
//	as the input and output filenames (if specified in the command), a flag for
//	redirecting input or output, the count of arguments, and a flag for if a
//	process is to be run in the background
struct CmdFlags {
	char *inputFile;		// filename of input file
	char *outputFile;		// filename of output file
	int redirInput;			// flag for if input is redirected
	int redirOutput;		// flag for if output is redirected
	int argc;				// keep count of number of arguments provided
	int background;			// check if command is to be run in background
};

char *readLine();
char **splitLine(char *line, struct CmdFlags *cmd);
void launch(char **args, struct CmdFlags *cmd);
void fgProcess(pid_t pid, char *exitStatus);
void left4dead();	// killing zombies, hehehe
// void printTerm();	// used if you want custom termination messages at exit

int main(int argc, char **argv)
{
	int i;				// loop controller
	int exitShell = 0;	// controls shell loop
	pid_t processes[100];	// list of open processes
	int processCount = 0;
	char exitStatus[MAX_CHARS] = "no previous fg process";
		// exit status of last fg process
	struct sigaction defaultAction, ignoreAction;
		// signal event handlers
	// struct sigaction printAction;	// custom termination messages at exit

	// set signal event handlers
	defaultAction.sa_handler = SIG_DFL;
	ignoreAction.sa_handler = SIG_IGN;
	// printAction.sa_handler = printTerm;	// custom term messages at exit
	// prevent interruption of shell and background processes
	sigaction(SIGINT, &ignoreAction, NULL);

	do
	{
		char *line = NULL;		// raw command
		char **args;			// parsed command line
		struct CmdFlags *cmd = malloc(sizeof(struct CmdFlags));
			// manage command flags
		cmd->redirInput = 0;	// reset input redirection flag
		cmd->redirOutput = 0;	// reset output redirection flag
		cmd->argc = 0;			// reset argument count
		cmd->background = 0;	// reset background flag

		// deal with zombies and background processes
		left4dead();

		printf(": ");			// shell prompt

		// get input and parse
		line = readLine();				// read input
		args = splitLine(line, cmd);	// parse input into args and get
			// information on input/output/background

		// check if blank line entered
		if (cmd->argc == 0 || args[0] == NULL)
		{
			exitShell = 0;
			// prompts again
		}

		// check if comment line entered
		else if (strncmp(args[0], "#", 1) == 0)
		{
			exitShell = 0;
			// prompts again
		}

		// built-in command "exit"
		else if (strcmp(args[0], "exit") == 0)
		{
			// check for extraneous arguments
			if (cmd->argc > 2)
			{
				printf("exit: unexpected arguments\n");
				fflush(stdout);
				// does not need to set exit status; uncomment if needed
				// sprintf(exitStatus, "exit value 1");
			}
			else if (args[1])
			{
				printf("exit: unexpected argument '%s'\n", args[1]);
				fflush(stdout);
				// does not need to set exit status; uncomment if needed
				// sprintf(exitStatus, "exit value 1");
			}
			else
			{
				exitShell = 1;
				// does not need to set exit status; uncomment if needed
				// sprintf(exitStatus, "exit value 0");

				printf("killing background processes...\n");
				fflush(stdout);
				// sigaction(SIGTERM, &printAction, NULL);
				for (i = 0; i < processCount; i++)
				{
					kill(processes[processCount], SIGTERM);
				}
			}
		}

		// built-in command "cd"
		else if (strcmp(args[0], "cd") == 0)
		{
			// check for extraneous arguments
			if (cmd->argc > 2)
			{
				fprintf(stderr, "cd: unexpected arguments\n");
				// does not need to set exit status; uncomment if needed
				// sprintf(exitStatus, "exit value 1");
			}
			// if argument is provided (a dir to change to)
			if (args[1])
			{
				// if unsuccessful chdir
				if (chdir(args[1]) != 0)
				{
					fprintf(stderr, "%s: no such file or directory\n",
						args[1]);
					// does not need to set exit status; uncomment if needed
					// sprintf(exitStatus, "exit value 1");
				}
			}
			else
			{
				chdir(getenv("HOME"));
				// does not need to set exit status; uncomment if needed
				// sprintf(exitStatus, "exit value 0");
			}
		}

		// built-in command "status"
		else if (strcmp(args[0], "status") == 0)
		{
			fprintf(stdout, "%s\n", exitStatus);
			fflush(stdout);
			// does not need to set exit status; uncomment if needed
			// sprintf(exitStatus, "exit value 0");
		}

		// execute command
		else
		{
			pid_t cpid;
			cpid = fork();

			if (cpid == 0)				// child process cpid = 0
			{
				if (!cmd->background)	// if foreground process
				{
					// allow interruption of foreground processes with signals
					sigaction(SIGINT, &defaultAction, NULL);
				}

				launch(args, cmd);
			}
			else if (cpid == -1)		// failure to fork
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			else						// parent process
			{
				if (cmd->background)	// background process
				{
					processes[processCount] = cpid;
					processCount++;
					printf("background pid is %d\n", cpid);
					fflush(stdout);
				}
				else					// foreground process
				{
					fgProcess(cpid, exitStatus);
				}
			}
		}

		// deallocate memory
		free(line);
		free(args);
		free(cmd);
	}
	while (!exitShell);

	return 0;
}

/* readLine() reads in a line from stdin with getline, with a maximum size
	specified with the global variable MAX_CHARS. Returns the input line.
	Preconditions: MAX_CHARS is defined globally
	Postconditions: none
	Receives: none
	Returns: input line as a char array */
char *readLine()
{
	char *line = NULL;
	ssize_t buffer = MAX_CHARS;
	getline(&line, &buffer, stdin);

	return line;
}

/* splitLine() takes an input line and a CmdFlags struct and parses the input
	line, setting the appropriate flags in the CmdFlags struct. Determines if
	the provided input command line specifies input/output redirection,
	input/output files, and background processes. Returns the parsed command as
	an array of char arrays.
	Preconditions: MAX_ARGS is defined globally, TOK_DELIM is defined globally
	Postconditions: the command flags are set
	Receives: char array as input line, CmdFlags object
	Returns: array of char arrays as parsed command */
char **splitLine(char *line, struct CmdFlags *cmd)
{
	int bufferSize = MAX_ARGS;
	int readStatus = 1;	// 1 = continue; 0 = stop; 2 = input; 3 = output
	char **tokens = malloc(bufferSize * sizeof(char *));
	char *token;

	cmd->redirInput = 0;
	cmd->redirOutput = 0;
	cmd->argc = 0;
	cmd->background = 0;

	if (!tokens)
	{
		fprintf(stderr, "smallsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	// tokenize the string
	token = strtok(line, TOK_DELIM);
	while (token != NULL && readStatus != 0)
	{
		if (strcmp(token, "&") == 0)		// background process indicated
		{
			cmd->background = 1;
			readStatus = 0;		// end of command
		}
		else if (strcmp(token, "<") == 0)	// input redirection indicated
		{
			cmd->redirInput = 1;
			readStatus = 2;		// set to read in next argument as input file
		}
		else if (strcmp(token, ">") == 0)	// output redirection indicated
		{
			cmd->redirOutput = 1;
			readStatus = 3;		// set to read in next argument as output file
		}
		else	// token is an argument or filename
		{
			if (readStatus == 2)			// previous argument was "<"
			{
				cmd->inputFile = token;
			}
			else if (readStatus == 3)		// previous argument was ">"
			{
				cmd->outputFile = token;
			}
			else
			{
				tokens[cmd->argc] = token;
				cmd->argc++;
				tokens[cmd->argc] = NULL;	// NULL terminate the list of args
			}
		}

		token = strtok(NULL, TOK_DELIM); // get the next token
	}

	// // debugging output
	// printf("arg status: arguments %d, background %d\n", cmd->argc,
	// 	cmd->background);
	// if (cmd->redirInput)
	// {
	// 	printf("input file: %s\n", cmd->inputFile);
	// }
	// if (cmd->redirOutput)
	// {
	// 	printf("output file: %s\n", cmd->outputFile);
	// }

	return tokens;
}

/* launch() executes a provided command line, with additional information in a
	provided CmdFlags struct. Redirects input and/or output if needed.
	Preconditions: appropriate flags set in CmdFlags
	Postconditions: command supplied in args is executed
	Receives: array of char arrays as list of commands, CmdFlags object
	Returns: none */
void launch(char **args, struct CmdFlags *cmd)
{
	int inputfd, outputfd;	// file descriptors
	char fileName[MAX_CHARS] = "";

	// redirect input
	if (cmd->redirInput)
	{
		// open new input file to redirect
		inputfd = open(cmd->inputFile, O_RDONLY);
		if (inputfd == -1)
		{
			fprintf(stderr, "smallsh: cannot open %s for input\n",
				cmd->inputFile);
			exit(EXIT_FAILURE);
		}
		// redirect stdin so fd 0 points to inputfd
		if (dup2(inputfd, 0) == -1)
		{
			fprintf(stderr, "smallsh: cannot open %s for input\n",
				cmd->inputFile);
			exit(EXIT_FAILURE);
		}
	}
	else if (cmd->background)	// if no input file specified, redirect
	{
		inputfd = open("/dev/null", O_RDONLY);
		if (inputfd == -1)
		{
			fprintf(stderr, "smallsh: cannot open /dev/null for input\n");
			exit(EXIT_FAILURE);
		}
		if (dup2(inputfd, 0) == -1)
		{
			fprintf(stderr, "smallsh: cannot open /dev/null for input\n");
			exit(EXIT_FAILURE);
		}
	}

	// redirect output
	if (cmd->redirOutput)
	{
		// open new output file to redirect
		outputfd = open(cmd->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outputfd == -1)
		{
			fprintf(stderr, "smallsh: cannot open %s for output\n",
				cmd->outputFile);
			exit(EXIT_FAILURE);
		}
		// redirect stdout so fd 1 points to outputfd
		if (dup2(outputfd, 1) == -1)
		{
			fprintf(stderr, "smallsh: cannot open %s for output\n",
				cmd->outputFile);
			exit(EXIT_FAILURE);
		}
	}

	execvp(args[0], args);

	// error if command not valid
	fprintf(stderr, "%s: no such file or directory\n", args[0]);
	exit(EXIT_FAILURE);
}

/* fgProcess() handles the status of foreground processes. Sets exit status
	details for the foreground process.
	Preconditions: none
	Postconditions: exitStatus is modified based on process exit status
	Receives: pid_t process id of foreground process, char array as exit status
	Returns: none */
void fgProcess(pid_t pid, char *exitStatus)
{
	int status;

	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
	{
		sprintf(exitStatus, "exit value %d", WEXITSTATUS(status));
	}
	if (WIFSIGNALED(status))
	{
		sprintf(exitStatus, "terminated by signal %d", WTERMSIG(status));
		printf("%s\n", exitStatus);
		fflush(stdout);
	}
}

/* left4dead() handles the status of background processes. Sets exit status
	details for the background process. Named after a video game involving
	killing zombies.
	Preconditions: none
	Postconditions: status is written to stdout
	Receives: none
	Returns: none */
void left4dead()
{
	pid_t cpid;
	int status;

	cpid = waitpid(-1, &status, WNOHANG);
	if (cpid > 0)
	{
		if (WIFEXITED(status))
		{
			printf("background pid %d is done: exit value %d\n", cpid,
				WEXITSTATUS(status));
			fflush(stdout);
		}
		else if (WIFSIGNALED(status))
		{
			printf("background pid %d is done: terminated by signal %d\n",
				cpid, WTERMSIG(status));
			fflush(stdout);
		}
	}
}

/* printTerm() prints out a termination message to stdout. Used with sigaction.
	Preconditions: none
	Postconditions: message is printed to stdout
	Receives: none
	Returns: none */
// void printTerm()
// {
// 	printf("terminated!\n");
// 	fflush(stdout);
// }