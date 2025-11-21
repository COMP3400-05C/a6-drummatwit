#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * TODO: Describe what the function does
 * Creates and initializes an array of PCB structures on the heap
 * Each PCB gets:
 *  - pid = its index
 *  - burst_left = bursts[i] 
 *  - wait = 0
 * 
 * bursts: array of CPU burst times
 * blen: length of burts
 * 
 * returns: pointer o allocated PCB array
 */
struct pcb* init_procs(int* bursts, int blen) {
    if (blen <= 0) {
        return NULL;
    }
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (!procs) {
        return NULL; 
    }

    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    } 
    return procs;   
}

/**
 * Prints the burst_left and wait time of all PCBs.
 */
void printall(struct pcb* procs, int plen) {
    for (int i = 0; i < plen; i++) {
        printf("P%d: burst_left=%d wait =%d\n", 
            procs[i].pid, procs[i].burst_left, procs[i].wait); 
    }
}

/**
 * Runs a process for amount time+
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (!procs || plen <= 0 || current < 0 || current >= plen) {
        return; 
    }
    if (amount <= 0 || procs[current].burst_left <= 0) {
        return; 
    }
    int actual = amount; 
    if (actual > procs[current].burst_left) {
        actual = procs[current].burst_left; 
    }

    //Increase wait for other processes 
    for (int i = 0; i < plen; i++) {
        if (i != current && procs[i].burst_left > 0) {
            procs[i].wait += actual; 
        }
    }
    // Reduce current burst left
    procs[current].burst_left -= actual;
}

/**
 * Runs FCFS scheduling
 * Returns total time elasped
 */
int fcfs_run(struct pcb* procs, int plen) {
    if (!procs || plen <= 0) {
        return 0;
    }
    int time = 0;

    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            int amount = procs[i].burst_left; 
            run_proc(procs, plen, i, amount);
            time += amount; 
        }
    }
    return time;
}

/**
 * Finds next process in round-robin order
 * returns index of next active process
 *  - returns -1 if all done
 */
int rr_next(int current, struct pcb* procs, int plen) {
    if (!procs || plen <= 0) {
        return -1; 
    }
    for (int i = 1; i <= plen; i++) {
        int index = (current + i) % plen;
        if (procs[index].burst_left > 0) {
            return index;
        }
    }
    return -1;
}
/**
 * Runs round-robin scheduling 
 * return total time elapsed 
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    if (!procs || plen <= 0 || quantum <= 0) {
        return 0;
    }
    int time = 0;
    int current = 0;

    //Find first active process
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            current = i;
            break; 
        }
    }
    while (1) {
        // If the current process still has burst time, run it
        if (procs[current].burst_left > 0) {
            int amount = (procs[current].burst_left > quantum)
                            ? quantum
                            : procs[current].burst_left; 
            run_proc(procs, plen, current, amount );
            time += amount; 
        }
        // Find next active process
        int next = rr_next(current, procs, plen);

        if (next == -1) {
            break;
        }
        current = next;
    }
    return time;
}

