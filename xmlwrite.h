#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"buildindex.h"


int xmlwrite(Node_t Head, char* indexedfile);
char* getNextLine(FILE *file);
void createout();
void addFileToIndex(FILE *read, FILE *write, char* filename, int count);