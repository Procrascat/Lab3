#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

//Received Help from Robby Gamble
int Turn = 0;
void Student(int[]);
void ParentTransaction(int[]);
     

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
     
     
     
     
     
  
     srand(getpid());
     
     if (argc != 3) {
          
          printf("Start Balance: %s $0 \n", argv[1]);
         
          
     }

     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of two integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");
     
     ShmPTR[0] = atoi(argv[1]);
     ShmPTR[1] = atoi(argv[2]);
     
     printf("Server has filled %d %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1]);

     printf("Server is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          Student(ShmPTR);
          printf("here");
          exit(0);
     }else{
         printf("here now");
         ParentTransaction(ShmPTR);
     }

     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}





void Student(int SharedMem[]) {
  
  int account = SharedMem[0];
  int suggestBalance;
  
  for(int counter = 0; counter < 25; counter++) {
    
    sleep(rand()%5);
    while(SharedMem[1] != 1);
    account = SharedMem[0];
    suggestBalance = rand()%50;
    printf("Poor Student needs $%d\n", suggestBalance);
    
    if(suggestBalance <= account) {
      account -= suggestBalance;
      printf("Poor Student: Withdraws $%d / Balance = $%d\n", suggestBalance, account);
    }else if(suggestBalance > account) {
      printf("Poor Student: Not Enough Cash ($%d)\n", account);
    }
    SharedMem[0] = account;
    SharedMem[1] = 0;
  }
  
}

void ParentTransaction(int SharedMem[]) {
  int newBalance;
  int account = SharedMem[0];
  for(int counter = 0; counter < 25; counter++) {
    sleep(rand()%5);

    while(SharedMem[1] != 0);
    account = SharedMem[0];
    if(account <= 100) {
      newBalance = rand()%100;

      if(newBalance % 2 == 0) {
        account += newBalance;
        printf("Dear old Dad: Deposits $%d / Balance = $%d\n", newBalance, account);
      }else{
        printf("Dear old Dad: Doesn't have any money to give\n");
      }
      
    }else{
      printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
    }
    
    SharedMem[0] = account;
    SharedMem[1] = 1;
  }
}

