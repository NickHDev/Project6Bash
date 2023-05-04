#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NUMS 100

void write_CHILD_data_to_file(int pid, int data[], int count, char* filename);
int main(int argc, char* argv[]) {
    pid_t pid;
    int i, count;
    int data[argc-2];
    int status;

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
   // printf(pid);

    if ((pid = fork()) == -1) {
	perror("fork");
        exit(1);
    } 
    else if (pid == 0) {
        count = argc - 2;
        for (i = 0; i < count; i++) {
		 data[i] = atoi(argv[i+2]);
        }
	int *shared_mem = mmap(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (shared_mem == MAP_FAILED) {
		perror("error with map");
		exit(1);
	}
	shared_mem[0] = -2;
	shared_mem9[1] = -2;
        write_CHILD_data_to_file(getpid(), data, count, argv[1]);
    } 
    
    if (wait(&status) < 0) {
        perror("wait");
        exit(1);
    }
    if (WIFEXITED(status)) {
        fprintf(fp, "Child process exited normally with status: %d\n", WEXITSTATUS(status));
        for (i = 0; i < count; i++) 
    {
	    for (j = i+1; j < count; j++)
	    {
	        if (data[i] + data[j] == 19)
	        {
	            fprintf(fp, "Child: %d: Pair: %d %d\n", pid, data[i], data[j]);	
	        } 
	    }
    }
    fclose(fp);
    exit(0);
}
