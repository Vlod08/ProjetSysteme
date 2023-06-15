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
    // extracts `n` characters from the source string starting from `beg` index
    // and copy them into the destination string
    while (n > 0)
    {
        *destination = *(source + beg);
 
        destination++;
        source++;
        n--;
    }
 
    // null terminate destination string
    *destination = '\0';
 
    // return the destination string
    return destination;
}



int redirection(char* command){
	
	
    int redir =0;	
	printf("Checking for redirection \n");
	int length = sizeof(command)/sizeof(char);
	
	for(int i =0; i<length; i++){
	
		//printf(" %c \n", command[i]);
		if(command[i] == '<'){
            redir = 1;
            char* filename = (char*) malloc(sizeof(char) * (length - (i+1)));
            //printf("here1 \n");
            substring(filename,command, i+1, length);			
            //printf("here2< \n");            
            int file = open(filename, O_RDONLY , 0777);
			char line[100];
			dup2(file,STDIN_FILENO);
            char buffer[100];
            while(read(file,buffer,100)>0){
                printf("%s \n", buffer);            
            }            
            
            
			
		}
		
		else if(command[i] == '>'){
            redir = 1;
            char* filename = (char*) malloc(sizeof(char) * (length - (i+1)));
            //printf("here1 \n");
            substring(filename,command, i+1, length);			
            //printf("here2< \n");            
            int file = open(filename, O_WRONLY  | O_CREAT , 0777);
		

			dup2(file,STDIN_FILENO);
            //char buffer[100];
            
			printf("printing the file \n");
			
		
	}
    return redir;
}




int main() {
    char command[MAX_COMMAND_LENGTH];
    int run = 1;
    
    while (run) {
        // Affichage du prompt
        printf("Entrer la commande a executer : \n");
        
        gets(command);
        if (strcmp(command,"exit")==0){
        	run = 0;
        	break;
        }
        //printf("printing the command : %s\n", command);
        

        // Vérification de la commande "cd" pour changer de répertoire
        else if (strncmp(command, "cd", 2) == 0) {
        
            //printf("testing the cd command \n");
            char delim[] = " ";
            char* path = strtok(command + 2, delim);
            
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
        }
        else if(redirection(command) == 1){
            continue;        
        }
        
         else {
            // Exécution de la commande dans un nouveau processus
            execute_command(command);
        }
    }

    return 0;
}
