#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 3

char storage[MAX] = {};
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void show_storage(char* role, char* op, char prod) {
    printf("%s:", role);
    for (int i = 0; i < count; i++) {
        printf("%c", storage[i]);
    }
    printf("%s%c\n", op, prod);
}

void* pro_run(void* arg) {
    char* who = "生产者";
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count >= MAX) {
            printf("%s:满仓\n", who);
            pthread_cond_wait(&full, &mutex);
        }

        char prod = 'A' + rand() % 26;
        storage[count++] = prod;

        show_storage(who, "->", prod);

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

        usleep(rand() % 100 * 1000);
    }
}

void* con_run(void* arg) {
    char* who = "消费者";
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count <= 0) {
            printf("%s:空仓\n", who);
            pthread_cond_wait(&empty, &mutex);
        }

        char prod = storage[count--];

        show_storage(who, "<-", prod);

        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);

        usleep(rand() % 100 * 1000);
    }
}

int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, pro_run, NULL);
    pthread_create(&tid2, NULL, con_run, NULL);
    getchar();
}