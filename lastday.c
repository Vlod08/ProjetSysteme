#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_PATH 4096


//DEb Etape1***************************************************************************
void copyFile(char* sourceFile, char* destinationFile){

    int srcFd, desFd;
    char buffer[1024];
    size_t buffSize, bytesRead, bytesWrote;
    srcFd = open(sourceFile,O_RDONLY);
    buffSize = sizeof(buffer);
    if(srcFd<0){
        perror("Cant find the source file \n");
        exit(1);
    }    
    else{
        desFd = open(destinationFile,O_CREAT | O_WRONLY);
        if(desFd<0){
            perror("Cant find the destination file \n");
            exit(2);
        }
        else{
            while((bytesRead = read(srcFd, buffer, buffSize)) > 0){
                
                if(bytesWrote = write(desFd, buffer, bytesRead) < 0){
                    perror("error while writing \n");
                    exit(3);
                }
                
            }

            if(bytesRead < 0){
                 perror("Error while reading \n");
                exit(4);
            }

            struct stat sourceStat;

            if(fstat(srcFd, &sourceStat) < 0 ){
                perror("Error while retrieving the source file information \n");
                exit(7);

            }

            if(fchmod (desFd, sourceStat.st_mode) < 0){
                perror("Error while setting the permissions of the destination file \n");
                exit(8);
            }

            else if(close(srcFd)<0){
                perror("Error while closing the source file \n");
                exit(5);
            }
            else if(close(desFd)<0){
                perror("Error while closing the destination file \n");
                exit(6);
            }


        }
        
    }

}

//Fin Etape1***********************************************************************************************/


void copyDirectory(char* sourceDirectory, char* destinationDirectory){
    
    DIR* srcDirPtr = opendir(sourceDirectory);
    struct dirent* dirEnt;
    struct stat* dirCheck = malloc(sizeof(struct stat));

    if(srcDirPtr == NULL){
        perror("Can't find the source directory");
        exit(9);
    }

    else{
        struct stat fileStat;
        char absSrcFilePath[MAX_PATH] = "";
        char absDesFilePath[MAX_PATH] = "";

        

        while(( dirEnt = readdir(srcDirPtr)) != NULL ){
    		
	    
	    if(strcmp(dirEnt->d_name, ".") == 0 || strcmp(dirEnt->d_name, "..") == 0) {
                continue;
            }
            
            strcat(absSrcFilePath,sourceDirectory);
            
            if(sourceDirectory[sizeof(sourceDirectory)-1] != '/'){
                strcat(absSrcFilePath,"/");
            }
            
            strcat(absSrcFilePath,dirEnt->d_name);
            
            printf("%s",dirEnt->d_name);
            printf("\n");
            
            
            strcat(absDesFilePath,destinationDirectory);
            
            if(destinationDirectory[sizeof(destinationDirectory)-1] != '/'){
                strcat(absDesFilePath,"/");
            }
            
            strcat(absDesFilePath,dirEnt->d_name);
            
            printf("%s",absSrcFilePath);
            printf("\n");
            printf("%s",absDesFilePath);
            printf("\n");

	    stat(absSrcFilePath, dirCheck);
	    
	    if(S_ISDIR(dirCheck->st_mode)){
            
            	mkdir(absDesFilePath,dirCheck->st_mode);
            	copyDirectory(absSrcFilePath,absDesFilePath);
            
            } 
           
            else{
     		copyFile(absSrcFilePath,absDesFilePath);
            }
            		
            memset(absSrcFilePath, 0, sizeof(absSrcFilePath));
            memset(absDesFilePath, 0, sizeof(absDesFilePath));

        }
        
    }

}



