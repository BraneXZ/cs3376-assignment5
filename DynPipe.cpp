//File: DynPipe.cpp
//Author: Fortythree Shiaohongtu
//Date: 04/07/2017
//Puporse: CS 3376
//Description: This program takes in 2 to 5 linux commands and executes them

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv){

	//count number of argument
	int numArgs = argc - 1;
	
	if( numArgs < 2 || numArgs > 5){
		printf("Please enter between 2 to 5 arguments \n");
		return (0);
	}
	
		//assigning the first argument to firstArg
		char *tok = argv[1];	
		tok = strtok(argv[1], " ");
		char *temp = tok;
		
		//create arrays for upcoming args
		char *firstArg[5];
		char *secondArg[5];
		char *thirdArg[5];
		char *fourthArg[5];
		char *fifthArg[5];
		firstArg[0] = temp;
		int count = 1; 

		//seperate spaces within the second arg
		while( tok != NULL){
			tok = strtok(NULL, " ");
			temp = tok;
			firstArg[count] = temp;
			count++;
		}
		
		//assigning the second argument to secondArg
		tok = strtok(argv[2], " ");
		temp = tok;

		secondArg[0] = temp;
		count = 1; 
		//seperate spaces within the second arg
		while( tok != NULL){
			tok = strtok(NULL, " ");
			temp = tok;
			secondArg[count] = temp;
			count++;
		}
		
		//create a third argument if there are 3 arguments
		if(numArgs == 3){
			tok = strtok(argv[3], " ");
			temp = tok;
			
			thirdArg[0] = temp; 
			count = 1;
			//seperate space within the third arg
			while( tok != NULL){
				tok = strtok(NULL, " ");
				temp = tok;
				thirdArg[count] = temp;
				count++;
			}
		}
	
		//do the same if there are 4 or 5 arguments
		if(numArgs == 4){
			tok = strtok(argv[4], " ");
			temp = tok;

			fourthArg[0] = temp;
			count = 1;

			while( tok != NULL){
				tok = strtok(NULL, " ");
				temp = tok;
				fourthArg[count] = temp;
				count++;
			}
		}

		if(numArgs == 5){
			tok = strtok(argv[5], " ");
			temp = tok;

			fifthArg[0] = temp;
			count = 1;

			while( tok != NULL){
				tok = strtok(NULL, " ");
				temp = tok;
				fifthArg[count] = temp;
				count++;
			}
		}
		//create and use up to 4 pipes
		int firstPipe[2];
		int secondPipe[2];
		int thirdPipe[2];
		int fourthPipe[2];
		
		//create and use up to 4 childpid
		int childpid;
		int childpid2;
		int childpid3;
		int childpid4;

		pipe(firstPipe);
		
		if((childpid = fork()) < 0){
			perror("Failed to create a child process");
			exit(1);
		}

		//child process
		else if (childpid ==0)
		{
			//close unused pipe
			close(firstPipe[0]);

			//assigning write end of pipe to stdout
			dup2(firstPipe[1], 1);

			//close used pipe
			close(firstPipe[1]);
			execvp(*firstArg, firstArg);
			exit(0);
		}
		

		//parent process
		//if numArg is greater than 2, create more processes to run the arguments
		else{
			if(numArgs > 2){
				//use second pipe
				pipe(secondPipe);

				if((childpid2 = fork()) < 0){
					perror("Failed ot create a second child process");
					exit(1);
				}
				else if (childpid2 == 0){
					//close unused pipes
					close(firstPipe[1]);

					//assign read end of first pipe to write end of second pipe
					dup2(firstPipe[0], 0);
					
					close(firstPipe[1]);
					close(secondPipe[0]);

					dup2(secondPipe[1], 1);
					
					//close used pipe
					close(firstPipe[0]);
					close(secondPipe[1]);

					execvp(*secondArg, secondArg);
					exit(0);
				}
				//if numArgs is greater than 3, create another process
				if(numArgs > 3){
					pipe(thirdPipe);

					if((childpid3 = fork()) < 0 ){
						perror("Failed to create a third child process");
						exit(1);
					}
					else if(childpid3 == 0){
						//close unused pipes
						close(firstPipe[0]);
						close(firstPipe[1]);
						close(secondPipe[1]);

						//assign read end of second pipe to write end of third
						dup2(secondPipe[0], 0);

						close(thirdPipe[0]);

						dup2(thirdPipe[1], 1);

						close(secondPipe[0]);
						close(thirdPipe[1]);

						execvp(*thirdArg, thirdArg);
						exit(0);
					}
					//if there are 5 arguments
					if(numArgs == 5){
						pipe(fourthPipe);

						if((childpid4 = fork()) < 0 ){
							perror("Failed to create a fourth child process");
							exit(1);
						}
						else if(childpid4 == 0){
							//close unused pipes
							close(firstPipe[0]);
							close(firstPipe[1]);
							close(secondPipe[0]);
							close(secondPipe[1]);
							close(thirdPipe[1]);
							close(fourthPipe[0]);

							//assign read end of third pipe to write end of fourth
							dup2(thirdPipe[0], 0);
							dup2(fourthPipe[1], 1);

							close(fourthPipe[1]);
							close(thirdPipe[0]);

							execvp(*fourthArg, fourthArg);
							exit(0);
						}
						//parent process for 5 argument
						else{
							close(firstPipe[0]);
							close(firstPipe[1]);
							close(secondPipe[0]);
							close(secondPipe[1]);
							close(thirdPipe[0]);
							close(thirdPipe[1]);
							close(fourthPipe[1]);

							dup2(fourthPipe[0], 0);

							close(fourthPipe[0]);
							
							execvp(*fifthArg, fifthArg);
							exit(0);


						}
					}
					//parent process for 4 arguments
					else{
						//close all unused pipes
						close(firstPipe[0]);
						close(firstPipe[1]);
						close(secondPipe[0]);
						close(secondPipe[1]);
						close(thirdPipe[1]);
						
						dup2(thirdPipe[0], 0);
						
						close(thirdPipe[0]);

						execvp(*fourthArg, fourthArg);
						exit(0);

					}

				}
				//else for 3 aguments
				else{
					//close unused pipes
					close(secondPipe[1]);
					close(firstPipe[0]);
					close(firstPipe[1]);

					//assign read end of second pipe to stdin
					dup2(secondPipe[0], 0);

					close(secondPipe[0]);

					execvp(*thirdArg, thirdArg);
					exit(0);

				}
			}
			//else for 2 arguments
			else{
				
			//close unused pipe
			close(firstPipe[1]);

			//assigning read end of pipe to stdin
			dup2(firstPipe[0], 0);

			//close used pipe
			close(firstPipe[0]);
			execvp(*secondArg, secondArg);
			exit(0);
			}
		}
	return(0);
	
}


