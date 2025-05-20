#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define MAX_STEPS 1000

// Shortest Job First (Preemptive) Scheduling with I/O handling
void sjf_preemptive(Process processes[], int count) {
    Queue ready_queue, waiting_queue;
    init_queue(&ready_queue);
    init_queue(&waiting_queue);

    int current_time = 0;
    int completed = 0;
    int step = 0;

    Process* gantt[MAX_STEPS];  // To visualize the execution order
    int timeline[MAX_STEPS];    // Time markers for each step
    int inserted[100] = {0};    // Track which processes have been added

    printf("\n=== SJF (Preemptive) Result ===\n");

    while (completed < count) {
        // Step 1: Add any newly arrived processes to the ready queue
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[i]);
                inserted[i] = 1;
            }
        }

        // Step 2: Update waiting queue (I/O simulation)
        int wsize = waiting_queue.size;
        for (int i = 0; i < wsize; i++) {
            Process* p = dequeue(&waiting_queue);
            if (!p) continue;

            p->io_remaining_time--;
            if (p->io_remaining_time <= 0) {
                p->io_done = 1;
                enqueue(&ready_queue, p);
            } else {
                enqueue(&waiting_queue, p);  // Still waiting on I/O
            }
        }

        // Step 3: Select the process with the shortest remaining time
        Process* current = NULL;
        Node* iter = ready_queue.front;
        int shortest = 1e9;

        while (iter != NULL) {
            Process* p = iter->process;
            if (p->remaining_time > 0 && p->remaining_time < shortest) {
                shortest = p->remaining_time;
                current = p;
            }
            iter = iter->next;
        }

        // If no process is ready, skip time forward
        if (!current) {
            current_time++;
            continue;
        }

        // Step 4: Check for I/O request
        int cpu_used = current->cpu_burst - current->remaining_time;
        if (current->has_io && !current->io_done && cpu_used == current->io_start_time) {
            current->io_remaining_time = current->io_burst;
            enqueue(&waiting_queue, current);
            current_time++;
            continue;
        }

        // Step 5: Execute one time unit
        gantt[step] = current;
        timeline[step++] = current_time;
        current->remaining_time--;
        current_time++;

        // Step 6: Completion check
        if (current->remaining_time == 0) {
            current->completion_time = current_time;
            current->turnaround_time = current->completion_time - current->arrival_time;
            current->waiting_time = current->turnaround_time - current->cpu_burst;
            completed++;
        }
    }

    // Final Output: Per-process statistics
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Average Time Calculation
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }
    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);

    // Gantt Chart
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
}
