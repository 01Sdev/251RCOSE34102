#include <stdio.h>
#include "scheduler.h"

// Non-preemptive Priority Scheduling
void priority_non_preemptive(Process processes[], int count) {
    int current_time = 0;
    int completed = 0;
    int step = 0;
    int inserted[100] = {0};

    Process* gantt[500];
    int timeline[500];

    printf("\n=== Non-Preemptive Priority Scheduling Results ===\n");

    // Repeat until all processes are completed
    while (completed < count) {
        int highest_prio = 9999;  // Initialize to a large number
        int selected = -1;        // Index of the selected process

        // Find the highest-priority process that has arrived
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].priority < highest_prio) {
                    highest_prio = processes[i].priority;
                    selected = i;
                } else if (processes[i].priority == highest_prio &&
                           processes[i].arrival_time < processes[selected].arrival_time) {
                    // Tie-breaker: earlier arrival time wins
                    selected = i;
                }
            }
        }

        // If no process is ready yet, increment time
        if (selected == -1) {
            current_time++;
            continue;
        }

        // Select and execute the chosen process
        Process* p = &processes[selected];
        gantt[step] = p;                      // Store in Gantt chart
        timeline[step++] = current_time;      // Record start time

        p->waiting_time = current_time - p->arrival_time;       // Time spent waiting
        current_time += p->cpu_burst;                           // Advance time by CPU burst
        p->completion_time = current_time;                      // Mark completion time
        p->turnaround_time = p->completion_time - p->arrival_time;

        inserted[selected] = 1;
        completed++;
    }

    // Output individual process statistics
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Calculate and print average waiting and turnaround time
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }

    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);

    // Draw Gantt Chart for execution order
    printf("\nNon-Preemptive Priority Gantt Chart:\n ");
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
