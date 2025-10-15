/*
Name: Hubert Iwanow
Student Number: 23422334

I acknowlledge the DCU Academic Integrity Policy
*/

#ifndef UTILITY_H
#define UTILITY_H

#define MAX_INPUT_SIZE 1024
#define MAX_COMMAND_AMOUNT 64

void set_shell_envionment(); // function to set the shell environment
void batch_mode(const char *filename); // function to run batch mode
void display_prompt(); // function to display the prompt
void execute_command(char **command, int background); // function to execute a command
void change_directory(char *directory); // function to change directory
void clear_screen(); // function to clear the screen
void list_directory(char *directory); // function to list the directory
void envi_strings(); // function to list the environment variables
void echo_prompt(char **args); // function to echo the prompt
void get_help(); // function to display help
void pause_program(); // function to pause the program
void quit_program(); // function to quit the program
void execute_redirection(char **args, int background, char *input_file, char * output_file, int append_mode); // function to execute a command with redirection(input and output)

#endif