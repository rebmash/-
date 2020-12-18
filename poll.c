#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char** argv)
{

    int n = atoi(argv[1]); //amount of children
    


    int child[1000][2];  //pipes, which children will use to send info to parent



    for (int i = 0; i < n; i++)
    {
        pipe(child[i]);
    }


    int parent[1000][2]; //pipes for parent to get info from child
    for (int i = 0; i < n; i++)
    {
        pipe(parent[i]);
    }


    struct pollfd fds_c[1000]; //poll for children
    for (int i = 0; i < n; i++)
    {
        fds_c[i].fd = child[i][0]; //poll for parent to check if we can read form child
        fds_c[i].events = POLLIN | POLLNVAL | POLLERR;
    }


    struct pollfd fds_p[1000]; //poll for parent
    for (int i = 0; i < n; i++)
    {
        fds_p[i].fd = parent[i][0]; //poll for child to check if we can read from parent
        fds_p[i].events = POLLIN | POLLNVAL | POLLERR;
    }
    int k = 1;
    for (int i = 0; i < n; i++)
    {
        if (k != 0)
        {
            k = fork();
        }
        if (k == 0)
        {
            if (i == 0)
            {
                int fd = open(argv[2], O_RDONLY);
                printf("fd - %d\n", fd);
                close(parent[0][0]);
                parent[0][0] = fd;
            }

            if (i == n - 1)
            {
                int fd = open(argv[3], O_WRONLY);
                printf("fd - %d\n", fd);
                close(child[n-1][0]);
                child[n-1][0] = fd;
            }


            poll(fds_p + i, 1, -1);
            printf("child's number %d\n", i);


            char buf1[1000000];
            long int buf1_size = 1000000;

            int file_size1 = read(parent[i][0], &buf1, buf1_size);
            if (i != n - 1)
            {
                write(child[i][1], buf1, file_size1);
            }
            else
            {
                printf("the last process is writing\n");
                int fd = open(argv[3], O_WRONLY);
                write(fd, buf1, file_size1);
                close(fd);
            }
            close(child[i][0]);
            close(child[i][1]);
            close(parent[i][0]);
            close(parent[i][1]);
            return 0;

        }
    }


    if (k != 0)
    {
        while (1)
        {
            if (n == 1)
            {
                break;
            }
            poll(fds_c, n, -1);
            printf("parent ready to read\n");
            int flag = 0;
            for (int i = 0; i < n; i++)
            {
                if (fds_c[i].revents & POLLIN)
                {
                    printf("getting info from %d\n", i);
                    char buf[1000000];
                    int buf_size = 1000000;

                    long int file_size = read(child[i][0], &buf, buf_size);

                    write(parent[i+1][1], buf, file_size);
                    if (i == n - 2)
                    {
                        flag = 1;
                    }

                }
            }
            if (flag)
            {
                break;
            }

        }
    }






}