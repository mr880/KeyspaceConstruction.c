#include"xmlwrite.h"
#include<errno.h>
int xmlwrite(char *token, int count, char *filename){
    printf("in xmlwrite");
    FILE *output;
    FILE *tmpoutput;
    output = fopen("tmpout.xml", "r");
    //rewind(output);
    size_t *derp = (size_t *)malloc(sizeof(size_t));
    tmpoutput = fopen("tmpcopy.xml","w");
    char *currentline = NULL;
    if(output == NULL){
        //perror("i hate everything because");
        //printf("%d",errno);
        output = fopen("tmpout.xml","a+");
        fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<fileIndex>\n</fileIndex>",output);
        //fclose(output);
        //output = fopen("tmpout.xml","r");
	    rewind(output);
    }
    getline(&currentline,derp,output);
    while(currentline){
	    printf("%s",currentline);
        if(strstr(currentline,"<word text=")){
            char *index;
            index = (char*)malloc(strlen(currentline-13));
            strcpy(index, &currentline[12]);
            *(index+(strlen(index)-2)) = '\0';
            printf("%s",index);
            int cmp = strcmp(currentline, index);
            if(cmp == 0){
                //word is indexed add file to the list
                fputs(currentline,tmpoutput);
                free(currentline);
                currentline = NULL;
                getline(&currentline,derp,output);
                fpos_t* mark, *linestart;
                while(strstr(currentline,"<file name")){
                    int i = 0;
                    fgetpos(tmpoutput, linestart);
                    while(currentline[i] != '>')
                        i++;
                    int currcount = atoi(&currentline[i]);//,NULL,10);
                    char* currfile = malloc(strlen(currentline));
                    strcpy(currfile, &currentline[14]);
                    currfile[i-2] = '\0';
                    
                    if(strcmp(currfile, filename) == 0){
                        
                        count += currcount;
                        sprintf(currentline,"\t\t<file name=\"%s\">%d</file>\n",filename,count);
                        fputs(currentline, tmpoutput);
                        if(mark != 0){
                            fsetpos(tmpoutput, mark-1);
                            char curr;
                            curr = fgetc(tmpoutput);
                            while(curr != '\n'){
                                fputc('\0', tmpoutput);  
                            }
                        }
                        break;
                    }
                    else if(count < currcount){
                        mark = linestart;
                        char *line = malloc(sizeof(char)*14+strlen(filename)+sizeof(char)*2+sizeof(int)+sizeof(char)*9);
                        sprintf(line,"\t\t<file name=\"%s\">%d</file>\n",filename,count);
                        fputs(line, tmpoutput);
                        fputs(currentline,tmpoutput);
                        
                    }
                    getline(&currentline,derp,output);   
                }

                
            }
            else if(cmp > 0){
                //word goes next in index
                char *line = (char*)malloc(sizeof(char)*13+strlen(token)+sizeof(char)*4);
                sprintf(line,"\t<word text=\"%s\">\n",token);
                fputs(line, tmpoutput);
                line = realloc(line, sizeof(char)*14+strlen(filename)+sizeof(char)*2+sizeof(int)+sizeof(char)*9);
                sprintf(line,"\t\t<file name=\"%s\">%d</file>\n",filename,count);
                fputs(line, tmpoutput);
                fputs("\t</word>\n", tmpoutput);
                fputs(currentline,tmpoutput);
                free(line);
                fflush(tmpoutput);
            }
            free(index);
        }
	    else if(strstr(currentline,"</fileIndex>")){
		        char *line = (char*)malloc(sizeof(char)*13+strlen(token)+sizeof(char)*4);
                //printf("the loop should end\n");
                sprintf(line,"\t<word text=\"%s\">\n",token);
                fputs(line, tmpoutput);
                line = realloc(line, sizeof(char)*14+strlen(filename)+sizeof(char)*2+sizeof(int)+sizeof(char)*9);
                sprintf(line,"\t\t<file name=\"%s\">%d</file>\n",filename,count);
                printf("%s",line);
            
                fputs(line, tmpoutput);
                fputs("\t</word>\n", tmpoutput);
                fputs(currentline,tmpoutput);
                free(line);
		        break;
	    }
        else{
            //printf("just copying %s\n",currentline);
            fputs(currentline,tmpoutput);
        }
	    getline(&currentline,derp,output);
        //currentline == NULL;
    }
    free(currentline);
    fclose(output);
    fclose(tmpoutput);
    //free(currentline);
    rename("tmpout.xml","tmpcopy.xml");
    return 0;
}
