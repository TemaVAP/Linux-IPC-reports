#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_WRITE_NAME "/sem_write"
#define SEM_READ_NAME "/sem_read"
#define SHM_SIZE sizeof(int)

int flag = 1;

sem_t *sem_write;
sem_t *sem_read;
int shm_fd;
int *shared_data;

void sigint_handler(int signum) {
    printf("\nвыход из отправителя\n");

    flag = 0; 
    
    sem_post(sem_read);  
    sem_post(sem_write); 

    sem_close(sem_read);
    sem_unlink(SEM_READ_NAME);

    sem_close(sem_write);
    sem_unlink(SEM_WRITE_NAME);

    munmap(shared_data, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    exit(0);
}

void *thread_func(void *arg) {
    while (flag) {
        int val = getdtablesize(); 

        printf("Sender: getdtablesize() = %d\n", val);

        memcpy(shared_data, &val, sizeof(int));

        sem_post(sem_write);
        sem_wait(sem_read);
        if (!flag) break;

        sleep(1);
    }
    return NULL;
}

int main() {
    signal(SIGINT, sigint_handler);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);

    shared_data = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_write = sem_open(SEM_WRITE_NAME, O_CREAT, 0666, 0);
    sem_read  = sem_open(SEM_READ_NAME,  O_CREAT, 0666, 0);

    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);

    getchar(); 

    flag = 0;
    sem_post(sem_read); 
    pthread_join(thread, NULL);

    sem_close(sem_write);
    sem_unlink(SEM_WRITE_NAME);

    sem_close(sem_read);
    sem_unlink(SEM_READ_NAME);

    munmap(shared_data, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return 0;
}
