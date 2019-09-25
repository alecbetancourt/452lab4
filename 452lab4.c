//add sample printout
//add code to compute running average
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#define MAX 256

pthread_mutex_t lock;
int threadCount = 0;
//float runningAvg = 0;

void* thread(void* arg);
void sigHandler(int signal);


int main() {
    pthread_t thread1;
    char buf[MAX];
    int status;

    signal(SIGINT, sigHandler);
    
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        puts("mutex init has failed\n");
        return 1;
    }

    while(1) {
        printf("\nInput name of file to access: ");
        fgets(buf, MAX, stdin);
	strtok(buf, "\n");

        if ((status = pthread_create (&thread1, NULL,  thread, &buf)) != 0) {
            printf ("thread create error %d: %s\n", status, strerror(status));
            exit (1);
        }
    }
}

void* thread(void* arg) {
    //int sleep;
    char *str_ptr = (char *)malloc(MAX*sizeof(char));
    strcpy(str_ptr, (char *) arg);
    
    //rand 0-9, divided into 0-7 and 8-9
    if ((int)(rand() % 10) < 8) {
	//sleepTime = 1;
        sleep(1);
    }
    else {
        //sleep for 7-10 seconds randomly
	//sleepTime = (int)(rand() % 4) + 7;
	//sleep(sleepTime);
        sleep((int)(rand() % 4) + 7);
    }
    fprintf(stderr, "\n%s found, thread terminating...\n", str_ptr);

    //add to running average using sleeptime
    
    pthread_mutex_lock(&lock);
    threadCount++;
    pthread_mutex_unlock(&lock);
    
    pthread_exit(0);
}

void sigHandler(int signal){
	if (signal == SIGINT){
		
		printf("\nTotal number of files received: %d\n", threadCount);
		//print running average file time
		printf("Shutting down...\n");
		pthread_mutex_destroy(&lock);
		exit(0);
	} else {
		fprintf(stderr, "Signal error\n");
	}

}
