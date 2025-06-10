#include <stdio.h>
#include "scheduler.h"

// Implements the Non-Preemptive SJF Scheduling Algorithm
void sjf_non_preemptive(Process processes[], int count) {
    int completed = 0, current_time = 0;
    int inserted[100] = {0};
    Process* gantt[500];
    int timeline[500];
    int step = 0;

    printf("\n=== SJF (Non-Preemptive) Results ===\n");

    while (completed < count) {
        int shortest_index = -1;
        int shortest_burst = 1e9;

        // Find the process with the shortest CPU burst among arrived processes
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].cpu_burst < shortest_burst) {
                    shortest_burst = processes[i].cpu_burst;
                    shortest_index = i;
                } else if (processes[i].cpu_burst == shortest_burst) {
                    // Tie-breaker: select the one that arrived first
                    if (processes[i].arrival_time < processes[shortest_index].arrival_time) {
                        shortest_index = i;
                    }
                }
            }
        }

        if (shortest_index == -1) {
            current_time++;
            continue;
        }

        Process* p = &processes[shortest_index];
        gantt[step] = p;
        timeline[step] = current_time;
        step++;

        p->waiting_time = current_time - p->arrival_time;
        current_time += p->cpu_burst;
        p->completion_time = current_time;
        p->turnaround_time = p->completion_time - p->arrival_time;

        inserted[shortest_index] = 1;
        completed++;
    }

    // Print individual process results
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Gantt chart
    if (step > 0) {
        printf("\n Non-Preemptive SJF Gantt Chart:\n ");
        for (int i = 0; i < step; i++) {
            printf("| P%d ", gantt[i]->pid);
        }
        printf("|\n");

        printf("%d", timeline[0]);
        for (int i = 1; i < step; i++) {
            printf("   %2d", timeline[i]);
        }
        printf("   %2d\n", current_time);
    }

    // Average
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }
    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);
}
