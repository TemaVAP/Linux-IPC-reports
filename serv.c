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
#include <sys/queue.h>

#define PORT 7000
#define BACKLOG 1
#define BUF_SIZE 256

volatile int running = 1;
int client_sock = -1;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

struct request_entry {
    int number;
    STAILQ_ENTRY(request_entry) entries;
};

STAILQ_HEAD(request_queue, request_entry);
struct request_queue requestQueue;

void handle_signal(int signum) {
    running = 0;
}

void *recv_thread(void *arg) {
    char buf[BUF_SIZE];
    while (running) {
        int count = recv(client_sock, buf, sizeof(buf), 0);
        if (count > 0) {
            int num = atoi(buf);
            struct request_entry *req = malloc(sizeof(struct request_entry));
            req->number = num;

            pthread_mutex_lock(&queue_mutex);
            STAILQ_INSERT_TAIL(&requestQueue, req, entries);
            pthread_mutex_unlock(&queue_mutex);

            printf("Received request #%d\n", num);
        } else if (count == 0) {
            printf("Client disconnected.\n");
            running = 0;
        } else {
            perror("recv");
            sleep(1);
        }
    }
    return NULL;
}

void *send_thread(void *arg) {
    while (running) {
        pthread_mutex_lock(&queue_mutex);
        if (!STAILQ_EMPTY(&requestQueue)) {
            struct request_entry *req = STAILQ_FIRST(&requestQueue);
            STAILQ_REMOVE_HEAD(&requestQueue, entries);
            pthread_mutex_unlock(&queue_mutex);

            char response[BUF_SIZE];
            snprintf(response, sizeof(response), "Ответ на запрос #%d", req->number);
            int sent = send(client_sock, response, strlen(response) + 1, 0);
            if (sent == -1) {
                perror("send");
            } else {
                printf("Sent response: %s\n", response);
            }
            free(req);
        } else {
            pthread_mutex_unlock(&queue_mutex);
            sleep(1);
        }
    }
    return NULL;
}

void *accept_thread(void *arg) {
    int server_sock = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);

    while (running) {
        int s = accept(server_sock, (struct sockaddr *)&client_addr, &addrlen);
        if (s == -1) {
            perror("accept");
            sleep(1);
        } else {
            client_sock = s;
            printf("Client connected: ip = %s, port = %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            fcntl(client_sock, F_SETFL, O_NONBLOCK);

            pthread_t tid_recv, tid_send;
            pthread_create(&tid_recv, NULL, recv_thread, NULL);
            pthread_create(&tid_send, NULL, send_thread, NULL);
            pthread_join(tid_recv, NULL);
            pthread_join(tid_send, NULL);
            close(client_sock);
            client_sock = -1;
        }
    }
    return NULL;
}

int main() {
    signal(SIGINT, handle_signal); // Используем обычную функцию для обработки сигнала

    STAILQ_INIT(&requestQueue);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    fcntl(server_sock, F_SETFL, O_NONBLOCK);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server_sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_sock, BACKLOG);

    pthread_t tid_accept;
    pthread_create(&tid_accept, NULL, accept_thread, &server_sock);

    printf("Press Enter to stop server...\n");
    getchar();
    running = 0;

    if (client_sock != -1) {
        shutdown(client_sock, SHUT_RDWR);
        close(client_sock);
    }
    close(server_sock);
    pthread_join(tid_accept, NULL);

    return 0;
}

