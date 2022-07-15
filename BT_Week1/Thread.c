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

FILE *file_print;
FILE *file_freq;

void *Sampling(void * SAMPLE){

    clock_gettime(CLOCK_REALTIME, &request);

    
    while (1){ 
	// Lap lai voi chu ky X (nanoseconds)
	request.tv_nsec +=X;
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);

        clock_gettime(CLOCK_REALTIME, &ts);
        // printf("Current Time: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);

        // Luu thoi gian vao bien T
        char buff[50];
        strftime(buff, sizeof buff, "%s", gmtime(&ts.tv_sec));
        snprintf(T, sizeof T, "%s.%09ld\n", buff, ts.tv_nsec);
    }   
}

void *Logging_Print(void *LOGGING){
    
    while(1){

    	if (ts.tv_sec>time_sec_prev)
    	    interval = (int)(1e9) + ts.tv_nsec - time_nsec_prev;
    	else
            interval = ts.tv_nsec - time_nsec_prev;

	time_sec_prev = ts.tv_sec;
	time_nsec_prev = ts.tv_nsec;

    	// In gia tri thoi gian trong bien T vaof file tiime_and_interval.txt
    	file_print = fopen("freq_1000000ns.txt","a");
    	if (file_print && interval != 0){
            fprintf(file_print,"%ld\n",interval);
    	}
    	
    	fclose(file_print);
    }

}

void *Reading_Input(void *INPUT){

    while(1){
    	// Doc gia tri chu ky lay mau tu file freq.txt 
    	file_freq = fopen("freq.txt","r");
    	fscanf (file_freq, "%d", &X);
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

