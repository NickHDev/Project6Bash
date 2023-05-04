#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char* argv[]) {
    pid_t pid;
    int i, count,j,sum;
    int data[argc-2];
    int status;
    int shared_mem_id;
    int* shared_mem_ptr;
    FILE* fp;
    fp = fopen(argv[1], "a");

    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    if (argc == 1 || (argc == 2 && strcmp(argv[1], "-h") == 0)) {
        printf("Usage: %s <filename> <number1> <number2> ... <numberN>\n", argv[0]);
        printf("Option: -h\tPrints this help message.\n");
        return 0;
    }
    shared_mem_id = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shared_mem_id < 0) {
        perror("shared memory error");
        exit(1);
    }
    shared_mem_ptr = (int*) shmat(shared_mem_id, NULL, 0);
    if (shared_mem_ptr == (int*) -1) {
        perror("shared memory attach error");
        exit(1);
    }
    shared_mem_ptr[0] = -2;
    shared_mem_ptr[1] = -2;


    if ((pid = fork()) == -1) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        //child process
        for (i = 2; i < argc; i++) {
            data[i-2] = atoi(argv[i]);
        }
        for (i = 0; i < argc-2; i++) {
            for (j = 0; j < argc-2; j++) {
                sum = data[i] + data[j];
                if (sum == 19)
                {
                    //found pair
                    shared_mem_ptr[0] = data[i];
                    shared_mem_ptr[1] = data[j];
                    exit(0);
                }
            }
        }
        shared_mem_ptr[0] = -1;
        shared_mem_ptr[1] = -1;
        free(data);
        shmdt(shared_mem_ptr);
        exit(0);
    }
    //parent process
    wait(&status);
    if (shared_mem_ptr[0] == -1 && shared_mem_ptr[1] == -1) {
        fprintf(fp, "No pair of numbers in the list add up to 19.\n");
    } else if (shared_mem_ptr[0] == -2 && shared_mem_ptr[1] == -2) {
        fprintf(fp, "Child process terminated unexpectedly.\n");
    } else {
        fprintf(fp, "Found pair: %d, %d\n", shared_mem_ptr[0], shared_mem_ptr[1]);
    } 
    shmdt(shared_mem_ptr);
    shmctl(shared_mem_id, IPC_RMID, NULL);


    fclose(fp);
    exit(0);
}
