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

static struct timespec ts;

FILE *file_print;
FILE *file_freq;

void *Sampling(void * SAMPLE){
    // Lap lai voi chu ky X (nanoseconds)
    if (nanosleep((const struct timespec[]){{0, X}}, NULL))
        return NULL;  
    else{  
        // Doc thoi gian thuc cuar he thong
        long int time_sec_prev = ts.tv_sec;
        long int time_nsec_prev = ts.tv_nsec;

        timespec_get(&ts, TIME_UTC);
        // printf("Current Time: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);
        
        // Tinh quang thoi gian giua T hien tai vaf T truoc do
        if (ts.tv_sec>time_sec_prev)
            interval = (int)(1e9) + ts.tv_nsec - time_nsec_prev;
        else
            interval = ts.tv_nsec - time_nsec_prev;

        // Luu thoi gian vao bien T
        char buff[50];
        strftime(buff, sizeof buff, "%s", gmtime(&ts.tv_sec));
        snprintf(T, sizeof T, "%s.%09ld\n", buff, ts.tv_nsec);
    }   
}

void *Logging_Print(void *LOGGING){

    // In gia tri thoi gian trong bien T vaof file tiime_and_interval.txt
    file_print = fopen("time_and_interval.txt","a");
    if (file_print){
        fprintf(file_print,"%s\n%ld\n",T,interval);
    }
    else{
        printf("Failed to open the file \n");
    }
    fclose(file_print);

}

void *Reading_Input(void *INPUT){

    // Doc gia tri chu ky lay mau tu file freq.txt 
    file_freq = fopen("freq.txt","r");
    fscanf (file_freq, "%d", &X);

    fclose(file_freq);
}

int main(int argc, char** argv) { 
    pthread_t SAMPLE, LOGGING, INPUT;

    while (1){
    pthread_create( &INPUT, NULL, Reading_Input, (void*) &INPUT);
    pthread_create( &SAMPLE, NULL, Sampling, (void *)&SAMPLE);  
    pthread_create( &LOGGING, NULL, Logging_Print, (void*) &LOGGING);

    pthread_join( INPUT, NULL);
    pthread_join( SAMPLE, NULL);
    pthread_join( LOGGING, NULL);
    }  

    return 0;
}

