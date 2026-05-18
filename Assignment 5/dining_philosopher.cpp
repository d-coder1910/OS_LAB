#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int N, LIMIT;

int *state;
int *eatCount;
pthread_t *thread_id;
int *phil;

pthread_mutex_t mutex1, printMutex;
pthread_cond_t *cond;

#define LEFT(i)  ((i + N - 1) % N)
#define RIGHT(i) ((i + 1) % N)

void test(int i)
{
    if(state[i] == HUNGRY &&
       state[LEFT(i)] != EATING &&
       state[RIGHT(i)] != EATING)
    {
        state[i] = EATING;
        pthread_cond_signal(&cond[i]);
    }
}

void take_forks(int i)
{
    pthread_mutex_lock(&mutex1);

    state[i] = HUNGRY;

    pthread_mutex_lock(&printMutex);
    cout << "Philosopher " << i + 1 << " is HUNGRY" << endl;
    pthread_mutex_unlock(&printMutex);

    test(i);

    while(state[i] != EATING)
        pthread_cond_wait(&cond[i], &mutex1);

    pthread_mutex_unlock(&mutex1);
}

void put_forks(int i)
{
    pthread_mutex_lock(&mutex1);

    state[i] = THINKING;

    pthread_mutex_lock(&printMutex);
    cout << "Philosopher " << i + 1 << " is THINKING" << endl;
    pthread_mutex_unlock(&printMutex);

    test(LEFT(i));
    test(RIGHT(i));

    pthread_mutex_unlock(&mutex1);
}

void* philosopher(void* num)
{
    int i = *(int*)num;

    while(eatCount[i] < LIMIT)
    {
        sleep(1);

        take_forks(i);

        pthread_mutex_lock(&printMutex);
        cout << "Philosopher " << i + 1 << " is EATING ("
             << eatCount[i] + 1 << "/" << LIMIT << ")" << endl;
        pthread_mutex_unlock(&printMutex);

        sleep(2);

        eatCount[i]++;

        put_forks(i);
    }

    return NULL;
}

int main()
{
    cout << "Enter number of philosophers: ";
    cin >> N;

    cout << "Enter number of times each philosopher eats: ";
    cin >> LIMIT;

    state = new int[N];
    eatCount = new int[N];
    phil = new int[N];
    thread_id = new pthread_t[N];
    cond = new pthread_cond_t[N];

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&printMutex, NULL);

    for(int i = 0; i < N; i++)
    {
        state[i] = THINKING;
        eatCount[i] = 0;
        pthread_cond_init(&cond[i], NULL);
    }

    for(int i = 0; i < N; i++)
    {
        phil[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
    }

    for(int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&printMutex);

    for(int i = 0; i < N; i++)
        pthread_cond_destroy(&cond[i]);

    delete[] state;
    delete[] eatCount;
    delete[] phil;
    delete[] thread_id;
    delete[] cond;

    return 0;
}