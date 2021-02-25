#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define N 5
#define TRUE 1
#define LEFT (x + N - 1) % N
#define RIGHT (x + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
int state[N];
sem_t sem[N];
pthread_t threads[N];
sem_t mutex;

void *philosopher(void *);
void think();
void take_forks(int);
void eat();
void put_forks(int);
void test(int);
void print_table();
void print_states();

int main()
{
    int i;
    sem_init(&mutex, 0, 1); //initialize mutex with 1, will be downed to enter critical region
    for (i = 0; i < N; i++)
        sem_init(&sem[i], 0, 0); //initilize semaphore array with 0
    printf("\t\tWELOME TO DINNER PHILOSOPHERS!\n\n");
    print_table();

    printf("------------------------------------------------\n"
           "Phil 1    Phil 2    Phil 3    Phil 4    Phil 5\n");

    for (i = 0; i < N; i++)
    {
        int *x = (int *)malloc(sizeof(*x));
        *x = i;
        pthread_create(&threads[i], NULL, philosopher, x); 
    }
    for (i = 0; i < N; i++)
        pthread_join(threads[i], NULL);
}

void *philosopher(void *a)
{
    int i = *(int *)a;
    while (TRUE)
    {
        think();
        take_forks(i);
        eat();
        put_forks(i);
    }
}

void think()
{
    int x = 2 + rand() % 3;
    sleep(x);
}
void eat()
{   
    int x = 2 + rand() % 3;
    sleep(x);
}

void take_forks(int x)
{
    sem_wait(&mutex); //Enter Critical Region
    state[x] = HUNGRY;
    print_states();
    test(x);          //test adjacent forks
    sem_post(&mutex); //Exit Critical region
    sem_wait(&sem[x]);
}
void put_forks(int x)
{
    sem_wait(&mutex);    //Enter Critical Region
    state[x] = THINKING; //Eating finished
    print_states();
    test(LEFT);  //Check if left can eat
    test(RIGHT); //Check if right can eat
    sem_post(&mutex);
}
void test(int x)
{
    if (state[x] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[x] = EATING;
        print_states();
        sem_post(&sem[x]);
    }
}
void print_table()
{
    printf("\n\t\t\t   1     2\n");
    printf("\n\t\t\t     ***\n");
    printf("\n\t\t\t    *****\n");
    printf("\n\t\t\t 5 ******* 3\n");
    printf("\n\t\t\t    *****\n");
    printf("\n\t\t\t      *\n");
    printf("\n\t\t\t      4\n\n");
}
void print_states()
{
    char *test = (char *)malloc(sizeof(test));
    for (int i = 0; i < N; i++)
    {
        switch (state[i])
        {
        case EATING:
            test = "EATING";
            printf("%-10s", test);
            break;
        case THINKING:
            test = "THINKING";
            printf("%-10s", test);
            break;
        case HUNGRY:
            test = "HUNGRY";
            printf("%-10s", test);
            break;
        default:
            break;
        }
    }
    printf("\n");
}