int* redirection(char** command, int length){
	
	int in_redir = 0;
	int out_redir = 0;
	int* redir = malloc(3 * sizeof(int));	
	int numLines;
	char** commandCopy = malloc(length * sizeof(char*)); 
	//char** commandCopy = realloc(command, length * sizeof(char*));
	//printf("Checking for redirection \n");
	//int length = sizeof(*command);
	//printf("printing the length : %d \n", length);
	
	for(int i =0; i<length; i++){
	
	
		//printf(" %c \n", command[i]);
	    	if(strcmp(command[i],"<")==0){
	    	
            		printf("found the input command !!!\n");
            		
            		char* in_filename = command[i+1]; 
            		//printf("here1 \n");
            		
            		//substring(filename,command, i+1 , length);			
            		//printf("here2< \n");            
            		
            		int in_file = open(in_filename, O_RDONLY , 0777);
            		printf("Printing the filename :%s*\n",in_filename);
            		/*for(int p = 0;i<strlen(filename); i++){
            			printf("%c\n",filename[i]);
            		}*/
            		if(in_file < 0 ){
            			perror("The open function failed \n");
            		}
            		//printf("No error found \n");
			dup2(in_file,STDIN_FILENO);
			close(in_file);
			
			redir[0] = redir[0]+1;
			redir[1] = in_file;
			numLines = 1;
			char line[MAX_COMMAND_LENGTH];
			
			commandCopy[0] = malloc(strlen(command[0]) + 1);  
			strcpy(commandCopy[0], command[0]);
			 
			while (read(STDIN_FILENO, line, MAX_COMMAND_LENGTH) != 0) {
        			
        			commandCopy[numLines] = malloc(strlen(line) + 1);
        			//printf("%s\n",line);

        			// Copy the line into the new memory block
        			strcpy(commandCopy[numLines], line);

        			// Increase the size of the lines array
        			numLines++;
        			//commandCopy = (char**) realloc(commandCopy, numLines * sizeof(char*) * 100);
        			//printf("No error found line 2\n");

        			// Store the new line in the lines array
        			//commandCopy[numLines - 1] = newLine;
    			}
			
			/*for(int k=0;k<numLines ;k++){
				printf("%s\n",command[k]);
			}*/
			
			//dup2(STDOUT_FILENO,out_file);
			
            		
            
            	}
		
		else if(strcmp(command[i],">")==0){
            		printf("found the > command \n");
            		out_redir = 1;
            		char* out_filename = command[i+1];
            		//printf("here1 \n");
            		//substring(filename,command, i+1, length);			
            		printf("Printing the filename :%s*\n",out_filename);            
            		int out_file = open(out_filename, O_WRONLY  | O_CREAT , 0777);
			if(out_file < 0 ){
            			perror("The open function failed \n");
            		}
			dup2(out_file,STDOUT_FILENO);
			close(out_file);
			redir[0] = redir[0]+2;
			redir[2] = out_file;
			//dup2(STDOUT_FILENO,out_file);
			//return redir;
			
            		//char buffer[100];
            	
            
			//printf("printing the file \n");
			
		
		}
		if (redir[0] == 1 || redir[0] == 3){
			for (int k = 1; k< numLines; k++){
				
				command[1] = (char*) malloc(sizeof(commandCopy[k]));
				//printf("HEre \n");
				strcpy(command[k], commandCopy[k]);
			}
		}
	}
    return redir;
}

int pipes(char** args, int length) {
    int nbPipes = 0;
    int pipeIndices[MAX_COMMAND_LENGTH];
    pipeIndices[0] = -1;

    for (int i = 0; i < length; i++) {
        if (strcmp(args[i], "|") == 0) {
            nbPipes++;
            pipeIndices[nbPipes] = i;
            args[i] = NULL;
        }
    }
    if (strcmp(args[length - 1], "|") == 0) {
        pipeIndices[nbPipes + 1] = length;
        args[length-1] = NULL;
    }

    if (nbPipes == 0) {
        return 0;
    } else {
        printf("Number of pipes: %d\n", nbPipes);
    }

    int start = 0;
    for (int i = 0; i <= nbPipes; i++) {
        int end = pipeIndices[i + 1];
        int fd[2];

        if (i < nbPipes) {
            if (pipe(fd) == -1) {
                perror("Pipe creation failed");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to create child process");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (i > 0) {
                if (dup2(fd[0], STDIN_FILENO) == -1) {
                    perror("Duplication of file descriptor failed");
                    exit(EXIT_FAILURE);
                }
                close(fd[0]);
                close(fd[1]);
            }

            if (i < nbPipes) {
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("Duplication of file descriptor failed");
                    exit(EXIT_FAILURE);
                }
                close(fd[0]);
                close(fd[1]);
            }

            execvp(args[start], args + start);
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            if (i > 0) {
                close(fd[0]);
                close(fd[1]);
            }
            wait(NULL);
            start = end + 1;
        }
    }

    return 1;
}

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
        
        /*******Checking for copying file***********************************************************************************/
        
        if (strcmp(args[0],"cp")==0){
        	copyDirectory(args[1], args[2]);
        	exit(0);
        }
        
        
        /******************************************************************************************************************/
        
        
        /*******Redirection Part *****************************************************************************************/
	
	if (pipes(args, i)==1){
	
		printf("Pipes Created successfully");
		exit(0);
	}
	
	printf("Coming here\n"); 
	
	int redir[3];
	int* redirection_result = redirection(args, i);
	memcpy(redir, redirection_result, 3 * sizeof(int)); 
	
	execvp(args[0], args);
	      
        if(redir[0]==1){
        	dup2(STDIN_FILENO, redir[1]);
        }
        else if(redir[0]==2){
        	dup2(STDOUT_FILENO, redir[2]);
        }
        else if(redir[0]==3){
        	dup2(STDIN_FILENO, redir[1]);
        	dup2(STDOUT_FILENO, redir[2]);
        }
 
       /******************************************************************************************************************/
        
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
        
         else{
            // Exécution de la commande dans un nouveau processus
            //printf("Entering here -------> \n");
            execute_command(command);
        }
    }

    return 0;
}
