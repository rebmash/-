
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
int id;
int passangers;
int ramp;
int board;
int voyages;
sem_t *sem0, *sem1, *sem2, *sem3, *sem4, *sem5;
//sem0 - amount of passengers who got to queue
//sem1 - amount of passengers who got on ramp
//sem2 - amount of passengers who got on the bord
//sem3 - voyage is started or not
//sem4 -voyage is finished or not


int passenger(int i)
{

    struct sembuf a;
    while(1)
    {

        sem_wait(sem0); //Passenger in queue
        perror("sem_wait");
        printf("Pas %d: I'm in the queue\n", i);


        sem_wait(sem1); //Passenger getting on trap
        printf("Pas %d: I'm on the trap\n", i);
        sync();


        sem_post(sem1); //Leaving trap
        printf("Pas %d: I'm out of the trap\n", i);
        sync();


        sem_post(sem2); //Getting on board
        printf("Pas %d: I'm on the board\n", i);
        sync();

        //a.sem_num = 3;
        //a.sem_op = 0; //Началось ли путешествие
        //semop(id, &a, 1);
        sem_wait(sem3);

        //a.sem_num = 4;
        //a.sem_op = 0; //Закончилось ли путешествие
        //semop(id, &a, 1);
        sem_wait(sem4);
        //a.sem_num = 1;
        //a.sem_op = -1; //Пассажир пытается пройти на трап
        //semop(id, &a, 1);
        sem_wait(sem1);
        printf("Pas %d: I left the board, I'm on trap\n", i);
        sync();

        //a.sem_num = 1;
       // a.sem_op = 1; //Пассажир сходит с трапа
       // semop(id, &a, 1);
        sem_post(sem1);

        
        printf("Pas %d: I left the trap, Bye!\n", i);
        sync();
        //a.sem_num = 2;
        //a.sem_op = 1; //выходим из очереди
        //semop(id, &a, 1);
        sem_post(sem2);

    }
    printf("pass");
    //finish
    return 0;
}



int cap()
{
    struct sembuf a;

    printf("Cap: trap  down\n");
    sync();
    for (int i = 0; i < voyages; i ++)
    {

       for (int i = 0; i < ramp; i++)
       {
           perror("sem_post");
           sem_post(sem1);
       }

        for (int i = 0; i < board; i++)
        {
            sem_post(sem0);
        }

        printf("Cap: READYYYY FOR ANOTHER ONE\n");



        for (int i = 0; i < board; i++)
        {
            sem_wait(sem2);
        }


        printf("Cap: the board is full. The trap is up.\n");
        sync();


        for (int i = 0; i < board; i++)
        {
            sem_post(sem3);
        }

        printf("Cap: the voyage is started.\n");
        sync();

        printf("Cap: the voyage is finished. The trap is down\n");
        sync();


        for (int i = 0; i < board; i++)
        {
            sem_post(sem4);
        }



        for (int i = 0; i < board; i++)
        {
            sem_wait(sem2);
        }

        printf("Cap: Everybody went out\n");
        sync();

        for (int i = 0; i < ramp; i++)
        {
            sem_wait(sem1);
        }
        printf("Cap: Finished! \n");
        sync();

        //finish
    }
    printf("kek");
    sync();
    return 0;
}


int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

//sem0 - amount of passengers who got to queue
//sem1 - amount of passengers who got on ramp
//sem2 - amount of passengers who got on the bord
//sem3 - voyage is started or not
//sem4 -voyage is finished or not
int main(int ac, char *av[])
{
    //id = semget(IPC_PRIVATE, 6, IPC_CREAT|0777);
    sem0 = sem_open("1", O_CREAT, 0644, 0);
    sem1 = sem_open("2", O_CREAT, 0644, 0);
    sem2 = sem_open("3", O_CREAT, 0644, 0);
    sem3 = sem_open("4", O_CREAT, 0644, 0);
    sem4 = sem_open("5", O_CREAT, 0644, 0);
    sem5 = sem_open("6", O_CREAT, 0644, 0);
    perror("sem_open");
    //sem_init(&sem1, 0, 0);
    //sem_init(&sem2, 0, 0);
    //sem_init(&sem3, 0, 0);
    //sem_init(&sem4, 0, 0);
    //sem_init(&sem5, 0, 0);



    //arg.val = 0 ;
    //semctl(id, 0, SETVAL, arg);
    //semctl(id, 1, SETVAL, arg);
    //semctl(id, 2, SETVAL, arg);
    //semctl(id, 3, SETVAL, arg);
    //semctl(id, 4, SETVAL, arg);


    passangers = atoi(av[1]);
    board = atoi(av[2]);
    ramp = atoi(av[3]);
    voyages = atoi(av[4]);
    board = min(passangers, board);
    int j;




    j = fork();
    if (j != 0)
    {
        int r = 1;
        for (int i = 0; i < passangers; i++)
        {
            if (r != 0)
            {
                r = fork();
            }
            if (r == 0)
            {
                passenger(i);
                return(0);
            }
        }

    }
    else
    {
        cap();
        return(0);
    }
    wait(0);
    int value;
    sem_getvalue(sem0, &value);
    printf("val0 - %d\n", value);
    sem_getvalue(sem1, &value);
    printf("val1 - %d\n", value);
    sem_getvalue(sem2, &value);
    printf("val2 - %d\n", value);
    sem_getvalue(sem3, &value);
    printf("val3 - %d\n", value);
    sem_getvalue(sem4, &value);
    printf("val4 - %d\n", value);
    sem_getvalue(sem5, &value);
    printf("val5 - %d\n", value);
}
