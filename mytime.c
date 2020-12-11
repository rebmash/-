#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <pthread.h>
#include <errno.h>
int main(int argc, char* argv[])
{

  struct timespec tp1;
  struct timespec tp2;
  clock_gettime(CLOCK_REALTIME, &tp1);
  int flag = 0;
  int k = fork();
  if (k == 0)
  {
  
    int a = execvp(argv[1], argv + 1);
    if (a == -1)
    {
        perror("zsh");
    }

    exit(0);
  }
  else
  {
    wait(0);

    clock_gettime(CLOCK_REALTIME, &tp2);
    long int time = ((tp2.tv_sec - tp1.tv_sec) * 1e9 + tp2.tv_nsec - tp1.tv_nsec);
    struct tms buf;
    times(&buf);
    long int a = sysconf(_SC_CLK_TCK);
    //for (int i = 1; i < argc; i++)
   // {
    //    printf("%s ", argv[i]);
    //}
    printf("real    %ld,%lds\n", time/1000000000, time%1000000000);
    printf("user    %ld,00s\n", (buf.tms_utime + buf.tms_cutime) * a );
    printf("sys     %ld,00s\n", (buf.tms_stime + buf.tms_cstime) * a);
  }
}
