#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define MAX_PROCESSES 10

void fcfs(Process processes[], int count) {
    Queue ready_queue;
    init_queue(&ready_queue);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    int completed = 0;
    int inserted[MAX_PROCESSES] = {0};
    Process* gantt[MAX_PROCESSES];
    int timeline[MAX_PROCESSES];
    int step = 0;

    Process* running_process = NULL;

    printf("\n=== FCFS Results ===\n");

    while (completed < count) {
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[i]);
                inserted[i] = 1;
                printf("[Time: %d] Process %d arrived\n", current_time, processes[i].pid);
            }
        }

        if (running_process == NULL && !is_empty(&ready_queue)) {
            running_process = dequeue(&ready_queue);

            // Record the process in the Gantt chart
            gantt[step] = running_process;
            timeline[step] = current_time;
            step++;

            printf("[Time %d] Process started %d\n", current_time, running_process->pid);
        }

        if (running_process == NULL) {
            current_time++;
            continue;
        }

        running_process->remaining_time--;
        current_time++;

        // Record the current process in the Gantt chart
        if (running_process->remaining_time == 0) {
            running_process->completion_time = current_time;
            running_process->turnaround_time = current_time - running_process->arrival_time;
            running_process->waiting_time = running_process->turnaround_time - running_process->cpu_burst;

            printf("[Time: %d] Process finished %d\n", current_time, running_process->pid);
            running_process = NULL;
            completed++;
        }
    }
    printf("\n");

    // Final Output: Per-process statistics
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Average
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }
    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);

    // Gantt Chart
    if (step > 0) {
        printf("\nFCFS Gantt Chart:\n ");
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
}
