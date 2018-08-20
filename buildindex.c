
#include"xmlwrite.h"


//Node_t firstNode = NULL;
//Node_t lastNode = NULL;
//Node is a struct type
//#Node_t is a pointer to struct type
int get_file_size(FILE *fp){
  int len;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  printf("%d\n", len);
  return len;
}


void outputData(Node_t nodes){
	//Node_t nodes = firstNode;
	//move the node to the first token
	nodes = nodes->next;

	while(nodes != NULL){

		printf("%s :: %d\n", nodes->token, nodes->counter);
		nodes = nodes->next;

	}

}

int search(Node_t nodes, char* word, int len){
	//Node_t nodes = firstNode;
	//move the node to the first token
	//nodes = nodes->next;

  while(nodes != NULL){

		//printf("word: %s :: token: %s\n", word, nodes->token);
    if(strcmp(nodes->token, word) == 0){

      nodes->counter = nodes->counter + 1;

      //printf("Duplicate: \"%s\" >> %d\n", nodes->token, nodes->counter);

      return 1;
    }
    nodes = nodes->next;

  }
  return 0;
}


int buildIndex(FILE *file, char* filename){

  Node Head; //creates a Node statically
  char *token;
  Node_t newNode = (Node_t) malloc(sizeof(Node));
  int iterations = 0;
  char delimitors[] = " .,;:!?-\"[]#$?&^*()@+=_~`{}|/<>\n\'";
  //char * input = argv[1];
  //char *tokens;
  int len;
  //FILE *file;
  //file = fopen(argv[1], "r");

  if(!newNode)
  {
    fprintf(stderr, "node creation failed here\n");
  }
  else
  {
    newNode->token = NULL;
    newNode->next = NULL;
    newNode->counter = 0;
    //firstNode = lastNode = newNode;
  }


  if(file == NULL)
  {
      printf("\n failed to open file");
  }
  else
  {
      len = get_file_size(file);

      //printf("Num Chars: %d\n", (int)len);
	  //char null = '\0';
      char str[len];
      int count = 1;
      Node_t current = &Head;
      while(fgets(str, len, file) != NULL)
      {
        //printf("Line %d: %s\n\n", count, str);
		    //if(count > 1)
			 //outputData(&Head);
  	    token = malloc(strlen(str));
        token = strtok(str,delimitors);
        //newNode->token = strtok(str,delimitors);
        //printf("token: %s\n", token);
        
        while(token != NULL)
        {
          char a = token[0];

          if( (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
          {
            token[0] = tolower(token[0]); //converts to lowercase
            if(iterations >= 1){

              if(search((&Head)->next, token, len) == 1)
              {
                //
              }
              else
              {

                newNode = (Node_t) malloc(sizeof(Node));
  	            newNode->token = (char*)malloc(strlen(token));
                strcpy(newNode->token, token);
                newNode->counter++;
    			      //printf("current token is: %s at address %p\n",newNode->token,newNode->token);
	        newNode->next = NULL;
	        current->next = newNode;

	        current = current->next;
              }
            }
            else{

              newNode = (Node_t) malloc(sizeof(Node));
  	          newNode->token = (char*)malloc(strlen(token));
              strcpy(newNode->token, token);
              newNode->counter++;
  			  //printf("current token is: %s at address %p\n",newNode->token,newNode->token);
	          newNode->next = NULL;
	          current->next = newNode;
	          current = current->next;
              iterations++;
            }

            //lastNode->next = newNode;
            //newNode->next = NULL;
            //lastNode = newNode;
            //printf("%s\n", token);
          }
          //else
            //token = strtok(NULL, delimitors);


          token = strtok(NULL, delimitors);
        }
        //outputData();

        count++;



      }
      
      Node_t tmp = (&Head)->next;
      while(tmp){
      xmlwrite(tmp,filename);
      tmp = tmp->next;
      }
      //outputData(&Head);
      free(newNode);
      free(token);



      //printf("%s\n", tokens);

  }

    return 0;

}
