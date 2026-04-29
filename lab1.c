#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

static void* proc1(void* arg) {
    int *flag = (int*) arg;
    while (*flag == 0) {
        printf("1");
        fflush(stdout);
        sleep(1);
    }
    pthread_exit((void*)2);
}

static void* proc2(void* arg) {
    int *flag = (int*) arg;
    while (*flag == 0) {
        printf("2");
        fflush(stdout);
        sleep(1);
    }
    pthread_exit((void*)3);
}

int main() {
    int create;
    pthread_attr_t attr;
    int* exitcode1;
    int* exitcode2;
    pthread_t thread1;
    pthread_t thread2;
    int flag1 = 0;
    int flag2 = 0;
    printf("\nProgram starts work!\n");
    pthread_attr_init(&attr);
    size_t stack_size = 0; // Uncorrect STACK!
    if (pthread_attr_setstacksize(&attr, stack_size) != 0) {
        printf("\nError setting stack size!\n");
        pthread_attr_destroy(&attr);
        return 1;
    }
    create = pthread_create(&thread1, &attr, proc1, &flag1);
    if (create != 0) {
        printf("\nError creating thread1!\n");
        pthread_attr_destroy(&attr);
        return 1;
    }
    pthread_attr_destroy(&attr);
    create = pthread_create(&thread2, NULL, proc2, &flag2);
    if (create != 0) {
        printf("\nError creating thread2!\n");
        flag1 = 1;
        pthread_join(thread1, (void**)&exitcode1);
        return 1;
    }
    printf("\nEnter the symbol:\n");
    getchar();
    printf("\nSymbol entered successfully\n");
    flag1 = 1;
    flag2 = 1;

    pthread_join(thread1, (void**)&exitcode1);
    pthread_join(thread2, (void**)&exitcode2);
    printf("\nexitcode for 1 = %p\n", exitcode1);
    printf("\nexitcode for 2 = %p\n", exitcode2);

    printf("\nProgram has completed. Bye bye.\n");
    return 0;
}
