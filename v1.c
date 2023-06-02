#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

int main(){

    char* com = (char*) malloc(100 * sizeof(char));
    printf("Entrez la commande a executer \n");
    scanf("%s", com);
    pid_t new_P = fork();
    
    if(new_P == 0){
        //c'est le fils'
        printf("created a new process %d \n", getpid());

        printf(" Executing the command : %s \n",  com );
        //int com_size = sizeof(com) // Taille de com 
                 
        char* path = "/usr/bin/";
        

        int status = execl(path,com,NULL);  

        printf("%d", status);
    }   

     
    else if(new_P > 0){
    //printf("pere %d \n", getpid());    
    }
    //printf("%s \n",com);
    
    
    return 0;
}
// google-chrome

