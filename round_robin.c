#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define TIME_QUANTUM 3

// Implements the Round Robin Scheduling Algorithm
void round_robin(Process processes[], int count, int tq) {
    Queue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int completed = 0;
    int inserted[100] = {0};
    Process* gantt[1000];
    int timeline[1000];
    int step = 0;

    printf("\n=== Round Robin (TQ=%d) Results ===\n", tq);

    while (completed < count) {
        // Enqueue newly arrived processes
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[i]);
                inserted[i] = 1;
            }
        }

        if (is_empty(&ready_queue)) {
            // CPU is idle, skip time
            current_time++;
            continue;
        }

        Process* current = dequeue(&ready_queue);
        int burst = (current->remaining_time < tq) ? current->remaining_time : tq;
        int finished_early = 0;

        // ✅ Record in Gantt Chart ONLY when switching to a new process
        if (step == 0 || gantt[step - 1] != current) {
            gantt[step] = current;
            timeline[step] = current_time;
            step++;
        }

        for (int i = 0; i < burst; i++) {
            current->remaining_time--;
            current_time++;

            // Check for newly arrived processes during burst
            for (int j = 0; j < count; j++) {
                if (!inserted[j] && processes[j].arrival_time <= current_time) {
                    enqueue(&ready_queue, &processes[j]);
                    inserted[j] = 1;
                }
            }

            if (current->remaining_time == 0) {
                current->completion_time = current_time;
                current->turnaround_time = current->completion_time - current->arrival_time;
                current->waiting_time = current->turnaround_time - current->cpu_burst;
                completed++;
                finished_early = 1;
                break;
            }
        }

        if (!finished_early) {
            enqueue(&ready_queue, current);
        }
    }

    // Display results
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Gantt Chart (FCFS-style)
    if (step > 0) {
        printf("\nRound Robin Gantt Chart:\n ");
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

    // Averages
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }
    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);
}
