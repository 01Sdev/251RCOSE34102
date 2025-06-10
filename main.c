#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "process.h"
#include "queue.h"
#include "fcfs.h"
#include "sjf_non_preemptive.h"
#include "sjf_preemptive.h"
#include "priority_non_preemptive.h"
#include "priority_preemptive.h"
#include "round_robin.h"

#define MAX_PROCESSES 5

int main(int argc, char* argv[]) {
    Process processes[MAX_PROCESSES];

    // Manual mode
    if (argc == 2 && strcmp(argv[1], "-manual") == 0) {
        printf("This is manual mode. Please write the data\n");
        for (int i = 0; i < MAX_PROCESSES; i++) {
            processes[i].pid = i + 1;

            printf("Process %d\n", i + 1);
            printf("AT: "); scanf("%d", &processes[i].arrival_time);
            printf("CPU Burst: "); scanf("%d", &processes[i].cpu_burst);
            processes[i].remaining_time = processes[i].cpu_burst;


            printf("Prio (1~5): "); scanf("%d", &processes[i].priority);

            processes[i].completion_time = 0;
            processes[i].waiting_time = 0;
            processes[i].turnaround_time = 0;
            printf("\n");
        }
        printf(" Input finished, start simulation...\n");
    }
    else {
        Create_Processes(processes, MAX_PROCESSES);
    }

    // Display all input process information
    Print_Processes(processes, MAX_PROCESSES);

    // === FCFS + I/O ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        fcfs(temp, MAX_PROCESSES);
    }

    // === SJF (Non-Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        sjf_non_preemptive(temp, MAX_PROCESSES);
    }

    // === SJF (Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        sjf_preemptive(temp, MAX_PROCESSES);
    }

    // === Priority (Non-Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        priority_non_preemptive(temp, MAX_PROCESSES);
    }

    // === Priority (Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        priority_preemptive(temp, MAX_PROCESSES);
    }

    // === Round Robin (Time Quantum = 3) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        round_robin(temp, MAX_PROCESSES, 3);
    }

    return 0;
}
