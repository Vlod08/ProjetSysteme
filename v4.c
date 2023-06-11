#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

void execute_command(char* command) {
    // Séparation du nom de commande et des arguments
    char* args[MAX_COMMAND_LENGTH];
    char* token = strtok(command, " \n");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;

    // Exécution de la commande
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




int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        // Affichage du prompt
        printf("Entrer la commande a executer : \n");
        
        gets(command);
        printf("printing the command : %s\n", command);
        

        // Vérification de la commande "cd" pour changer de répertoire
        if (strncmp(command, "cd", 2) == 0) {
        
            printf("found the cd command \n");
            char delim[] = " ";
            char* path = strtok(command + 2, delim);
            
            printf("Printing the path : %s\n", path);
            if (path != NULL) {
                printf("Changing to the path : %s\n", path);
                if (chdir(path) != 0) {
                    perror("Erreur lors du changement de répertoire");
                    
                }
                else{
                	execute_command("pwd");
                
                }
            }
        } else {
            // Exécution de la commande dans un nouveau processus
            execute_command(command);
        }
    }

    return 0;
}
