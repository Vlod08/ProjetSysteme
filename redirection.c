#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>


void redirection(char* command){
	printf("Checking for redirection \n");
	int length = sizeof(command)/sizeof(char);
	
	for(int i =0; i<length; i++){
	
		//printf(" %c \n", command[i]);
		if(command[i] == '<'){
		
			
			int file = open("input.txt", O_RDONLY , 0777);
			char line[100];
			dup2(file,STDIN_FILENO);
			gets(line);
			while(strcmp(line, "")){
				printf("%s \n", line);
				gets(line);
			}
		}
		
		else if(command[i] == '>'){
			int file = open("output.txt", O_WRONLY | O_CREAT, 0777);
			int fId = dup2(file, STDOUT_FILENO);
			close(file);
			
			printf("printing the file \n");
			
		}
	}
	
	
	
	
}

int main (){
	char command[100];
	printf("Enter the command \n");
	gets(command);
	printf("Got the command : %s", command);
	redirection(command);
	return 0;
}
