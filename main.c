#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//Handler Function Prototype
void handler(int x);

int main(){

 printf("Welcome To The Shell\n");
 
 while(1){
 printf(">>");
 
// Getting String input From the user
  char words[255];
  fgets(words,sizeof(words),stdin);
  
  char *p = strtok(words," \n\t");
  
// i --> array index | background =1 if the command contains "&" , =0 if the command doesn't contain "&" 
  int i=0,background=0,status;
  char *args[15];
  
// Dividing the input String into Tokens to get command and arguments (if they exist) and storing them in args[]
  while (p != NULL){
    args[i++] = p;
    p = strtok(NULL," \n\t");
  }
  
// Checking if the last argument entered is "&" but the input must be more than 1 word
 if(strcmp(args[i-1] , "&") == 0 && i>1){
     i--;   
     background = 1;  
  }  
  
// Making sure that the last element after the arguments is NULL  
  args[i]=NULL;
 
  
// If the user enters command "exit" stop program execution  	
  if(strcmp(args[0],"exit") == 0)
    exit(0);
  
// Catching the execution signal of the child process and calling handler function to handle the interrupt   
  signal(SIGCHLD,handler);

// Creating child process   
  pid_t pid=fork();
 
// If fork failed pid is -1  
  if (pid < 0){
    printf("An Error Has Occured!"); 
  }
 
// If pid=0 Then this is the child process 
  else if (pid == 0){
  // Execute the command Given by the user  
    execvp(args[0],args);
  // if wrong command is entered print error message  
    printf("Execution Failed! (file not found)\n");
    exit(0);
  }
  
// Parent Process
  else {
// if the command doesn't contain "&" Then wait for the child to execute 
  if(background==0)
   waitpid(pid,&status,0);
  }
 }
 return 0;
}

// Handler function opens a log file and print a message in the file everytime a child finishes execution 
void handler(int x){
 FILE *fptr;
 fptr = fopen("logfile.log","a");
 fputs("Child Process is Terminated\n",fptr);
}