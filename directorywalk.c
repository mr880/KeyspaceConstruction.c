#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "buildindex.h"

void listdir(char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))){
        FILE *file;
        file = fopen(name,"r");
        if(file != NULL){
           buildIndex(file,name);
        }
        else{
            printf("Invaldid file or Directory");
        }
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            if(entry->d_name[0] == '.')
                continue;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2);
        }
        else
        {
          //char * point;
          // if((point = strchr(entry->d_name, '.')) != NULL)
          // {
          //   if(strcmp(point, ".txt") == 0)
          //   {
          //     printf("%*s- %s\n", indent, "", entry->d_name);
          //   }
          // }
          char path[1024];
          if(entry->d_name[0] == '.')
              continue;
          if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
              continue;

          snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
          FILE *file;
          file = fopen(path,"r");

          // if(entry->d_name[0] == '.')
          //     continue;


          printf("%*s- %s\n", indent, "", entry->d_name);
          buildIndex(file,entry->d_name);
        }
    }
    closedir(dir);
}

int main(int argc, char** argv)
{
    if(argc != 3){
        printf("wrong number of arguments\n");
        return 1;
    }
    listdir(argv[2], 0);
    rename(argv[1],"output.xml");
    return 0;
}
