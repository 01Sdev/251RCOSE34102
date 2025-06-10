#include <stdio.h>
#include "process.h"

void Create_Processes(Process processes[], int count) {
    int at[] = {0, 2, 3, 4, 5};
    int cb[] = {10, 7, 8, 6, 5};
    int prio[] = {3, 2, 5, 1, 4};

    for (int i = 0; i < count; i++) {
        processes[i].pid = i + 1;
        processes[i].arrival_time = at[i];
        processes[i].cpu_burst = cb[i];
        processes[i].remaining_time = cb[i];
        processes[i].priority = prio[i];

        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}

void Print_Processes(Process processes[], int count) {
    printf("PID\tAT\tCPU\tPrio\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].cpu_burst,
               processes[i].priority);
    }
}
