#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

using namespace std;

sem_t wrt;
pthread_mutex_t mutex;
int readcount = 0;

void *reader(void* arg){
    int id = *((int*)arg);

    pthread_mutex_lock(&mutex);
    readcount++;

    if(readcount == 1){
        sem_wait(&wrt);
    }

    pthread_mutex_unlock(&mutex);

    cout << "Reader " << id << " is reading" << endl;
    sleep(1);

    pthread_mutex_lock(&mutex);
    readcount--;

    if(readcount == 0){
        sem_post(&wrt);
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* writer(void* arg){
    int id = *((int*)arg);

    sem_wait(&wrt);

    cout << "Writer " << id << " is writing" << endl;
    sleep(1);

    sem_post(&wrt);

    return NULL;
}

int main(){
    int r_count, w_count;

    cout << "Enter number of readers: ";
    cin >> r_count;

    cout << "Enter number of writers: ";
    cin >> w_count;

    pthread_t r[r_count], w[w_count];
    int rid[r_count], wid[w_count];

    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    
    for(int i = 0; i < r_count; i++){
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }

    
    for(int i = 0; i < w_count; i++){
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }

  
    for(int i = 0; i < r_count; i++){
        pthread_join(r[i], NULL);
    }

    
    for(int i = 0; i < w_count; i++){
        pthread_join(w[i], NULL);
    }

    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}