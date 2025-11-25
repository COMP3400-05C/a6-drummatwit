#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    char * algorithm = argv[1];
    if (strcmp(algorithm, "fcfs") == 0) {
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        int plen = argc - 2; //number of bursts
        int *bursts = malloc(sizeof(int) * plen); 

        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[i+2]); 
        }
        printf("Using FCFS\n\n");
        // print accepted process
        for (int i = 0; i < plen; i++) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        struct pcb* procs = init_procs(bursts, plen); 
        int total_time = fcfs_run(procs, plen);
        //compute average wait time
        double total_wait = 0;
        for (int i = 0; i < plen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = total_wait / plen; 
        printf("Average wait time: %.2f\n", avg_wait);
        free(bursts);
        free(procs);
        return 0;
    } else if (strcmp(algorithm, "rr") == 0) {
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        int quantum = atoi(argv[2]);
        int plen = argc - 3;
        int* bursts = malloc(sizeof(int) *plen);
        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[i+3]);   
        }
        printf("Using RR(%d)\n\n", quantum);
        for (int i = 0; i < plen; i++) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        struct pcb* procs = init_procs(bursts, plen); 
        int total_time = rr_run(procs, plen, quantum);
        double total_wait = 0;
        for (int i = 0; i < plen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = total_wait / plen; 
        printf("Average wait time: %.2f\n", avg_wait);
        free(bursts);
        free(procs);
        return 0;
    } else {
        printf("ERROR: Unknown algorithm\n");
        return 1;
    }
}
