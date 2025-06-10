#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define MAX_STEPS 1000

void sjf_preemptive(Process *processes, int n) {
    int completed = 0, current_time = 0, min_burst, shortest = -1;
    int is_completed[n];
    int remaining_time[n];

    for (int i = 0; i < n; i++) {
        is_completed[i] = 0;
        remaining_time[i] = processes[i].cpu_burst;
    }

    // Gantt chart trace
    int gantt_pids[100]; 
    int gantt_times[100];
    int gantt_index = 0;

    int prev = -1;

    while (completed != n) {
        shortest = -1;
        min_burst = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                !is_completed[i] &&
                remaining_time[i] < min_burst &&
                remaining_time[i] > 0) {
                min_burst = remaining_time[i];
                shortest = i;
            }
        }

        if (shortest == -1) {
            current_time++;
            continue;
        }

        if (prev != processes[shortest].pid) {
            gantt_times[gantt_index] = current_time;
            gantt_pids[gantt_index] = processes[shortest].pid;
            gantt_index++;
            prev = processes[shortest].pid;
        }

        remaining_time[shortest]--;
        current_time++;

        if (remaining_time[shortest] == 0) {
            is_completed[shortest] = 1;
            completed++;
            processes[shortest].completion_time = current_time;
        }
    }

    gantt_times[gantt_index] = current_time;

    // Gantt Chart 
    printf("Gantt Chart:\n");
    for (int i = 0; i < gantt_index; i++) {
        printf("| P%d ", gantt_pids[i]);
    }
    printf("|\n");

    for (int i = 0; i <= gantt_index; i++) {
        printf("%-4d", gantt_times[i]);
    }
    printf("\n");
}
