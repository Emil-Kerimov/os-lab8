#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int buffer_index = 0;

sem_t empty_slots;
sem_t full_slots;
pthread_mutex_t buffer_mutex;

void* producer(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        int item = rand() % 100;

        sem_wait(&empty_slots);
        pthread_mutex_lock(&buffer_mutex);

        buffer[buffer_index++] = item;
        printf("Виробник згенерував: %d\n", item);

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_slots);

        sleep(1);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        sem_wait(&full_slots);
        pthread_mutex_lock(&buffer_mutex);

        int item = buffer[--buffer_index];
        printf("Споживач отримав: %d\n", item);

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty_slots);

        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);

    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&buffer_mutex);

    printf("Програма завершена\n");
    return 0;
}
