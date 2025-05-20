#include <stdio.h>
#include "process.h"

// Manually creates an array of processes with predefined attributes.
void Create_Processes(Process processes[], int count) {
    // Predefined values for 5 processes:
    int at[] = {0, 2, 3, 4, 5};          // Arrival times
    int cb[] = {10, 7, 8, 6, 5};         // CPU burst times
    int io_start[] = {-1, 2, 3, -1, 1};  // When I/O begins (-1 means no I/O)
    int io_burst[] = {0, 3, 2, 0, 2};    // I/O burst durations
    int prio[] = {3, 2, 5, 1, 4};        // Priority levels (lower number = higher priority)

    for (int i = 0; i < count; i++) {
        processes[i].pid = i + 1;                       // Assign unique process ID (PID)
        processes[i].arrival_time = at[i];              // Set arrival time
        processes[i].cpu_burst = cb[i];                 // Set total CPU burst time
        processes[i].remaining_time = cb[i];            // Initialize remaining time for scheduling
        processes[i].priority = prio[i];                // Set static priority value

        // Configure I/O attributes
        processes[i].has_io = io_start[i] != -1 ? 1 : 0; // Determine if I/O exists
        processes[i].io_start_time = io_start[i];        // Set I/O start time
        processes[i].io_burst = io_burst[i];             // Set I/O duration
        processes[i].io_done = 0;                        // I/O not yet completed
        processes[i].io_remaining_time = 0;              // Start with no remaining I/O time

        // Initialize final metrics (used in output calculations)
        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;

        // Output process configuration
        printf("PID %d: has_io = %d (io_start = %d, io_burst = %d)\n",
               processes[i].pid,
               processes[i].has_io,
               processes[i].io_start_time,
               processes[i].io_burst);
    }

    printf("Create_Processes() from process.c 실행됨!!\n"); // Confirmation message
}

// Displays a formatted table showing process attributes.
void Print_Processes(Process processes[], int count) {
    printf("PID\tAT\tCPU\tIO@\tIOBurst\tPrio\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].cpu_burst,
               processes[i].io_start_time,
               processes[i].io_burst,
               processes[i].priority);
    }
}
