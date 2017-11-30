/*
 * File: hw3.c
 * Course: TCSS 333 – Winter 2016
 * Assignment 3 – Names
 * Copyright 2016 Benjamin Abdipour
 */

#include <stdio.h>
#include <string.h>
#define MAX_FILE 10
#define MAX_NAME_LENGTH 20
#define MAX_RANK_LENGTH 10
#define MAX_NAME 100
#define MAX_NAME_TOTAL 1000

//- A function that will sort the arrays
void sort(char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH], int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH]) {
    int i, j;
    char nameTemp[1][MAX_NAME_LENGTH];
    int rankTemp[1][MAX_RANK_LENGTH];
    int sorted = 0;
    while(!sorted) {
        sorted = 1;
        for(i = 1; i < MAX_NAME_TOTAL && myName[i][0] > 64 && myName[i][0] < 123; i++) {

            if(strcmp(myName[i], myName[i - 1]) < 0) {
                strcpy(nameTemp[0], myName[i - 1]);
                strcpy(myName[i - 1], myName[i]);
                strcpy(myName[i], nameTemp[0]);

                for(j = 0; j < MAX_RANK_LENGTH; j++) {
                    rankTemp[0][j] = myRank[i - 1][j];
                    myRank[i - 1][j] = myRank[i][j];
                    myRank[i][j] = rankTemp[0][j];
                }

                sorted = 0;
            }
        }
    }
}

//- A function that will create the output file
void createOutput(char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH], int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH]) {
    FILE *outfile;
    outfile = fopen("summary.csv", "wb");

    int i, j;
    char years[MAX_FILE][MAX_NAME_LENGTH] = {{"1920"}, {"1930"}, {"1940"}, {"1950"}, {"1960"}, {"1970"},
            {"1980"}, {"1990"}, {"2000"}, {"2010"}};
    //printf("Name,");
    fprintf(outfile, "Name,");
    for(i = 0; i < MAX_FILE; i++) {
        //printf("%s", &years[i][0]);
        fprintf(outfile, "%s", &years[i][0]);

        if(i != MAX_FILE - 1 ) {
            //printf(",");
            fprintf(outfile, ",");
        }
    }
    //printf("\r\n");
    fprintf(outfile, "\r\n");

    for(i = 0; i < MAX_NAME_TOTAL && myName[i][0] > 64 && myName[i][0] < 123; i++) {
        for(j = 0; j < MAX_NAME_LENGTH && myName[i][j] > 64 && myName[i][j] < 123; j++) {
            //printf("%c", myName[i][j]);
            fprintf(outfile, "%c", myName[i][j]);
        }
        //printf(",");
        fprintf(outfile, ",");

        for(j = 0; j < MAX_RANK_LENGTH ; j++) {
            if(myRank[i][j] > 0) {
                //printf("%d", myRank[i][j]);
                fprintf(outfile, "%d", myRank[i][j]);
            }

            if(j != MAX_RANK_LENGTH - 1) {
                //printf(",");
                fprintf(outfile, ",");
            }
        }

        //printf("\r\n");
        fprintf(outfile, "\r\n");
    }

    fclose(outfile);
}

void init(char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH], int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH]) {
    int i, j;
    for(i = 0; i < MAX_NAME_TOTAL; i++) {
        for(j = 0; j < MAX_NAME_LENGTH ; j++) {
            myName[i][j] = '\0';
            myRank[i][j] = '\0';
        }
    }
}

////- A function that will process a single name and rank pair for some year (e.g. Mary, 82,1990)
void processName(const char name[MAX_NAME_LENGTH], const int rank, const int year,
        char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH], int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH]) {
    int i, j, myNameIndex, found;
    found = -1;
    myNameIndex = 0;


    for(i = 0; i < MAX_NAME_TOTAL && myName[i][0] > 64 && myName[i][0] < 123; i++) {
        myNameIndex++;
        for(j = 0; found == -1 && j < MAX_NAME_LENGTH && myName[i][j] == name[j]; j++) {
            if(name[j] == '\0') {
                found = i;
            }
        }
    }

    if(found == -1) {
        for(j = 0; j < MAX_NAME_LENGTH; j++) {
            myName[myNameIndex][j] = name[j];
        }
        myRank[myNameIndex][year] = rank;

    } else {
        myRank[found][year] = rank;
    }
}

//A function that will read in 1 input file
void readFile(const char fileName[], int year, char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH],
        int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH]) {
    int i;
    FILE *infile;
    infile = fopen(&fileName[0], "r");
    char name[MAX_NAME_LENGTH];

    for(i = 0; i < MAX_NAME; i++) {
        fscanf(infile, " %[^,]%*s", name);
        processName(name, i + 1, year, myName, myRank);
    }

    fclose(infile);
}

//A function that will read all 10 input files
void readFiles(const char fileNames[], char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH],
        int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH]) {
    int i;
    for(i = 0; i < MAX_FILE; i++) {
        readFile(&fileNames[i * MAX_NAME_LENGTH], i, myName, myRank);
    }
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char myName[MAX_NAME_TOTAL][MAX_NAME_LENGTH];
    int myRank[MAX_NAME_TOTAL][MAX_RANK_LENGTH];
    init(myName, myRank);
    char fileNames[MAX_FILE][MAX_NAME_LENGTH] = {{"yob1920.txt"}, {"yob1930.txt"}, {"yob1940.txt"},
            {"yob1950.txt"}, {"yob1960.txt"}, {"yob1970.txt"}, {"yob1980.txt"}, {"yob1990.txt"},
            {"yob2000.txt"}, {"yob2010.txt"}};
    readFiles(*fileNames,myName, myRank);
    sort(myName, myRank);
    createOutput(myName, myRank);

    return 0;
}
