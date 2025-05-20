#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct {
    int pid;
    int arrival_time;
    int cpu_burst;
    int remaining_time;

    int has_io;
    int io_start_time;
    int io_burst;
    int io_remaining_time;
    int io_done;
    
    int priority;
    int completion_time;
    int waiting_time;
    int turnaround_time;
} Process;

#endif
