Linux IPC Labs — Лабораторные работы по IPC в Linux
Практические лабораторные работы по многопоточности, управлению процессами и межпроцессному взаимодействию в Linux.
Написаны на языке C, компилируются с помощью GCC на Linux (проверено на Ubuntu / Astra Linux).

RU
Лабораторные работы
№ТемаФайлы1Создание и завершение потоковlab1.c2Синхронизация потоков с помощью семафоров (sem_wait, sem_timedwait)lab2.c, lab2_1.c, lab2_2.c3Взаимодействие потоков через неименованные каналы (pipe, pipe2, fcntl)lab3.c4Создание процессов с помощью fork() и exec()lab4_1.c, lab4_2.c5Синхронизация процессов через именованные семафорыlab5_1.c, lab5_2.c6Межпроцессное взаимодействие через разделяемую память + семафорыlab6_1.c, lab6_2.c7IPC через очереди сообщений System V (msgsnd / msgrcv)lab7_1.c, lab7_2.c8Сетевой IPC через TCP-сокеты (сервер + клиент)serv.c, client.c
Сборка и запуск
Для каждой лабы есть .sh-скрипт для быстрой сборки и запуска:
bash# Лаб. 2 — три варианта: без синхронизации, sem_wait, sem_timedwait
./lab2.sh        # без синхронизации — демонстрация гонки данных
./lab2_1.sh      # sem_wait / sem_post
./lab2_2.sh      # sem_timedwait с таймаутом 15 секунд

# Лаб. 1
chmod +x lab1.sh
./lab1.sh

# Лаб. 3 — три режима работы каналов
chmod +x lab3.sh
./lab3.sh -m 1   # блокирующий pipe()
./lab3.sh -m 2   # pipe2() с флагом O_NONBLOCK
./lab3.sh -m 3   # pipe() + fcntl(O_NONBLOCK)

# Лаб. 8 — сервер и клиент запускаются в отдельных терминалах
./lab8.sh
./serv           # терминал 1
./client         # терминал 2
Рассматриваемые концепции

pthread_create / pthread_join / pthread_exit
Мьютексы, спин-локи, именованные и неименованные семафоры (sem_open, sem_wait, sem_post)
Неименованные каналы (pipe, pipe2) и флаги fcntl
Создание процессов: fork(), exec(), waitpid()
Разделяемая память: shm_open, mmap, munmap
Очереди сообщений System V: msgget, msgsnd, msgrcv
TCP-сокеты: socket, bind, listen, accept, connect, send, recv
Обработка сигналов: SIGINT, signal()

Требования

Linux (Ubuntu 20.04+ или Astra Linux)
GCC
POSIX threads (-lpthread)


EN
Labs Overview
#TopicFiles1Thread creation and destructionlab1.c2Thread synchronization with semaphores (sem_wait, sem_timedwait)lab2.c, lab2_1.c, lab2_2.c3Thread interaction via unnamed pipes (pipe, pipe2, fcntl)lab3.c4Process creation with fork() and exec()lab4_1.c, lab4_2.c5Process synchronization with named semaphoreslab5_1.c, lab5_2.c6Inter-process communication via shared memory + semaphoreslab6_1.c, lab6_2.c7IPC via System V message queues (msgsnd / msgrcv)lab7_1.c, lab7_2.c8Network IPC via TCP sockets (server + client)serv.c, client.c
Build & Run
Each lab has a .sh script for quick build and run:
bash# Lab 2 — 3 variants: no sync, sem_wait, sem_timedwait
./lab2.sh        # no synchronization — race condition demo
./lab2_1.sh      # sem_wait / sem_post
./lab2_2.sh      # sem_timedwait with 15s timeout

# Lab 1
chmod +x lab1.sh
./lab1.sh

# Lab 3 — supports 3 pipe modes
chmod +x lab3.sh
./lab3.sh -m 1   # blocking pipe()
./lab3.sh -m 2   # pipe2() with O_NONBLOCK
./lab3.sh -m 3   # pipe() + fcntl(O_NONBLOCK)

# Lab 8 — run server and client in separate terminals
./lab8.sh
./serv           # terminal 1
./client         # terminal 2
Key Concepts Covered

pthread_create / pthread_join / pthread_exit
Mutexes, spinlocks, unnamed and named semaphores (sem_open, sem_wait, sem_post)
Unnamed pipes (pipe, pipe2) and fcntl flags
Process creation: fork(), exec(), waitpid()
Shared memory: shm_open, mmap, munmap
System V message queues: msgget, msgsnd, msgrcv
TCP sockets: socket, bind, listen, accept, connect, send, recv
Signal handling: SIGINT, signal()

Requirements

Linux (Ubuntu 20.04+ or Astra Linux)
GCC
POSIX threads (-lpthread)
