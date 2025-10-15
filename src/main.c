/*
Name: Hubert Iwanow
*/

#include <stdio.h> // used for input and output
#include <stdlib.h> // used for memory allocation
#include <string.h> // used for string manipulation
#include <sys/stat.h> // used for file permissions
#include "utility.h" // used for utility functions

// these max lengths prevent from using too much memory, and aid performance
#define MAX_COMMAND_AMOUNT 64
#define MAX_INPUT_SIZE 1024

int main(int argc, char *argv[])
{
    char input[MAX_INPUT_SIZE]; // initializes input variable with the max input size
    char *args[MAX_COMMAND_AMOUNT]; /// initializes args variable with  the max command amount

    // this checks if the user has provided a batch file to run
    if(argc == 2)
    {
        batch_mode(argv[1]); // link to batch mode function
        return 0;
    }

    // while loop will run until user puts in the quit command or ctrl+c
    while(1)
    {
        display_prompt(); // displays prompt of current directory
        if(fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            printf("\n"); // creates new line for each command
            break; // breaks out of while loop
        }

        input[strcspn(input, "\n")] = 0; // removes newline from input

        if(strlen(input) == 0) // if input is empty(ie: enter is pressed with nothing written before), skips to next iteration
        {
            continue; // skips to next iteration
        }

        char *input_file = NULL; // this will be used to check if the user has provided an input file
        char *output_file = NULL; // this will be used to check if the user has provided an input or output file
        int append_mode = 0; // this will be used to check if the user wants to append to a file

        // this will make the input into tokens which are stored as strings in an array
        int i = 0;
        char *token = strtok(input, " ");
        while(token != NULL && i < MAX_COMMAND_AMOUNT - 1) // while there still is a token, adds token to args array, then moves to next token
        {
            if(strcmp(token, "<") == 0) // checks if the token is < (ie: input from file)
            {
                token = strtok(NULL, " "); //goes to the next token in array
                if(token != NULL) // if token is not null
                {
                    input_file = strdup(token); // sets input_file to the token
                }
            }
            else if(strcmp(token, ">") == 0) // checks if the token is > (ie: output to file)
            {
                token = strtok(NULL, " "); //goes to the next token in array
                if(token != NULL) // if token is not null
                {
                    output_file = strdup(token); // sets output_file to the token
                    append_mode = 0; // sets append mode to 1
                }
            }

            else if(strcmp(token, ">>") == 0) // checks if the token is >> (ie: append to file)
            {
                token = strtok(NULL, " "); //goes to the next token in array
                if(token != NULL) // if token is not null
                {
                    output_file = strdup(token); // sets output_file to the token
                    append_mode = 1; // sets append mode to 1
                }
            }

            // if the token is not <, > or >>, then it is added to the args array
            else
            {
                args[i] = token;
                i++;
            }
            token = strtok(NULL, " "); // goes to the next token in array
        }

        args[i] = NULL; // sets the last element of args array to NULL

        int background = 0; // this will be used to check if the user wants to run the command in the background
        if(i > 0 && strcmp(args[i -1], "&") == 0) // if the last token is &, then the user wants to run the command in the background
        {
            background = 1; // sets background processes to 1
            args[i - 1] = NULL; // removes the '&' from args array
        }

        execute_redirection(args, background, input_file, output_file, append_mode); // provides all the details to the execute_redirection function

        // this will free the memory allocated to the input_file and output_file strings helps to prevent memory leaks
        if(input_file != NULL)
        {
            free(input_file);
        }
        if(output_file != NULL)
        {
            free(output_file);
        }
    }

    return 0;
}
