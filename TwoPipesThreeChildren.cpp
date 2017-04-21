// file: TwoPipesTwoChildren.cpp
// Author: Fortythree Shiaohongtu
// Date: 04/06/2017
// Purpose: CS 3376
// Description: Executes the following pipe command: "ls -ltr | grep 3376 | wc -l" using two pipes and two children

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char **argv){
	int status;
	int childpid;
	int childpid2;
	int childpid3;

	char* cat_args[] = {"ls", "-ltr", NULL};
	char* grep_args[] = {"grep", "3376", NULL};
	char* wc_args[] = {"wc", "-l", NULL};
	
	//create first pipe
	int firstPipe[2];
	pipe(firstPipe);
	

	//fork the first child
	if((childpid = fork()) == -1){
		perror("Error creating a child process");
		exit(1);

	}
	if(childpid ==0) {
		close(firstPipe[0]);
		// replace stdout with write part of the first pipe
		dup2(firstPipe[1], 1);
		
		// close all pipe
		close(firstPipe[1]);
		execvp(*cat_args, cat_args);
		exit(0);
	}
	//create a second child under parent and a second pipe
	int secondPipe[2];
	pipe(secondPipe);
	if((childpid2 = fork()) == -1){
		perror("Error creating a child process");
		exit(1);
	}
	if(childpid2 == 0){
		//close unused pipe
		close(firstPipe[1]);
		//replace stdin with read end of first pipe
		dup2(firstPipe[0], 0);
		
		//close unused pipes
		close(firstPipe[1]);
		close(secondPipe[0]);

		//replace stdout with write end of second pipe
		dup2(secondPipe[1], 1);

		//close used pipe
		close(firstPipe[1]);

		execvp(*grep_args, grep_args);
		exit(0);
	}
	if((childpid3 = fork()) == -1){
		perror("Error creating a child process");
		exit(1);
	}
	if(childpid3 == 0){
		//close unused pipes
		close(firstPipe[1]);
		close(firstPipe[0]);
		close(secondPipe[1]);

		//replace stdin with read end of second pipe
		dup2(secondPipe[0], 0);
		close(secondPipe[0]);

		execvp(*wc_args, wc_args);
		exit(0);
	}
		return(0);
}

