#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>


//sems description
//sem0 - for clients not to work in one time
//sem1 - addr was updated
//sem2 - addr can be updated


int main()
{
    int key = ftok("server.c", 'A');
    int id = shmget(key, 4096, IPC_CREAT | 0600);
    char str[1000];
    str[0] = '\0';
    str = shmat(id, NULL, 0);

    int sem_key = ftok("server.c", 'B');
    int semid = semget(sem_key, 3, IPC_CREAT|S_IWOTH|S_IROTH|S_IRUSR|S_IWUSR);


    struct sembuf sop;
    union semun arg;


    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    int k = semctl(semid, 0, GETVAL);
    arg.val = 0;
    semctl(semid, 1, SETVAL, arg);
    arg.val = 1;
    semctl(semid, 2, SETVAL, arg);
    k = semctl(semid, 2, GETVAL);
    while (1)
    {

        sop.sem_num = 1;
        sop.sem_op = -1; //check if addr updated
        semop(semid, &sop, 1);


        printf("%s\n", str);

        sop.sem_num = 2; //client can write as we dont use addr anymore
        sop.sem_op = 1;

        semop(semid, &sop, 1);

    }
    shmctl(id, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID, arg);

}