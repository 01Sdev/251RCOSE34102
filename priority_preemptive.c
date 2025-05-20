#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define MAX_STEPS 1000

// Priority Scheduling (Preemptive)
// Processes are scheduled based on priority (lower number = higher priority).
// At each time unit, the scheduler checks for the process with the highest priority
// among the ready processes and runs it for 1 unit (preemption possible).
void priority_preemptive(Process processes[], int count) {
    Queue ready_queue;
    init_queue(&ready_queue); // Initialize the ready queue

    int current_time = 0;         // Current simulation time
    int completed = 0;            // Number of completed processes
    int inserted[100] = {0};      // Tracks which processes have been inserted
    Process* gantt[MAX_STEPS];    // Stores the order of process execution for Gantt chart
    int timeline[MAX_STEPS];      // Stores timestamps for Gantt chart
    int step = 0;                 // Step counter for Gantt chart

    printf("\n=== Priority Scheduling (Preemptive) 결과 ===\n");

    while (completed < count) {
        // Insert all processes that have arrived into the ready queue
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[i]);
                inserted[i] = 1;
            }
        }

        // Select the ready process with the highest priority (lowest number)
        Process* current = NULL;
        Node* iter = ready_queue.front;
        int highest_priority = 1e9;

        while (iter != NULL) {
            if (iter->process->remaining_time > 0 &&
                iter->process->priority < highest_priority) {
                highest_priority = iter->process->priority;
                current = iter->process;
            }
            iter = iter->next;
        }

        // If no process is ready to run, skip the current time unit
        if (current == NULL) {
            current_time++;
            continue;
        }

        // Record the currently selected process for the Gantt chart
        gantt[step] = current;
        timeline[step] = current_time;
        step++;

        // Simulate 1 unit of execution time
        current->remaining_time--;
        current_time++;

        // If the process has finished execution
        if (current->remaining_time == 0) {
            current->completion_time = current_time;
            current->turnaround_time = current->completion_time - current->arrival_time;
            current->waiting_time = current->turnaround_time - current->cpu_burst;
            completed++;
        }
    }

    // Print individual process metrics
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Calculate and display average waiting and turnaround times
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }

    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);

    // Print Gantt chart to visualize execution timeline
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
