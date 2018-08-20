#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"xmlwrite.h"

int xmlwrite(Node_t Head, char* indexedfile){
	FILE *file;
	if(!Head)
		return 1;
	//try to open the output file
	//if it doesn't exist create it
	//if it still doesn't open exit with error
	file = fopen("output.xml","r");
	if(file == NULL){
		createout();
		file = fopen("output.xml","r");
		if(file == NULL){
			printf("could not create file");
			return -1;
		}
	}
	FILE *tmpfile = fopen("tmp","w");
	//copy the first 2 header lines to the tmp file
	char *currline = malloc(sizeof(char)*100);
	fputs(fgets(currline,100,file),tmpfile);
	fputs(fgets(currline,100,file),tmpfile);
	fpos_t start;
	fgetpos(file,&start);
	*currline = '\0';
		while(!feof(file)){

			currline = getNextLine(file);
			printf("%s\n",currline);
			
			//if the current line contains an indexed word
			if(strstr(currline,"<word text=")){
				//extract the index word
				char *index = malloc(strlen(currline-13));
                strcpy(index,&currline[12]);
				*(index+(strlen(index)-2)) = '\0';
				int test = strcmp(index,Head->token);
                //printf("strcmp = %d for index %s and token %s",test,index,Head->token);
				if(test == 0){
					//the token is already indexed
					fputs(currline, tmpfile);
					addFileToIndex(file, tmpfile, Head->token,Head->counter);
				}
				else if (test>0){
					//the token should be indexed immediate above current line
					//adds the index word line
					fputs("\t<word text=\"", tmpfile);
					fputs(Head->token, tmpfile);
					fputs("\">\n", tmpfile);
					//add the file and count line
					fputs("\t\t<file name=\"", tmpfile);
					fputs(indexedfile, tmpfile);
					fputs("\">", tmpfile);
					char temp[Head->counter];
					sprintf(temp,"%d",Head->counter);
					fputs(temp, tmpfile);
					fputs("</file>\n", tmpfile);
                    fputs("\t</word>",tmpfile);
					fputs(currline, tmpfile);
				}
                free(index);
			}
			else if(strstr(currline,"</fileIndex>")){
					//the token should be indexed immediate above current line
					//adds the index word line
					fputs("\t<word text=\"", tmpfile);
					fputs(Head->token, tmpfile);
					fputs("\">\n", tmpfile);
					//add the file and count line
					fputs("\t\t<file name=\"", tmpfile);
					fputs(indexedfile, tmpfile);
					fputs("\">", tmpfile);
					char temp[Head->counter];
					sprintf(temp,"%d",Head->counter);
					fputs(temp, tmpfile);
					fputs("</file>\n", tmpfile);
					fputs(currline, tmpfile);
			}
			else{
				fputs(currline,tmpfile);
			}
            
			//currline = getNextLine(file);
		}
        free(currline);
		fclose(file);
        fclose(tmpfile);
    	rename("tmp","output.xml");
	return 0;
}

char* getNextLine(FILE *file){
	char buffer[20];
	size_t currlen = 0;
	char* currentline = malloc(sizeof(char));
	if(feof(file))
		return NULL;
	while(fgets(buffer,sizeof(buffer),file)){

		char *extra = realloc(currentline, strlen(buffer)+currlen+1);
		if(!extra)
			break;
		currentline = extra;
		strcpy(currentline+currlen,buffer);
		currlen += strlen(buffer);
        free(extra);
        if(strlen(buffer)<sizeof(buffer))
            break;
        
	}
	//free(buffer);
	//free(currlen);
	return currentline;
}
void createout(){
	FILE *file; file = fopen("output.xml","w");
	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<fileIndex>\n</fileIndex>",file);
	fclose(file);
	return;
}

void addFileToIndex(FILE *read, FILE *write, char* filename, int count){
	Node head;
	Node_t tmp = &head;
	Node_t move;
	char* line = getNextLine(read);
	int i, duplicate;
	while(strstr(line,"</word>")){
		i = 12;
		(tmp->token) = &line[12];
		while(line[i]!= '"') i++;
		line[i] = '\0';
		i++;
		while(!isdigit(line[i]))i++;
		char* countc = &line[i];
		while(isdigit(line[i]))i++;
		line[i] = '\0';
		tmp->counter = atoi(countc);
		
		if(filename == tmp->token){
			tmp->counter += count;
			move = tmp;
			duplicate = 1;
		}
		tmp = tmp->next;
		line = getNextLine(read);
	}
	if(duplicate){
		tmp = &head;
		Node_t hold;
		while(tmp != NULL){
			if(tmp->next->counter < move->counter){
				hold = tmp->next;
				tmp->next = move;
				tmp = hold;
			}
			else{
				if(tmp->next == move){
					tmp->next = tmp->next->next;
					move->next = hold;
					break;
				}
			}
		}
	}
	else{
		tmp = &head;
		while(tmp != NULL){
			if(tmp->next->counter > count){
				Node new;
				(&new)->token = filename;
				(&new)->counter = count;
				(&new)->next = tmp->next;
				tmp->next = &new;
				break;
			}
		}
		
	}
	tmp = &head;
	while(tmp != NULL){
		fputs("\t\t<file name=\"", write);
		fputs(tmp->token, write);
		fputs("\">", write);
		char temp[tmp->counter];
		sprintf(temp,"%d",tmp->counter);
		fputs(temp, write);
		free(temp);
		fputs("</file>\n", write);
		tmp = tmp->next;
	}
	fputs(line,write);
}

