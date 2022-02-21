/**************************************************************
* Class:  CSC-415-01 Spring 2022
* Name: Duccio Rocca
* Student ID: 922254031
* GitHub UserID: RINO-GAELICO
* Project: Assignment 3 – Simple Shell
*
* File: rocca_duccio_HW3_main.c
*
* Description: A C program that emulates the Shell interface. 
* It reads the input from the user, tokenizes it, and executes the command with the arguments passed with it.
*
**************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>    

#define MAX_ARGS 90 // max number of args 
#define MAX_BUFF 180 // max number of characters for the buffer
#define TOKEN_DELIM " \n\t" // delimiters for tokenizer

char* readInput(void);
char** lineTokenizer(char *);
int creatingProcess(char**);

int main(int argc, char *argv[]){
    

    char *line;
    char **args;
    int status = 1;


    do
    {   
        // argv[1] is my prompt
        //if no prompt specified, ">" is the default prompt
        if(argv[1]!= NULL){
            printf("%s",argv[1]);
        }else{
            printf("> ");
        }
        
        line = readInput();
        fflush(stdout);
        args = lineTokenizer(line);

    

        // An empty command was entered.
        if (args[0] == NULL) { 
        printf("ERROR: The prompt is empty. Enter a new input\n");
        continue;
        }else if(strcmp(args[0], "exit") == 0){//check if exit is entered
            free(line);
            free(args);
            exit(EXIT_SUCCESS); //if exit is entered we exit successfullysss
        }

        status = creatingProcess(args);

        free(line);
        free(args);

    } while (status);

    return 0;
}

char* readInput(void){

    char *inputUser = malloc(MAX_BUFF);//180*1 MAX_BUFF * sizeof(char)
    int charCounter = 0;
	char character;


    if (inputUser == NULL){
        printf("malloc not successfull\n");
        exit(-1);
    }


    if(fgets(inputUser,MAX_BUFF,stdin)==NULL){
            if(feof(stdin)){
                // encountered EOF, exit gracefully 
                printf("\n");
                exit(EXIT_SUCCESS);
            }else{
                printf("Value of errno: %d\n ", errno);
                printf("The error message is : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
    }else{
        // adding a conditionals in case the input is not typed from the terminal
        // makes it easier to read the final output
        if(!isatty(fileno(stdin))){
            printf("%s",inputUser);
        }

        char *pointer;
         //check if there's a newline
        if(pointer=strchr(inputUser, '\n')){
            *pointer = 0;
        } else {
            //flushing the rest of what is in stdin
            scanf("%*[^\n]");//scan everything else left till newline and discard
            scanf("%*c"); // scan newline and discard
            
        }
        
        return inputUser;
    }

    

}

char** lineTokenizer(char * line){

    int tokenCounter = 0;
    //allocate memory for 90 substrings
    char **args = malloc(MAX_ARGS * sizeof(char*));
    char *argument;

    if (!args) {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }

    argument = strtok(line, TOKEN_DELIM);
    //we keep going until we find NULL
    while (argument != NULL) {

    args[tokenCounter] = argument;
    tokenCounter++;
    //reset the token holder
    argument = strtok(NULL, TOKEN_DELIM);

    }
    //we are at the end of the collection of substrings so we add a NULL
    args[tokenCounter] = NULL;
    free(argument);
    argument = NULL;
    return args;
}

int creatingProcess(char** args){
    pid_t pid;
    int status;

    
    // fork a child process 
    pid = fork();

    if (pid < 0){ 
        //error occurred
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0){ 
        
        // Child process
        char* filename = args[0];
        
              
        if (execvp(filename,args) == -1) {
        perror("error");
        }
        exit(EXIT_FAILURE);
  
    }
    else{ 
        // parent process 
        // parent will wait for the child to die
        pid_t pidChild = wait(&status);//wait until the child change status then return the child id
        if (WIFEXITED(status)){
            printf("Child Complete; Child PID: %d\n", pidChild);
            printf("Exit status: %d\n", WEXITSTATUS(status));
        }
       
    }


}



