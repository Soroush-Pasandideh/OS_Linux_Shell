#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>


#include<signal.h>


#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

char* inputString;

void clearScrean(){
    printf("\033[H\033[J");
} 


bool isWhiteSpace(char ch){
    return ch == ' ' || ch == '\t' || ch == '\n';
}

void init_shell()
{
	clearScrean();
	printf("\n\n\n\n******************"
		"************************");
	printf("\n\n\n\t****MY SHELL****");
	printf("\n\n\t-USE AT YOUR OWN RISK-");
	printf("\n\n\n\n*******************"
		"***********************");
	char* username = getenv("USER");//get User name
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
}

void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    printf("%d\n" , 1);
	int i;

	for (i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}



void runComand(char** parsed);
/*
 0  -> not My Cmd
 1  -> exit
 2  -> cd
-1  -> My_cmd_Compleated
*/
int ownCmdHandler(char** parsed){
    int NoOfOwnCmds = 3, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];
	char* username;
    char* newCmd;
    char** newParsed;
    char* fileAddres = parsed[1];
    int returnValue = 0;
    pid_t pid , pid2;

	ListOfOwnCmds[0] = "exit";
	ListOfOwnCmds[1] = "help";
    ListOfOwnCmds[2] = "cd";

    


	for (i = 0; i < NoOfOwnCmds; i++) {
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}

	switch (switchOwnArg) {
        case 1://exit
            printf("\nGoodbye\n");
            returnValue = 1;
            break;
        case 2://help
            printf("help");
            returnValue = -1;
            break;
        case 3://cd
            returnValue = 2;
            break; 
        
        default:
            break;
	}

	return returnValue;
}

void runComand(char** parsed){
    pid_t pid = fork();

    if (pid == -1) { // error fork
        printf("\nFailed forking child..");
    } else if (pid == 0) { //child 
        int d = ownCmdHandler(parsed);
        printf("\n-----\n%s\n",parsed[0]);
        if (d != 0)
            exit(d);
        else if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        //wait(NULL);

        int status;
        if ( waitpid(pid, &status, 0) == -1 ) {
            perror("waitpid failed");
            return;
        }

        if ( WIFEXITED(status) ) {
            const int es = WEXITSTATUS(status);
            if (es == 1)
                exit(0);
            else if (es == 2)
                chdir(parsed[1]);
            else 
                printf("ES : %d \n" , es);

        }

    }
}




int main(){
    char *parsedArgs[MAXLIST];
	char* parsedArgsPiped[MAXLIST];
	int execFlag = 0;
	init_shell();


	while (1) {

        printDir();

        inputString = readline(" \n>>>\n ");

        if (strlen(inputString) == 0)
			continue;

        parseSpace(inputString, parsedArgs);
        
        runComand(parsedArgs);
    }
}