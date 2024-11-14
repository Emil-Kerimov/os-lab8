#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int data = -1; // Глобальна змінна для збереження числа
pthread_mutex_t mutex;
pthread_cond_t data_ready;

void* producer(void* arg) {
    while (1) {
        sleep(2);

        int item = rand() % 100;

        pthread_mutex_lock(&mutex);
        data = item;
        printf("Виробник згенерував: %d\n", item);

        pthread_cond_signal(&data_ready);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (data == -1) {
            pthread_cond_wait(&data_ready, &mutex);
        }

        printf("Споживач отримав: %d\n", data);
        data = -1;

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&data_ready, NULL); // Ініціалізуємо умовну змінну

    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    sleep(10);

    pthread_cancel(producer_thread);
    pthread_cancel(consumer_thread);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&data_ready);

    printf("Програма завершена\n");
    return 0;
}
