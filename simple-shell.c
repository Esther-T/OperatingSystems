/*
Author: Esther Tan
Description: Simulates a shell program that creates child processes to run external programs.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	10 /* size of history */

char history[MAX_COMMANDS][MAX_LINE]; //the array used to store history commands.
char display_history [MAX_COMMANDS][MAX_LINE]; 
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

/**
 * Add the most recent command to the history.
 */

 //adds inputBuffer from user to history
 //pre: inputBuffer[] exists
 //post: inputBuffer is added into history and display_history
 //usage: addtohistory(inputBuffer);
void addtohistory(char inputBuffer[]) {

	int j = 0;
	int z = 2;
	// update array"history": add the command to history, strcpy(str1,str2);
	// update array"display_history": remove characters like '\n', '\0' in order to display nicely

if((strncmp(inputBuffer, "history", 7))) //checks if input is "history"
{ //if no, then we add into history buffer
		strcpy(history[command_count % MAX_COMMANDS], inputBuffer);
		command_count ++;
		
		//checks if command_count exceeded 9 commands
			if(command_count > (MAX_COMMANDS - 1)) 
			{
				//push the latest commands to the bottom
				for(int y = 1; z < MAX_COMMANDS; y++)
				{	
					strncpy(display_history[y], display_history[z], MAX_LINE);
					z++;
				}
				//clears the 9th display_history buffer
				for(int i = 0; i < MAX_LINE; i++)
				{
					display_history[MAX_COMMANDS -1][i] = '\0';
				}
				//inserts the new 9th command
				for(int i = 0; i < strlen(inputBuffer); i++)
				{
					if((inputBuffer[i] != '\n') && (inputBuffer[i] != '\0'))
					{
						display_history[MAX_COMMANDS -1][j] = inputBuffer[i];
						j ++;
					}
				}
			}
			else//if lesser than 9 commands
			{
				//inserts the new command 
				for(int i = 0; i < strlen(inputBuffer); i++)
				{
					if((inputBuffer[i] != '\n') && (inputBuffer[i] != '\0'))
					{
						display_history[command_count % MAX_COMMANDS][j] = inputBuffer[i];
						j ++;
					}
				}
				for(; j < MAX_LINE; j++)
				{
					display_history[command_count % MAX_COMMANDS][j] = '\0';
				}
			}
}
	return;
}

/** 
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */
 //pre: inputBuffer, pointer args and pointer background are assigned
 //post: args[] contains content from inputBuffer
 //		background is 1 if & is detected 
 //		function returns 1 when done
 //usage:int shouldrun = setup(inputBuffer, args, background);
