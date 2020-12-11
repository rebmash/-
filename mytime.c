#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{ 
  struct timespec tp1;
  struct timespec tp2;
  clock_gettime(CLOCK_REALTIME, &tp1);
  int k = fork();
  if (k == 0)
  {
  
    execvp(argv[1], argv + 1);
    exit(0);
  }
  else
  {
    wait(0);
    clock_gettime(CLOCK_REALTIME, &tp2);
    long int time = ((tp2.tv_sec - tp1.tv_sec) * 1e9 + tp2.tv_nsec - tp1.tv_nsec);
    printf("%ld.%ldms\n", time/1000000, time%1000000);
  }
  
}
