# Linux IPC Labs — Лабораторные работы по IPC в Linux

Практические лабораторные работы по многопоточности, управлению процессами и межпроцессному взаимодействию в Linux.
Написаны на языке **C**, компилируются с помощью **GCC** на Linux (проверено на Ubuntu / Astra Linux).

---

## RU

### Лабораторные работы

| № | Тема | Файлы |
|---|------|-------|
| 1 | Создание и завершение потоков | `lab1.c` |
| 2 | Синхронизация потоков с помощью семафоров (`sem_wait`, `sem_timedwait`) | `lab2.c`, `lab2_1.c`, `lab2_2.c` |
| 3 | Взаимодействие потоков через неименованные каналы (`pipe`, `pipe2`, `fcntl`) | `lab3.c` |
| 4 | Создание процессов с помощью `fork()` и `exec()` | `lab4_1.c`, `lab4_2.c` |
| 5 | Синхронизация процессов через именованные семафоры | `lab5_1.c`, `lab5_2.c` |
| 6 | Межпроцессное взаимодействие через разделяемую память + семафоры | `lab6_1.c`, `lab6_2.c` |
| 7 | IPC через очереди сообщений System V (`msgsnd` / `msgrcv`) | `lab7_1.c`, `lab7_2.c` |
| 8 | Сетевой IPC через TCP-сокеты (сервер + клиент) | `serv.c`, `client.c` |

### Рассматриваемые концепции

- `pthread_create` / `pthread_join` / `pthread_exit`
- Мьютексы, спин-локи, именованные и неименованные семафоры (`sem_open`, `sem_wait`, `sem_post`)
- Неименованные каналы (`pipe`, `pipe2`) и флаги `fcntl`
- Создание процессов: `fork()`, `exec()`, `waitpid()`
- Разделяемая память: `shm_open`, `mmap`, `munmap`
- Очереди сообщений System V: `msgget`, `msgsnd`, `msgrcv`
- TCP-сокеты: `socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`
- Обработка сигналов: `SIGINT`, `signal()`

### Требования

- Linux (Ubuntu 20.04+ или Astra Linux)
- GCC
- POSIX threads (`-lpthread`)

---

## EN

### Labs Overview

| # | Topic | Files |
|---|-------|-------|
| 1 | Thread creation and destruction | `lab1.c` |
| 2 | Thread synchronization with semaphores (`sem_wait`, `sem_timedwait`) | `lab2.c`, `lab2_1.c`, `lab2_2.c` |
| 3 | Thread interaction via unnamed pipes (`pipe`, `pipe2`, `fcntl`) | `lab3.c` |
| 4 | Process creation with `fork()` and `exec()` | `lab4_1.c`, `lab4_2.c` |
| 5 | Process synchronization with named semaphores | `lab5_1.c`, `lab5_2.c` |
| 6 | Inter-process communication via shared memory + semaphores | `lab6_1.c`, `lab6_2.c` |
| 7 | IPC via System V message queues (`msgsnd` / `msgrcv`) | `lab7_1.c`, `lab7_2.c` |
| 8 | Network IPC via TCP sockets (server + client) | `serv.c`, `client.c` |


### Key Concepts Covered

- `pthread_create` / `pthread_join` / `pthread_exit`
- Mutexes, spinlocks, unnamed and named semaphores (`sem_open`, `sem_wait`, `sem_post`)
- Unnamed pipes (`pipe`, `pipe2`) and `fcntl` flags
- Process creation: `fork()`, `exec()`, `waitpid()`
- Shared memory: `shm_open`, `mmap`, `munmap`
- System V message queues: `msgget`, `msgsnd`, `msgrcv`
- TCP sockets: `socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`
- Signal handling: `SIGINT`, `signal()`

### Requirements

- Linux (Ubuntu 20.04+ or Astra Linux)
- GCC
- POSIX threads (`-lpthread`)
