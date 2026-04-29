# 🧵 Linux IPC Labs

Practical labs on multithreading, process management, and inter-process communication in Linux.  
Written in **C**, compiled with **GCC** on Linux (tested on Ubuntu / Astra Linux).

---

## 📋 Labs Overview

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

---

## 🔧 Build & Run

Each lab has a `.sh` script for quick build and run:

```bash
# Example for lab 2 (3 variants: no sync, sem_wait, sem_timedwait)
./lab2.sh       # no synchronization — race condition demo
./lab2_1.sh     # sem_wait / sem_post
./lab2_2.sh     # sem_timedwait with 15s timeout

# Example for lab 1
chmod +x lab1.sh
./lab1.sh

# Example for lab 3 (supports 3 pipe modes)
chmod +x lab3.sh
./lab3.sh -m 1   # blocking pipe()
./lab3.sh -m 2   # pipe2() with O_NONBLOCK
./lab3.sh -m 3   # pipe() + fcntl(O_NONBLOCK)

# Lab 8 — run server and client in separate terminals
./lab8.sh
./serv    # terminal 1
./client  # terminal 2
```

---

## 📌 Key Concepts Covered

- `pthread_create` / `pthread_join` / `pthread_exit`
- Mutexes, spinlocks, unnamed and named semaphores (`sem_open`, `sem_wait`, `sem_post`)
- Unnamed pipes (`pipe`, `pipe2`) and `fcntl` flags
- Process creation: `fork()`, `exec()`, `waitpid()`
- Shared memory: `shm_open`, `mmap`, `munmap`
- System V message queues: `msgget`, `msgsnd`, `msgrcv`
- TCP sockets: `socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`
- Signal handling: `SIGINT`, `signal()`

---

## ⚙️ Requirements

- Linux (Ubuntu 20.04+ or Astra Linux)
- GCC
- POSIX threads (`-lpthread`)

---

> 📚 Labs completed as part of the **Operating Systems** course at SPbETU "LETI"  
> ⚠️ For educational purposes only
