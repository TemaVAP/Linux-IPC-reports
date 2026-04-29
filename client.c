#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7000
#define BUF_SIZE 256

int sock = -1;
volatile int running = 1;
volatile int exit_connect = 0;  // Флаг для выхода из цикла подключения

void handle_signal(int signum) {
    running = 0;
}

void *wait_for_input(void *arg) {
    printf("Press Enter to stop client...\n");
    getchar();  // Ожидание нажатия Enter
    exit_connect = 1;  // Устанавливаем флаг выхода
    running = 0;  // Останавливаем выполнение программы
    return NULL;
}

void *send_thread(void *arg) {
    int counter = 1;
    char buf[BUF_SIZE];
    while (running) {
        snprintf(buf, sizeof(buf), "%d", counter++);
        int sent = send(sock, buf, strlen(buf) + 1, 0);
        if (sent == -1) {
            perror("send");
        } else {
            printf("Sent request #%s\n", buf);
        }
        sleep(1);
    }
    return NULL;
}

void *recv_thread(void *arg) {
    char buf[BUF_SIZE];
    while (running) {
        int count = recv(sock, buf, sizeof(buf), 0);
        if (count > 0) {
            printf("Received response: %s\n", buf);
        } else if (count == 0) {
            printf("Server disconnected.\n");
            running = 0;
        } else {
            perror("recv");
            sleep(1);
        }
    }
    return NULL;
}

int main() {
    signal(SIGINT, handle_signal);  // Используем обычную функцию для обработки сигнала

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Поток для ожидания нажатия Enter
    pthread_t input_thread;
    pthread_create(&input_thread, NULL, wait_for_input, NULL);

    while (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1 && running && !exit_connect) {
        perror("connect");
        sleep(1);
    }

    if (!running || exit_connect) {
        close(sock);
        pthread_join(input_thread, NULL);  // Ждем завершения потока ввода
        return 0;
    }

    pthread_t tid_send, tid_recv;
    pthread_create(&tid_send, NULL, send_thread, NULL);
    pthread_create(&tid_recv, NULL, recv_thread, NULL);

    pthread_join(input_thread, NULL);  // Ожидаем завершения потока ожидания ввода
    running = 0;

    shutdown(sock, SHUT_RDWR);
    close(sock);
    pthread_join(tid_send, NULL);
    pthread_join(tid_recv, NULL);

    return 0;
}