int setup(char inputBuffer[], char *args[],int *background)
{
    int length,		/* # of characters in the command line */
	i,				/* loop index for accessing inputBuffer array */
	command_number;	/* index of requested command number */

	//define your local variables here;
	//int markFirst = 0;
	int x = 0;
	int wholeRow = 0;
	int selDigit = 0;
	char* ptrFirst = NULL;
	int index = 0;
	//int z;

	//memset(markFirst, 0, MAX_LINE*sizeof(markFirst[0])); 
    /* read what the user enters on the command line */
	do {
		printf("osh>");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); /* swallow newline characters */


    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */
 
    if ( (length < 0) ) {
		perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
    }
	
	if(command_count > 9)
	{
		//z = 1;
		//for(int y = 0; z < MAX_COMMANDS; y++)
		//{
			//strcpy(display_history[y], display_history[z]);
			//z ++;
		//}
		//index = command_count - MAX_COMMANDS;
		index = MAX_COMMANDS - 1;
	}
	else
	{
		index = command_count;
	}
			
	//if user wants to use history commands
	if (inputBuffer[0] == '!')
	{
		if(inputBuffer[1] == '!')
		{
			if(command_count == 0)
			{
				printf("No commands in history.\n");
			}
			else
			{
				wholeRow = 0;
				
				//replaces the inputBuffer with commands in history 
				strcpy(inputBuffer,history[(command_count - 1) % MAX_COMMANDS]);
				length = strlen(inputBuffer) + 1;
				//gets the latest command
				//prints out all the characters in the whole row 
				while( display_history[index][wholeRow] != '\0')
				{
					printf("%c", display_history[index][wholeRow]);
					wholeRow ++;
				} 
				printf("\n");
			}
		}
		else
		{
			//checks if its a digit
			if(isdigit(inputBuffer[1]))
			{
				selDigit = inputBuffer[1] - 48;
				wholeRow = 0;
				
				//replaces inputBuffer with commands in history 
				command_number = atoi(&inputBuffer[1]);
				strcpy(inputBuffer,history[command_number]);
				length = strlen(inputBuffer) + 1;
				
				while( display_history[selDigit][wholeRow] != '\0')
				{
					printf("%c", display_history[selDigit][wholeRow]);
					wholeRow ++;
				}
				printf("\n");
			}
			else
			{
				printf("No such command in history.\n");
			}
		}
	}
	//adds the command into history
	 addtohistory(inputBuffer); 

	/**
	 * Parse the contents of inputBuffer
	 */
	
    for (i=0;i<length;i++) { 
		/* examine every character in the inputBuffer */
		
        switch (inputBuffer[i]){
			case ' ':
			case '\t' : /* argument separators */
				//fill in your code here, set up args				
			inputBuffer[i] = '\0'; //end of string
			  args[x] = ptrFirst; //feeds ptrFirst into args[x]
			  ptrFirst = NULL; //clears ptrFirst
			  ptrFirst = &inputBuffer[i+1]; //gets address of first character
			x ++;
			break;
			case '\n':  /* should be the final char examined */
				//fill in your code here, set up the last item args[x] == NULL;
				/* no more arguments to this command */	
				inputBuffer[i] = '\0';
				args[x] = ptrFirst;
				ptrFirst = NULL;
				free(ptrFirst);
				args[x + 1] = NULL;
			break;	
	    		default :             /* some other character */
				 //fill in your code here, 
				/* args[i] is a pointer to a string, its value is the address of the first charater of that string
				* You want to track the location of the beginning character of each string. 
				* The location is the first character, which is not '\t', not '\t', and not '\n'
				* You also need check "&". If '&' is detected, setup background flag.
				*/  
				if(inputBuffer[i] != '&')
				{
					if(i == 0)
					{
						//markFirst = i;
						ptrFirst = &inputBuffer[i];
					}
					
				}
				else if(inputBuffer[i] == '&')
				{
					*background = 1;
				}
				break;
			} /* end of switch */ 
		} /* end of for */
		
		/**
	 * Here you finish parsing the input. 
	 * There is one more thing to assure. If we get '&', make sure you don't enter it in the args array
	 */
	x = 0;
	
	while(args[x] != NULL)
	{
		if(!(strncmp(args[x],"&",1)))
		{
			args[x] = NULL; //removes &
		}
		x ++;
	}

	return 1;
	
} /* end of setup routine */
	
	
int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	int background = 0;             	/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	pid_t child;            		/* process id of the child process */
	//int pid;
	//define your local variables here, at the beginning of your program. 
	int j = 0;
	int shouldrun = 1;
	int status;
	
	memset(history, 0, sizeof(history[0][0]) * MAX_COMMANDS * MAX_LINE);
	memset(display_history, 0, sizeof(display_history[0][0])* MAX_COMMANDS * MAX_LINE);

    while (shouldrun){            		/* Program terminates normally inside setup */
		
		background = 0;
		
		shouldrun = setup(inputBuffer,args,&background);       /* get next command */
		
		// fill in your code here Part I
		/* if the user typed in "exit", the shell program will return (or terminate). 
		* "count" is the number of characters we use to compare.    
		*/	
		if(!(strncmp(args[0],"exit",4)) && (args[1] == NULL))
		{
			return 0 ;
		}
		// fill in your code here Part II
		/* if the user typed in "history", the shell program will display the history commands. 
		* you will use "printf" to print the display_history
		* after you display all the array, this command is done. 
		* Your program should go to read again, which means calling the "setup" function.  
		*/
		if(!(strncmp(args[0],"history",7))&& (args[1] == NULL))
		{
			for(int i = command_count; i > 0; i--)
			{	j = 0;
				if(display_history[i][j] != '\0')
				{
					printf("%d\t ", i);
					while(display_history[i][j] != '\0')
					{
						printf("%c", display_history[i][j]);
						j++;
					}
					
					printf("\n");
				}
			}
		}

		if (shouldrun) 
		{
			
			/* creates a duplicate process! */
			
			//here fill in your code
			/* pid<0  error
			*  pid == 0, it is the child process. use the system call execvp(args[0],args);
			*  pid > 0, it is the parent. Here you need consider it is foreground or background
			*/
			if((child = fork()) < 0)
			{
				printf("error while creating child process!\n");
				exit(0);
			}
			else if(child == 0)
			{
				execvp(args[0],args);
				exit(0);
			}
			else 
			{
				//foreground or background
				if(!background)
				{
					waitpid(child, &status, 0);
				}

			}
				
		}
		
		memset(inputBuffer, 0, sizeof(inputBuffer[0])* MAX_LINE);
	}
		
		
		return 0;
		
}
	
	


