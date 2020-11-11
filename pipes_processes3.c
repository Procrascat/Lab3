#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */
//Recieved help from Robby Gamble

int main(int argc, char **argv)
{
  
  int pipefd[2];
  int pipefd2[2];
  int pid;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL, NULL};
 
  
  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
  pipe(pipefd2);

  pid = fork();
  
  if (pid == 0)
    {
      
			
     
      if(fork() == 0) {
        //get read for pipe 2
        dup2(pipefd2[0], 0);
        // close all pipes
        close(pipefd2[1]);
        close(pipefd2[0]);
        close(pipefd[1]);
        close(pipefd[0]);
        execvp("sort", sort_args);
        
        
      }else{
          
        // child gets here and handles "grep Villanova"

          // replace standard input with input part of pipe

          dup2(pipefd[0], 0);
          //get write for pipe 2
          dup2(pipefd2[1], 1);

          
          
          // close all pipes
          close(pipefd2[1]);
          close(pipefd2[0]);
          close(pipefd[1]);
          close(pipefd[0]);
          
          // execute grep

          execvp("grep", grep_args);
      
          
          
        
          
      }
    }
  else
    {
      
        // parent gets here and handles "cat scores"

        // replace standard output with output part of pipe

        dup2(pipefd[1], 1);
        
        
        // close all pipes
        close(pipefd2[1]);
        close(pipefd2[0]);
        close(pipefd[1]);
        close(pipefd[0]);

        // execute cat

        execvp("cat", cat_args);
      
      
    
      
    
    }
        // close all pipes
        close(pipefd2[1]);
        close(pipefd2[0]);
        close(pipefd[1]);
        close(pipefd[0]);
}