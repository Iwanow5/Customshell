Name: Hubert Iwanow

This is my customshell project for the module CSC1021 Operrating Systems.
This manual will help you to understand how to use my customshell and its features.

Getting started need to firstly move inside the /src folder and run the command make, this will compile the code and create the customshell. To now run the customshell, you need to run the command ./customshell inside of the /bin folder.

Internal commands:
These are the commands that are built into my customshell as functions.
commands available:
cd (directory):  changes directory to the directory specified
clr:             clears screen
dir (directory): displays the contents of the directory specified
environ:         shows all the environment variables
echo (...):      displays the provided text
help:            displays the user_manual.txt
pause:           waits for the use to press enter
quit:            quits the shell

Batch file support:
The customshell can also execute batch files if provided. To execute the commands inside of a batch file, simply start the program withe the desired batch file as the argument.

Example: ./customshell batch_file.txt

External commands:
This customshell can also execute external commands(commands outside of the previously listed). To execute simply type in the desired command with any preferred arguments.

Example: sleep 10

Background process execution: The customshell can also execute commands in the background, with the use of the '&' symbol at the end of the command. It will then return to the prompt immediately. This is different from foreground as foreground execution will wait until command is finished before returning to the prompt.

Example: sleep 10 & (this will execute the sleep command in the background)

Program environment: The customshell has the following environment variables from the pareent process: HOME: the current users home directory of. PATH: the path to the directory containing the customshell executable. PWD: the current working directory. USER: the username of the user. SHELL: the path to the customshell executable.

Input/Output redirection: This customshell can also redirect the input and output of commands from/to files. To redirect the input of a command, use the '<' symbol followed by the file name. To redirect the output of a command, use the '>' symbol followed by the file name.
To append the output of a command to a file, use the '>>' symbol followed by the file name.

How it works: When a user enters a command the customshell will create a new process. The execute_redirection function will be called regardless of whether there is a redirection symbol or not. If there is a redirection symbol, the execute_redirection function will choose which redirection to use. Opens the file specified by the redirection symbol. Uses the dup2 function to redirect the input or output of the command to the file.
Execcutes the command with the redirected input or output. Restores the standard input or output of the command and closes the file.

Example: ls > output.txt (this will redirect the output of the ls command to the output.txt file)
Example: echo hello world >> output.txt (this will append the output of hello world to the output.txt file)
Example: sort < input.txt (this will sort the contents of the input.txt file)
