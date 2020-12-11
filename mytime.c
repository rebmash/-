#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[]) {

    struct timespec tp1;
    struct timespec tp2;
    clock_gettime(CLOCK_REALTIME, &tp1);


    int k = fork();
    if (k == 0) {

        //int a = execvp(argv[1], argv + 1);
        char f[1000];
        f[0] = '\0';



        for (int i = 1; i < argc; i++)
        {
            strcat(f, argv[i]);
            if (i != argc - 1)
            {
                strcat(f, " ");
            }
        }
        int a = system(f);
        if (a == -1 || a == 127)
        {
            perror("system");
        }

        exit(0);
    } else {
        wait(0);

        clock_gettime(CLOCK_REALTIME, &tp2);
        double time = ((tp2.tv_sec - tp1.tv_sec) + (tp2.tv_nsec - tp1.tv_nsec) * 1e-9);
        struct tms buf;
        int b = times(&buf);
        if (b == -1)
        {
            perror("times");
        }
        long int a = sysconf(_SC_CLK_TCK); //число тиков в секунду
        //for (int i = 1; i < argc; i++)
        // {
        //    printf("%s ", argv[i]);
        //}
        printf("real    %fs\n", time);
        double t11 = (buf.tms_utime + buf.tms_cutime) / a;
        printf("user    %fs\n", t11);
        double t22 = (buf.tms_stime + buf.tms_cstime) / a;
        printf("sys     %fs\n", t22);
    }
}