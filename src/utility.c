/*
Name: Hubert Iwanow
Student Number: 23422334

I acknowlledge the DCU Academic Integrity Policy
*/

#include <stdio.h> // used for input and output(ie: printf)
#include <stdlib.h> // used for memory allocation(ie: malloc)
#include <unistd.h> // used for system calls(ie: fork)
#include <string.h> // used for string manipulation(ie: strtok)
#include <sys/types.h> // used for wait(ie: pid_t)
#include <errno.h> // used for error handling(ie: perror)
#include <limits.h> // used for limits(ie: PATH_MAX)
#include <libgen.h> // used for signals(ie: SIGINT)
#include <fcntl.h> // used for file permissions(ie: O_RDONLY)
#include "utility.h" // used for functions(ie: display_prompt)

// function that will run the provided batch file using batch mode
void batch_mode(const char *file)
{
    FILE *batch_file = fopen(file, "r"); // opens the batch file in read mode
    if(file != NULL) // if file is not null, runs the batch file
    {
        char input[MAX_INPUT_SIZE]; // initializes input variable with the max input size
        char *args[MAX_COMMAND_AMOUNT]; // initializes args variable with  the max command amount
        while(fgets(input, MAX_INPUT_SIZE, batch_file) != NULL)
        {
            input[strcspn(input, "\n")] = 0; // removes newline from input
            if(strlen(input) == 0) // if input is empty(ie: enter is pressed with nothing written before), skips to next iteration
            {
                continue; // skips to next iteration
            }
            
            // this will make the input into tokens which are stored as strings in an array
            int i = 0;
            char *token = strtok(input, " ");
            while(token != NULL) // while token is not null, adds token to args array, then moves to next token
            {
                args[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            args[i] = NULL; // sets last element of args array to null

            int background = 0; // initializes background to 0, no background process
            if(i > 0 && strcmp(args[i - 1], "&") == 0) // if last element of args array is &, sets background to 1, and removes & from args array)
            {
                background = 1;
                args[i - 1] = NULL;
            }

            execute_command(args, background); // executes all the commands from the batch file
        }
        fclose(batch_file); // closes the batch file after all commands have been executed
    }

    // if file is null, prints error
    else
    {
        perror("Error opening batch file\n"); // uses errorno library to dislpay error with openning file
        exit(EXIT_FAILURE); // exits program with failure
    }
}


// function that will display the prompt
void display_prompt()
{
    char cwd[MAX_INPUT_SIZE]; // initializes a cwd to store the current directory
    if(getcwd(cwd, sizeof(cwd))) // gets the current directory
    {
        fprintf(stderr, "%s $ ", cwd); // prints the current directory
    }
    else
    {
        printf("CustomShell$"); //if currrent directory not found display this prompt instead
    }
}

// function that will execute the provided command
void execute_command(char **args, int background)
{
    if(args[0] == NULL) //if there is no command, returns nothing and continues to next iteration
    {
        return;
    }

    // if provided command is cd, uses change_directory function, passes in the directory to be changed to
    if(strcmp(args[0], "cd") == 0)
    {
        change_directory(args[1]);
    }

    // if provided command is clr, uses clear_screen function
    else if(strcmp(args[0], "clr") == 0)
    {
        clear_screen();
    }

    // if provided command is dir, uses list_directory function, passes in the directory to be listed
    else if(strcmp(args[0], "dir") == 0)
    {
        list_directory(args[1]);
    }

    // if provided command is environ, uses environment_variables function
    else if(strcmp(args[0], "environ") == 0)
    {
        envi_strings();
    }

    // if provided command is echo, uses echo_prompt function, passes in the prompt to be echoed
    else if(strcmp(args[0], "echo") == 0)
    {
        echo_prompt(args);
    }

    // if provided command is help, uses get_help function
    else if(strcmp(args[0], "help") == 0)
    {
        get_help();
    }

    // if provided commmand is pause, uses pause_program function
    else if(strcmp(args[0], "pause") == 0)
    {
        pause_program();
    }

    // if provided command is quit, uses quit_program function
    else if(strcmp(args[0], "quit") == 0)
    {
        quit_program();
    }

    //if it is not one of the built in commands, executes the command using the execvp function
    else
    {
        pid_t pid = fork(); // creates a child process, whcich will handle external commands, inheritss the same environment as the parent process
        if(pid == 0)
        {
            if(execvp(args[0], args) == -1) // if the child process fails to execute(returns -1)
            {
                perror("execvp"); // Uses errno library to display error message
            }
            exit(EXIT_FAILURE); // exits program with failure
        }

        else if(pid < 0) // if the child process fails to fork, prints error
        {
            perror("Error forking process"); //  Uses errno library to display error message
        }

        else // if the child process is successful, waits for it to finish
        {
            if(!background) // if background is not set, waits for child process to finish
            {    
                wait(NULL); // waits for child process to finish
            }

            else
            {
                //printf("Background process started with PID: %d\n", pid); // prints the PID of the background process
            }
        }
    }
}


//function to change directory
void change_directory(char *directory) 
{
    char cwd[MAX_INPUT_SIZE];
    if(directory == NULL) // if no directory is provided, prints current directory
    {
        if(getcwd(cwd, sizeof(cwd))) // if current directory is found, prints current directory
        {
            printf("Current directory is: %s\n", cwd);
        }
        else
        {
            perror("getcwd"); // uses errno library to display error message
        }
    }

    else if(chdir(directory) != 0) // if directory is not found, prints error
    {
        perror("chdir"); // uses errno library to display error message
    }

    else // if directory is found, changes the pwd to the new directory
    {
        setenv("PWD", directory, 1);
    }
}


//function to clear screen of previous commands/output/etc.
void clear_screen()
{
    printf("\e[1;1H\e[2J"); //this is ANSI code to clear screen
}

//function to list all items in specified directory
void list_directory(char *directory)
{
    if(directory == NULL)
    {
        directory = "."; //if no directory is provided, it lists everything in the current directory
    }
    
    char command[64]; //size of command to list directory
    snprintf(command, sizeof(command), "ls -al %s", directory); //creates a command to list the directory
    system(command); //executes the command

    if(chdir(directory) != 0) //if directory is not found, prints error
    {
        printf("Error changing directory\n");
    }
}


//funtion that lists all environment strings
void envi_strings()
{
    extern char **environ;  //extern is used to access the global variable environ
    char **env = environ; //env is a pointer to the first element of the environ array
    while(*env != NULL) //while the pointer is not null, prints the environment variable
    {
        printf("%s\n", *env); // prints the environment variable
        env++; //moves the pointer to the next element
    }
}


// function that echoes the provided prompt
void echo_prompt(char **args)
{
    for(int i = 1; args[i] != NULL; i++) //as long as the argument is not null, prints the argument
    {
        printf("%s ", args[i]);
    }
    printf("\n"); //prints a new line once finshed
}


// function that will display the user manual(tested on lab machine)
void get_help()
{
    char path[MAX_INPUT_SIZE]; //size of path
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1); //reads the path of the current executable, using proc/self/exe, which is a linux specific
    printf("%zd", len); //testing
    if(len != -1) //if the path is not -1, prints the path
    {
        path[len] = '\0'; // terminates the string with a null character
        char *dir = dirname(path); // gets the directory of the current executable
        char manual_path[MAX_INPUT_SIZE]; //size of manual path
        snprintf(manual_path, sizeof(manual_path), "%s/../manual/user_manual.txt", dir); // creates a path to the manual using the path of the current executable
        char command[MAX_INPUT_SIZE]; //size of command
        snprintf(command, sizeof(command), "cat %s", manual_path); //creates a command to print the manual
        system(command); //executes the command
    }
    else
    {
        perror("readlink"); //uses errno library to display error message
    }
    printf("\n"); // prints a new line once finshed
}


//function that will pause the shell program until enter is pressed by the user
void pause_program()
{
    printf("Press enter to continue");
    while(getchar() != '\n'); //while enter is not pressed, the program will not continue
}


//function that will execute commands with input/output redirection if needed, it is a wrapper for the execute_command function
void execute_redirection(char **args, int background, char *input_file, char *output_file, int append_mode) //uses  the same parameters as execute_command plus input and output files
{
    int fd_input = -1; //ile descriptor for input, set as -1 as easier to check if failed
    int fd_output = -1; //file descriptor for output
    int input_copy = -1; //copy of original input file
    int output_copy = -1; //copy of original output file

    if(input_file != NULL || output_file != NULL) // if input or output file is not null
    {
        input_copy = dup(STDIN_FILENO); // duplicates the standard input file, using dup
        output_copy = dup(STDOUT_FILENO); //duplicates the standard output file, using dup
        if(input_copy == -1 || output_copy == -1) // if the duplication fails
        {
            perror("dup"); //uses errno library to display error message
            return;
        }
    }

    if(input_file != NULL) //this will handle input redirection(ie: "<")
    {
        fd_input = open(input_file, O_RDONLY); //opens the input file in read only mode(using the <fcntl.h> library) assigning it to fd_input
        if(fd_input == -1) //if the file is not found
        {
            perror("opening input"); //uses errno library to display error message
            if(input_copy >= 0)
            {
                close(input_copy);//closes the copied input file
            }
            if(output_copy >= 0)
            {
                close(output_copy);//closes the copied output file
            }
            return;
        }

        if(dup2(fd_input, STDIN_FILENO) == -1)//duplicates the input file to the standard input file, any inputs will be redirected to the input file
        {
            perror("dup2 input");// uses errno library to
            if(input_copy >= 0)// if the copy is opened, closes the file (helps with memory management/memory leaks)
            {
                close(input_copy);// closes the input file
            }
            if(output_copy >= 0)// if the copy is opened, closes the file (helps with memory management/memory leaks)
            {
                close(output_copy);// closes the output file
            }
            return;
        }
    }

    if(output_file != NULL) // this will handle output redirection(ie: ">" and ">>")
    {
        // flags use the fcntl.h library to set the flags for the output file
        int flags = O_WRONLY | O_CREAT; // this will create a write only new file if it does not exist, and open the file if it does exist

        if(append_mode == 1) //if append mode is true, ie: ">>"
        {
            flags |= O_APPEND; // if append mode is true, it will append to the file(ie: adds to the end of the file)
        }
        else // if append mode is false, ie: ">"
        {
            flags |= O_TRUNC; // if append mode is false, it will truncate the file(ie: overwrites the file to 0 bytes)
        }

        fd_output = open(output_file, flags, 0644); //opens the output file(if exists) in write only mode, flags are set to write only, and 0644 is the file permissions
        if(fd_output == -1) // if the file is not found
        {
            perror("opening output"); // uses errno library to display error message
            // if any of the files are opened, closes the file(helps with memory management/memory leaks)
            if(fd_input >= 0)
            {
                close(fd_input);
            }
            if(input_copy >= 0)
            {
                close(input_copy);
            }
            if(output_copy >= 0)
            {
                close(output_copy);
            }
            return;
        }

        if(dup2(fd_output, STDOUT_FILENO) == -1) //duplicates the output file to the standard output file, now any outputs will be redirected to the output file
        {
            perror("dup2 output"); //uses errno library to display error message
            // if any of the files are opened, closes the file(helps with memory management/memory leaks)
            if(fd_output >= 0)
            {
                close(fd_output);
            }
            if(fd_input >= 0)
            {
                close(fd_input);
            }
            if(input_copy >= 0)
            {
                close(input_copy);
            }
            if(output_copy >= 0)
            {
            close(output_copy);
            }
            return;
        }
        
    }

    execute_command(args, background);// executes the command, with the now modified input and output files

    if(input_copy >= 0)// restores original stdin
    {
        dup2(output_copy, STDIN_FILENO);// restores original stdin
        close(input_copy);//closes the duplicate
    }

    if(output_copy >= 0)// restores original stdout
    {
        dup2(output_copy, STDOUT_FILENO);//restores original stdout
        close(output_copy);//closes the duplicate
    }
    
    // closes the file descriptors after all operations are done
    if(fd_input >= 0)
    {
        close(fd_input);
    }
    if(fd_output >= 0)
    {
        close(fd_output);
    }
}


// function that will exit the shell program
void quit_program()
{
    exit(EXIT_SUCCESS); //exits the program using exit_success from stdlib.h library
}