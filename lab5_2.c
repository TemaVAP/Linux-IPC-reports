#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <poll.h>
#include <string.h>
#include <time.h>

#define SEMAPHORE_NAME "/semNAME"
#define FILENAME "FILE.txt"
#define SYMBOL '2'  

int main() {
    sem_t *sem;
    FILE *file;
    struct pollfd fds[1];
    int ch;

    sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    
    file = fopen(FILENAME, "a");
    if (!file) {
        perror("fopen");
        sem_close(sem);
        sem_unlink(SEMAPHORE_NAME);
        exit(EXIT_FAILURE);
    }
    
    fds[0].fd = 0; 
    fds[0].events = POLLIN;

    while (1) {
        sem_wait(sem);
        for (int i = 0; i < 10; i++) {
            fputc(SYMBOL, file);
            fflush(file);
            printf("%c", SYMBOL);
            fflush(stdout);
            sleep(1);
        }
        printf("\n");
        sem_post(sem);  
        sleep(1); 
        if (poll(fds, 1, 0) > 0) {
            char buf[10];
            read(0, buf, 10); 
            break;
        }
    }

    fclose(file);
    sem_close(sem);
    sem_unlink(SEMAPHORE_NAME);

    return 0;
}

