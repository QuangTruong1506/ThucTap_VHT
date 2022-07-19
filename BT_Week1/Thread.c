#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <math.h>

static int X;               // Chu ky lay mau X
static char  T[50];          // Bien luu thoi gian
static long int interval;   // Bien luu khoang thoi gian giua hai moc

static struct timespec ts, request;
static long int time_sec_prev;
static long int time_nsec_prev;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

FILE *file_print;
FILE *file_freq;

void *Sampling(void * SAMPLE){
    
    clock_gettime(CLOCK_REALTIME, &request);

    while (1){ 
	pthread_mutex_lock(&mutex);

        clock_gettime(CLOCK_REALTIME, &ts);
        // printf("Current Time: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);

	// Lap lai voi chu ky X (nanoseconds)
	request.tv_nsec +=X;
	if (request.tv_nsec > 1000000000){
	    request.tv_nsec -= 1000000000;
	    request.tv_sec ++;
	}

	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);
	    
        // Luu thoi gian vao bien T
        char buff[50];
        strftime(buff, sizeof buff, "%s", gmtime(&ts.tv_sec));
        snprintf(T, sizeof T, "%s.%09ld\n", buff, ts.tv_nsec);
	pthread_mutex_unlock(&mutex);
    }   
}

void *Logging_Print(void *LOGGING){
    
    file_print = fopen("freq_1000000ns_test.txt","a");

    while(1){

    	interval = (1e9*ts.tv_sec+ts.tv_nsec) - (1e9*time_sec_prev+time_nsec_prev);

	time_sec_prev = ts.tv_sec;
	time_nsec_prev = ts.tv_nsec;

    	// In gia tri thoi gian trong bien T vaof file tiime_and_interval.txt
    	
    	if (file_print && interval != 0)
            fprintf(file_print,"%ld\n",interval);
    	
    }
    
    fclose(file_print);
}

void *Reading_Input(void *INPUT){

    while(1){

    	// Doc gia tri chu ky lay mau tu file freq.txt 
    	file_freq = fopen("freq.txt","r");
    	fscanf (file_freq, "%d", &X);
	//printf("Frequency = %d\n",X);
    	fclose(file_freq);
	
    }
}

int main(int argc, char** argv) { 
    pthread_t SAMPLE, LOGGING, INPUT;

    pthread_create( &INPUT, NULL, Reading_Input, (void*) &INPUT);
    pthread_create( &SAMPLE, NULL, Sampling, (void *)&SAMPLE);  
    pthread_create( &LOGGING, NULL, Logging_Print, (void*) &LOGGING);


    pthread_join( INPUT, NULL);
    pthread_join( SAMPLE, NULL);
    pthread_join( LOGGING, NULL);  

    exit(0);
}

