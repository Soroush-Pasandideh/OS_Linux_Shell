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

char *inputString;

void clearScrean() {
    printf("\033[H\033[J");
}


bool isWhiteSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

void init_shell() {
    clearScrean();
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
           "***********************");
    char *username = getenv("USER");//get User name
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
}

void printDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// function for parsing command words
void parseSpace(char *str, char **parsed) {
    printf("%d\n", 1);
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

//utility Functions
char *topTenLine(char *fileAddres) {
    FILE *ptr;
    char ch;
    int lines = 0;
    ptr = fopen(fileAddres, "r");

    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    printf("Top ten line of file is : \n");

    ch = fgetc(ptr);
    while (!feof(ptr)) {
        printf("%c", ch);
        if (ch == '\n')
            lines++;
        if (lines >= 10)
            break;
        ch = fgetc(ptr);
    }
    fclose(ptr);

}

void delEmptySpace(char *fileAddres) {
    FILE *ptr;
    char ch[1];
    ptr = fopen(fileAddres, "r");

    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    printf("File text without spaces : \t");

    ch[0] = fgetc(ptr);
    while (!feof(ptr)) {
//        printf("* %c *", ch[0]);
        if (ch[0]!=' ') {
            printf("%c", ch[0]);
        }
        ch[0] = fgetc(ptr);
    }
    fclose(ptr);
}


void runComand(char **parsed);

/*
 0  -> not My Cmd
 1  -> exit
 2  -> cd
-1  -> My_cmd_Completed
*/
int ownCmdHandler(char **parsed) {
    printf("%d\n", 3);
    int NoOfOwnCmds = 9, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;
    char *newCmd;
    char **newParsed;
    char *fileAddres = parsed[1];
    int returnValue = 0;
    pid_t pid, pid2;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "help";
    ListOfOwnCmds[2] = "cd";

    ListOfOwnCmds[3] = "gfwol";//getFirstWordOfLine
    ListOfOwnCmds[4] = "cs";//commonString
    ListOfOwnCmds[5] = "des";//delEmptySpace
    ListOfOwnCmds[6] = "snc";//ShowNotComment
    ListOfOwnCmds[7] = "nor";//NumOfRaw
    ListOfOwnCmds[8] = "s10l";//Show10Line


    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    printf("%d\n", switchOwnArg);

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
        case 4://getFirstWordOfLine
            //todo : grep -Eo '^[^ ]+' baseFileTest.txt
            parsed[3] = parsed[1];
            parsed[0] = "grep";
            parsed[1] = "-Eo";
            parsed[2] = "^[^ ]+";
            break;
        case 5://commonString
            break;
        case 6://delEmptySpace
            delEmptySpace(fileAddres);
            returnValue = -1;
            break;
        case 7://ShowNotComment
            //todo: grep -v "#" a.txt
            parsed[3] = parsed[1];
            parsed[0] = "grep";
            parsed[1] = "-v";
            parsed[2] = "#";
            break;
        case 8://NumOfRaw
            parsed[3] = parsed[1];
            parsed[0] = "grep";
            parsed[1] = "-c";
            parsed[2] = "\n*";
            break;
        case 9://Show10Line
            topTenLine(parsed[1]);
            returnValue = -1;
            break;

        default:
            printf("default");
            break;
    }

    return returnValue;
}

void runComand(char **parsed) {
    pid_t pid = fork();

    if (pid == -1) { // error fork
        printf("\nFailed forking child..");
    } else if (pid == 0) { //child
        int d = ownCmdHandler(parsed);
        printf("\n-----\n%s\n", parsed[0]);
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
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return;
        }

        if (WIFEXITED(status)) {
            const int es = WEXITSTATUS(status);
            if (es == 1)
                exit(0);
            else if (es == 2)
                chdir(parsed[1]);
            else
                printf("ES : %d \n", es);

        }

    }
}


void myMain() {
    char *parsedArgs[MAXLIST];
    char *parsedArgsPiped[MAXLIST];
    int execFlag = 0;

    while (1) {

        printDir();

        inputString = readline(" \n>>>\n ");

        if (strlen(inputString) == 0)
            continue;

        parseSpace(inputString, parsedArgs);

        runComand(parsedArgs);
    }

}

void sig_handler(int signum) {
    myMain();
}

int main() {

    init_shell();

    signal(SIGINT, sig_handler); // Register signal handler


    myMain();


}