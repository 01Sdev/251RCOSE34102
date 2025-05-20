// === round_robin.c ===
#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define TIME_QUANTUM 3

// Implements the Round Robin Scheduling Algorithm
void round_robin(Process processes[], int count, int tq) {
    Queue ready_queue, waiting_queue;
    init_queue(&ready_queue);
    init_queue(&waiting_queue);

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

        // Process I/O completion
        int wq_size = waiting_queue.size;
        for (int i = 0; i < wq_size; i++) {
            Process* p = dequeue(&waiting_queue);
            if (p == NULL) continue;

            p->io_remaining_time--;
            if (p->io_remaining_time <= 0) {
                p->io_done = 1;
                enqueue(&ready_queue, p);
            } else {
                enqueue(&waiting_queue, p);
            }
        }

        if (is_empty(&ready_queue)) {
            current_time++;
            continue;
        }

        Process* p = dequeue(&ready_queue);
        int burst = (p->remaining_time < tq) ? p->remaining_time : tq;

        for (int i = 0; i < burst; i++) {
            // I/O check during execution
            if (p->has_io && p->io_done == 0 && (p->cpu_burst - p->remaining_time) == p->io_start_time) {
                printf("  \u2192 I/O request: Process %d (Remaining CPU=%d)\n", p->pid, p->remaining_time);
                p->io_remaining_time = p->io_burst;
                enqueue(&waiting_queue, p);
                current_time++;
                goto end_of_loop;
            }

            gantt[step] = p;
            timeline[step] = current_time;
            step++;
            p->remaining_time--;
            current_time++;

            // Process completion
            if (p->remaining_time == 0) {
                p->completion_time = current_time;
                p->turnaround_time = p->completion_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->cpu_burst;
                completed++;
                goto end_of_loop;
            }
        }

        enqueue(&ready_queue, p);  // Time quantum expired; re-enqueue
end_of_loop:;
    }

    // Display results
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Display Gantt chart
    if (step > 0) {
        printf("\nGantt Chart:\n ");
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
