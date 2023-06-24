#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 100

void execute_command(char* command) {
    // on separe la commande (premiere string et les arguments pour la fonction execl)
    char* args[MAX_COMMAND_LENGTH];
    char* token = strtok(command, " \n");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;

    // On fait un process fils pour executer la commande
    pid_t pid = fork();
    if (pid < 0) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Processus fils
        execvp(args[0], args);
        perror("Erreur lors de l'exécution de la commande");
        exit(EXIT_FAILURE);
    } else {
        // Processus parent
        wait(NULL);
    }
}
	

char* substring(char *destination, const char *source, int beg, int n)
{
    // Extracts `n` characters from the source string starting from `beg` index
    // and copy them into the destination string, removing leading and trailing whitespaces
    
    // Skip leading whitespaces
    while (beg < n && (source[beg] == ' ' || source[beg] == '\n')) {
        beg++;
    }
    
    int end = n - 1;
    // Skip trailing whitespaces
    while (end >= beg && (source[end] == ' ' || source[end] == '\n')) {
        end--;
    }
    
    int length = end - beg + 1;  // Length of the substring
    
    // Copy the trimmed substring into the destination string
    strncpy(destination, source + beg, length);
    destination[length] = '\0';  // Null-terminate the destination string
    
    // Return the destination string
    return destination;
}

/*
char* substring(char *destination, const char *source, int beg, int n)
{
    // extracts `n` characters from the source string starting from `beg` index
    // and copy them into the destination string
    while (n > 0)
    {
        *destination = *(source + beg);
        //printf("%d : %c \n",n,*destination);
 
        destination++;
        
        beg++;
        
        n--;
    }
    
 
    // null terminate destination string
    *destination = '\0';
 
    // return the destination string
    return destination;
}
*/


int redirection(char* command, int length){
	
	int redir = 0;	
	//printf("Checking for redirection \n");
	//int length = sizeof(*command);
	//printf("printing the length : %d \n", length);
	
	for(int i =0; i<length; i++){
	
	
		//printf(" %c \n", command[i]);
	    	if(command[i] == '<'){
	    	
            		//printf("found the input command !!!\n");
            		redir = 1;
            		char* filename = (char*) malloc(sizeof(char) * (length - (i+1))); 
            		//printf("here1 \n");
            		
            		substring(filename,command, i+1 , length);			
            		//printf("here2< \n");            
            		
            		int file = open(filename, O_RDONLY , 0777);
            		//printf("Printing the filename :%s*\n",filename);
            		/*for(int p = 0;i<strlen(filename); i++){
            			printf("%c\n",filename[i]);
            		}*/
            		if(file < 0 ){
            			perror("The open function failed \n");
            		}
            		
			dup2(file,STDIN_FILENO);
			close(file);
			
			
            		return redir;      
            
            	}
		
		else if(command[i] == '>'){
            		//printf("found the > command \n");
            		redir = 1;
            		char* filename = (char*) malloc(sizeof(char) * (length - (i+1)));
            		//printf("here1 \n");
            		substring(filename,command, i+1, length);			
            		printf("Printing the filename :%s*\n",filename);            
            		int file = open(filename, O_WRONLY  | O_CREAT , 0777);
			if(file < 0 ){
            			perror("The open function failed \n");
            		}
			dup2(file,STDOUT_FILENO);
			return redir;
			break;
            		//char buffer[100];
            	
            
			//printf("printing the file \n");
			
		
		}
	}
    return redir;
}




int main() {
    char command[MAX_COMMAND_LENGTH];
    int run = 1;
    char o = '\0';
    //printf("***%c****\n",o);
    int input_redir = 0;
    
    while (run) {
        // Affichage du prompt
        //printf("Entrer la commande a executer : \n");
        memset(command, 0, strlen(command));
        
        
        fgets(command, sizeof(command), stdin);
        
        /*if(input_redir==2){
        	read(0,command,100);
        }
        else{
        	fgets(command, sizeof(command), stdin);
        }*/
        
    	//printf("Commande recu :%s\n", command);
    
        if (strcmp(command,"exit") == 0){
        	run = 0;
        	break;
        }
        //printf("printing the command : %s\n", command);
        

        // Vérification de la commande "cd" pour changer de répertoire
        else if (strncmp(command, "cd", 2) == 0) {
        
            //printf("testing the cd command \n");
            //char delim[] = " ";
            //char* path = strtok(command + 2, delim);
            char path[MAX_COMMAND_LENGTH];
            substring(path, command, 2, strlen(command));
            //printf("testing the path : %s\n", path);
            
            if (path != NULL) {
                //printf("Changing to the path : %s\n", path);
                if (chdir(path) != 0) {
                    perror("Erreur lors du changement de répertoire");
                    
                }
                else{
                	
                	execute_command("pwd");
                
                }
            }
            continue;
        }
        input_redir = redirection(command, strlen(command));
        if(input_redir > 0){
            continue;        
        }
        
         else{
            // Exécution de la commande dans un nouveau processus
            //printf("Entering here -------> \n");
            execute_command(command);
        }
    }

    return 0;
}
