#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

typedef struct _node{
  char* token;
  struct _node * next;
  int counter;
} Node, *Node_t;

int get_file_size(FILE *fp);
void outputData(Node_t nodes);
int search(Node_t nodes, char* word, int len);
int buildIndex(FILE *file, char* filename);
