#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t mutex;
sem_t emptySlots;
sem_t fullSlots;

int totalItems;   
int produced = 0;
int consumed = 0;


void* producer(void* arg) {
    int id = *((int*)arg);

    while (true) {
        if (produced >= totalItems) break;

        int item = rand() % 100;

        sem_wait(&emptySlots);
        sem_wait(&mutex);

        if (produced < totalItems) {
            buffer[in] = item;
            cout << "Producer " << id << " produced: " << item << " at " << in << endl;
            in = (in + 1) % BUFFER_SIZE;
            produced++;
        }

        sem_post(&mutex);
        sem_post(&fullSlots);

        sleep(1);
    }
    return NULL;
}


void* consumer(void* arg) {
    int id = *((int*)arg);

    while (true) {
        if (consumed >= totalItems) break;

        sem_wait(&fullSlots);
        sem_wait(&mutex);

        if (consumed < totalItems) {
            int item = buffer[out];
            cout << "Consumer " << id << " consumed: " << item << " from " << out << endl;
            out = (out + 1) % BUFFER_SIZE;
            consumed++;
        }

        sem_post(&mutex);
        sem_post(&emptySlots);

        sleep(2);
    }
    return NULL;
}

int main() {
    int p_count, c_count;

    cout << "Enter number of producers: ";
    cin >> p_count;

    cout << "Enter number of consumers: ";
    cin >> c_count;

    cout << "Enter total number of items to produce: ";
    cin >> totalItems;

    pthread_t p[p_count], c[c_count];
    int pid[p_count], cid[c_count];

    sem_init(&mutex, 0, 1);
    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&fullSlots, 0, 0);

    
    for (int i = 0; i < p_count; i++) {
        pid[i] = i + 1;
        pthread_create(&p[i], NULL, producer, &pid[i]);
    }

    
    for (int i = 0; i < c_count; i++) {
        cid[i] = i + 1;
        pthread_create(&c[i], NULL, consumer, &cid[i]);
    }

    
    for (int i = 0; i < p_count; i++) {
        pthread_join(p[i], NULL);
    }

    for (int i = 0; i < c_count; i++) {
        pthread_join(c[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);

    return 0;
}