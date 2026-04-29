#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int flag1 = 0;
int flag2 = 0;
int nonblock_mode = 0;

void sigint_handler(int sig) {
    flag1 = 1;
    flag2 = 1;
    printf("\nSIGINT Получен\n");
    fflush(stdout);
}

void* proc1(void* arg) {
    int* pipe = (int*) arg;
    char buffer[BUFFER_SIZE];

    while (!flag1) {
    	int max_fd=getdtablesize();
        snprintf(buffer, BUFFER_SIZE, "Всем Привет от Артёма! Max FD: %d\n", max_fd);

        ssize_t bytes_written = write(pipe[1], buffer, strlen(buffer) + 1);
        if (bytes_written == -1) {
            perror("ERROR WRITE: ");
        } else {
            printf("\nЗаписано: %s\n", buffer);
            fflush(stdout);
        }
        sleep(1);
    }
    pthread_exit((void*)2);
}

void* proc2(void* arg) {
    int* pipe = (int*) arg;
    char buffer[BUFFER_SIZE];

    while (!flag2) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(pipe[0], buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("ERROR READ: ");
        } else if (bytes_read > 0) {
            printf("Прочитано: %s", buffer);
        }
        sleep(1);
    }
    pthread_exit((void*)3);
}

int main(int argc, char* argv[]) {
    int* exitcode1;
    int* exitcode2;
    pthread_t thread1, thread2;
    int filedes[2];
    int mode = 0;
    int opt;

    // Обработка аргументов командной строки через getopt()
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':
                mode = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Используй: %s -m [1|2|3]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (mode != 1 && mode != 2 && mode != 3) {
        fprintf(stderr, "Не указан правильный режим: -m [1|2|3]\n");
        return 1;
    }

    signal(SIGINT, sigint_handler);

    // Инициализация канала
    if (mode == 1) {
        printf("Режим 1: pipe() (блокирующий)\n");
        if (pipe(filedes) == -1) {
            perror("ERROR PIPE");
            return 1;
        }
    } else if (mode == 2) {
        printf("Режим 2: pipe2() с O_NONBLOCK\n");
        if (pipe2(filedes, O_NONBLOCK) == -1) {
            perror("ERROR PIPE2");
            return 1;
        }
        nonblock_mode = 1;
    } else if (mode == 3) {
        printf("Режим 3: pipe() + fcntl(O_NONBLOCK)\n");
        if (pipe(filedes) == -1) {
            perror("ERROR PIPE");
            return 1;
        }

        int flags_r = fcntl(filedes[0], F_GETFL);
        int flags_w = fcntl(filedes[1], F_GETFL);
        fcntl(filedes[0], F_SETFL, flags_r | O_NONBLOCK);
        fcntl(filedes[1], F_SETFL, flags_w | O_NONBLOCK);
        nonblock_mode = 1;
    } else {
        printf("Неверный выбор. Используй 1, 2 or 3!\n");
        return 1;
    }
    pthread_create(&thread1, NULL, proc1, filedes);
    pthread_create(&thread2, NULL, proc2, filedes);

    printf("\nВведите символ чтобы остановить поток!\n");
    getchar();
    flag1 = 1;
    flag2 = 1;

    close(filedes[0]);
    close(filedes[1]);
    
    pthread_join(thread1, (void**)&exitcode1);
    pthread_join(thread2, (void**)&exitcode2);
   
    printf("\nexitcode for 1 = %p\n", exitcode1);
    printf("\nexitcode for 2 = %p\n", exitcode2);
    printf("Program has completed\n");
	
    return 0;
}

