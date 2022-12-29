#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>

//ssize_t getline(char **pString, size_t *pInt, FILE *pIobuf);

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define MAX_FILE_NAME 1000


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
void firstWordOfFile(char *fileAddres) {
    FILE *ptr;
    char ch;
    ptr = fopen(fileAddres, "r");

    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    printf("First word of File : \t");

    ch = fgetc(ptr);
    while (!feof(ptr) && !isWhiteSpace(ch)) {
        printf("%c", ch);
        ch = fgetc(ptr);
    }
    fclose(ptr);
}

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
    char result[1];
    ptr = fopen(fileAddres, "r+");

    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    printf("File text without spaces : \t");

    ch[0] = fgetc(ptr);
    strcat(&result[0], &ch[0]);
    printf("%s", result);
    while (!feof(ptr)) {
        if (ch[0] != ' ') {
            printf("%c", ch[0]);
        }
        ch[0] = fgetc(ptr);
    }
    fclose(ptr);
}

int fileWordNum(char *fileAddres) {
    FILE *file;
    int count = 0;
    file = fopen(fileAddres, "r");
    char word[20];
    while (!feof(file)) {
        fscanf(file, "%s", word);
        count++;
    }
    return count;
}

void mostCommonString(char *fileAddres) {
    FILE *file;
    file = fopen(fileAddres, "r");
    if (NULL == file) {
        printf("file can't be opened \n");
    }
    const int numOfWords = fileWordNum(fileAddres);
    char words[numOfWords][30]; //array of strings which contains words
    char word[30];
    int freqOfWords[numOfWords];

    for (int i = 0; i < numOfWords; i++) {
        freqOfWords[i] = 0;
    }

    for (int i = 0; i < numOfWords; i++) {
        fscanf(file, "%s", word);
        strcpy(words[i], word);
    }

    char currentWord[30];
    for (int i = 0; i < numOfWords; i++) {
        strcpy(currentWord, words[i]);
        for (int j = i + 1; j < numOfWords; j++) {
            if (strcmp(currentWord, words[j]) == 0) {
                freqOfWords[i]++;
            }
        }
    }
    int maxFreq = -1;
    int index[numOfWords];
    int count = 0 ;
    for (int i = 0; i < numOfWords; i++) {
        if (freqOfWords[i] > maxFreq) {
            count = 1;
            for (int k = 0; k < numOfWords; k++) {
                index[i] = 0;
            }
            maxFreq = freqOfWords[i];
            index[0] = i;
        } else if (freqOfWords[i] == maxFreq) {
            count++;
            index[count-1] = i;
        }
    }
    printf("most common word is:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d times\n", words[index[i]], maxFreq + 1);
    }

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

    ListOfOwnCmds[3] = "gfwof";//getFirstWordOfFile
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
        case 4://getFirstOfFile
            firstWordOfFile(fileAddres);
            returnValue = -1;
            break;
        case 5://commonString
            mostCommonString(fileAddres);
            returnValue = -1;
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
