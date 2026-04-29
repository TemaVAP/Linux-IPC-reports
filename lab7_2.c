#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define FILENAME "lab7"
#define PROJ_ID 'A'
#define MSG_TYPE 1

typedef struct {
    long mtype;
    char buff[256];
} TMessage;

volatile int stop = 0;
int msgid;

void *thread_func(void *arg) {
    TMessage message;
    while (!stop) {
        memset(&message, 0, sizeof(message));
        int result = msgrcv(msgid, &message, sizeof(message.buff), MSG_TYPE, IPC_NOWAIT);
        if (result == -1) {
        	if (errno != ENOMSG){
        		perror ("msgrcv error");
        		sleep (1);
        	}
        } else {
            printf("получено: %s\n", message.buff);
        }
    }
    return NULL;
}

int main() {
    key_t key = ftok(FILENAME, PROJ_ID);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);

    printf("нажмите клавишу, чтобы остановиться...\n");
    getchar();
    stop = 1;

    pthread_join(thread, NULL);

    msgctl(msgid, IPC_RMID, NULL); 
    return 0;
}

