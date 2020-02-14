#include <stdio.h>
#include <string.h>
 // EECS: bb1990
// Version: 0.9

#include <fcntl.h>

#include <stdlib.h>

#include <unistd.h>

#include<sys/wait.h>

#define MAXLINE 80 /* The maximum length command */

int main(void) {
  char * args[MAXLINE / 2 + 1]; /* command line arguments */
  char * pipe_args[MAXLINE / 2 + 1];
  int should_run = 1; /* flag to determine when to exit program */
  char string[MAXLINE]; // process
  char history[MAXLINE];
  char filename[MAXLINE];
  char SecCom[MAXLINE]; // second process for piping
  int flag = 0;
  int old_process[MAXLINE];
  int old_options[MAXLINE];
  char * dir = filename;

  int i;
  int j;
  int delete = 0;

  pid_t pid, pid2;
	int count=-1;
  while (should_run == 1) {
	count++;
    delete = 0;
    
    printf("mysh:~$ ");
    fflush(stdout);
	
    fgets(string, MAXLINE, stdin);
    int len = strlen(string);

    // remove new line character
    if (string[len - 1] == '\n')
      string[len - 1] = '\0';

    // restore filename to blank
    if (strcmp("!!", string) != 0) {

      strcpy(filename, "");

    }

  //  printf("history is %s \n" , history);	

    if (strcmp(string, "!!") == 0) {

      if (count==0) {
        printf("No commands in history \n");

      } else {
		  
			 
        strcpy(string, history);
        
	  }

    }
    strcpy(history, string);

    if ((strcmp(string, "exit") == 0) || (strcmp(string, " exit") == 0) || (strcmp(string, " exit ") == 0)) {

      should_run = 0;
      kill(pid, SIGKILL);

    }

    // if the user didnt type exit run the following code
    if (should_run == 1) {

      args[1] = NULL;

      const char seperator = ' ';
      const char amp = '&';
      const char output = '>';
      const char input = '<';
      const char pipetok = '|';
      const char dash = '-';

      char *
        const sep_at = strchr(string, seperator);
      char *
        const amp_at = strchr(string, amp);
      char *
        const output_at = strchr(string, output);
      char *
        const input_at = strchr(string, input);
      char *
        const pipe_at = strchr(string, pipetok);

      if (pipe_at != NULL) {

        // location of pipe in string
        int pipe_first = pipe_at - string;
        // characters from pipe to end of string 
        int SecondArrayLen = len - 2 - pipe_first;
        // turn pipe into empty character which splits string into two    
        * pipe_at = '\0';
        //pointer to dash in first string
        char *
          const dash_at = strchr(string, dash);

        for (i = 0; i < pipe_first; i++) {
          // dont remove the first space in the string	
          if (!(i == 2 && dash_at != NULL)) {
            if (string[i] == ' ') {
              for (j = i; j < len; j++) {

                string[j] = string[j + 1];
              }
              len--;
            }
          }
        }

        //printf("\n The value of string is %s\n" , string);
        //printf("\n The value of args[1] is %s\n" , args[1]);
// pointer to new position of dash after spaces removed
        char *
          const dash_at2 = strchr(string, dash);
// pointer to space in second process		  
        char *
          const space_at = strchr(string, seperator);

// assign process and options to array
        for (i = 0; i < SecondArrayLen; i++) {

          if (dash_at2 == NULL) {

            SecCom[i] = *(pipe_at + i + 1);
          } else {
            SecCom[i] = *(pipe_at + i + 1);

          }

        }

        //printf("\n The value of args[0] is %s\n" , args[0]);
		// Turn space into empty character
		// get options 
        if (dash_at2 != NULL) {

          *space_at = '\0';
          args[1] = space_at + 1;

        }

        
        // get the contents of the second process
        pipe_args[0] = SecCom;

        char *
          const sep_at2 = strchr(SecCom, seperator);
		// get options of process 2
        if (sep_at2 != NULL) {

          *sep_at2 = '\0';
          pipe_args[1] = sep_at2 + 1;

        } else {
          pipe_args[1] = NULL;
        }
        pipe_args[2] = NULL;

        /* printf("\n The value of pipeargs[0] is %s\n" , pipe_args[0]);
        printf("\n The value of pipeargs[1] is %s\n" , pipe_args[1]); */

      }


         
      
	



      if (amp_at != NULL) {
        //printf("%c \n", *(amp_at-1) );

        *(amp_at) = '\0';

        if ( *(amp_at - 1) == ' ') {

          *(amp_at - 1) = '\0';
        }
      }

      if (sep_at != NULL) {
        flag = 1;
        if (pipe_at == NULL) {
          *sep_at = '\0'; /* overwrite first separator, creating two strings. */
          args[1] = sep_at + 1;
        }

      }

      if (output_at != NULL) {
        int pos_output = output_at - string;
        int length = len - 2 - pos_output;

        //      printf("String after removing spaces = %s", string);
        for (i = 0 ; i < len ; i++) {
          if ( *(output_at + i) == ' ') {
            for (j = i ; j < len ; j++) {
              *(output_at + j) = *(output_at + j + 1);
            }
            len--;
          }
        }

			for (i = 0 ; i < length ; i++) {

          filename[i] = *(output_at + i + 1);
        }

        *output_at = '\0'; /* overwrite first separator, creating two strings. */
        args[1] = NULL;
        delete = 1;

      }

      if (input_at != NULL) {
        //printf("File name is %s" , filename);
        int pos_output = input_at - string;
        int length = len - 2 - pos_output;

        for (i = 0 ; i < len ; i++) {
          if ( *(input_at + i) == ' ') {
            for (j = i; j < len; j++) {
              *(input_at + j) = *(input_at + j + 1);
              
            }
            len--;
          }
        }

        for (i = 0 ; i < length ; i++) {

          filename[i] = *(input_at + i + 1);
        }

        //printf("The value of filename is %s " , filename);

        * input_at = '\0'; // overwrite first separator, creating two strings. */
        args[1] = NULL;
        delete = 2;

      }

      //printf(" The value of delete is %d " , delete) ;

      args[0] = string;

      args[2] = NULL;

 
  //printf("\n The value of args[1] is %s\n" , args[1]);
	//printf("\n The value of pipe_args[0] is %s\n" , pipe_args[0]);
  //printf("\n The value of pipe_args[1] is %s\n" , pipe_args[1]);
		 

      if (strcmp(string, "cd") == 0) {

        //printf("The process is cd");
        chdir(args[1]);
      } else {

        int pipefd[2];

        if (pipe_at != NULL) {
			pipe(pipefd);
      
        }
		
		
		
		// execute first process 
        pid = fork();

        if (pid < 0) { //error occurred
          fprintf(stderr, "Fork Failed");
          return 1;
        }

        if (pid == 0) {

          if (flag == 1) {
            setpgid(0, 0); // change permission group so process is leader of new group and seperate from foreground group and wont show in terminal
          }

          if (delete == 1) {

            // creates the file listed by output dir with read/write/executable for user,group and world
  
            int fd1 = open(dir, O_CREAT | O_RDWR, 0777);

            // any write to standard output will be sent to the text file
            
			int fp_stdout= dup2(fd1, STDOUT_FILENO);
			
            //fprintf(stderr,"%d \n" , fp_stdout);

            close(fd1);
            //fprintf(stderr,"fp_dup is : %d", fp_stdout);
            if (fp_stdout != 1) {
              // fprintf(stderr, "Failed to redirect command output.\n");
              return 0;
            }

          } else if (delete == 2) {

            // creates the file listed by output dir (third argument 0 means file)
            int fd0 = open(dir, O_RDONLY, 0);
            //any write to standard output will be sent to the text file
            //fprintf(stderr,"Fd0 is : %d", fd0);
            //fprintf(stderr,"STDIn_FILENO is : %d", STDIN_FILENO);

            int fp_stdin = dup2(fd0, STDIN_FILENO);
            //fprintf(stderr,"fp_dup is : %d", fp_dup_2);
            close(fd0);

            if (fp_stdin == -1) {
              // fprintf(stderr, "Failed to redirect command input.\n");
              return 0;
            }

          }

          if (strcmp(args[0], "ls") == 0) {

            //fprintf(stderr,"First argument is good \n \n");    
          }

          if (args[1]) {
            //fprintf(stderr,"Second argument is not NULL \n \n");    
          }

          close(pipefd[0]);
          dup2(pipefd[1], STDOUT_FILENO); // redirects stdout to writing part of pipe
          close(pipefd[1]);

          
	  
		  

          execvp(args[0], args);

        }
        if (pipe_at != NULL) {
			
          pid2 = fork();

          if (pid2 == 0) {

            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO); // redirects stdin to read from pipe
            close(pipefd[0]);
            execvp(pipe_args[0], pipe_args);

          }
        }
        // parent process
		// sanity close 
        close(pipefd[0]);
        close(pipefd[1]);

        if (pipe_at != NULL) {
          wait(NULL);
        }
        if (amp_at == NULL) {
          wait(NULL); 

        }
        
      }
    }
    flag = 0;

  }
  return 0;

}