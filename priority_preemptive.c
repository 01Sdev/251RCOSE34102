#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define MAX_STEPS 1000

// Preemptive Priority Scheduling Algorithm
void priority_preemptive(Process processes[], int count) {
    Queue ready_queue;
    init_queue(&ready_queue);

    int current_time = 0;
    int completed = 0;
    int inserted[100] = {0};
    Process* raw_gantt[MAX_STEPS];
    int raw_timeline[MAX_STEPS];
    int step = 0;

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
        raw_gantt[step] = current;
        raw_timeline[step] = current_time;
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
        printf("\n Preemptive Priority Gantt Chart:\n ");

        // Print process IDs with compression
        printf(" ");
        int prev_pid = raw_gantt[0]->pid;
        printf("| P%d ", prev_pid);

        for (int i = 1; i < step; i++) {
            if (raw_gantt[i]->pid != prev_pid) {
                printf("| P%d ", raw_gantt[i]->pid);
                prev_pid = raw_gantt[i]->pid;
            }
        }
        printf("|\n");

        // Print time markers
        int time_marker = raw_timeline[0];
        printf("%2d", time_marker);
        prev_pid = raw_gantt[0]->pid;

        for (int i = 1; i < step; i++) {
            if (raw_gantt[i]->pid != prev_pid) {
                printf("   %2d", raw_timeline[i]);
                prev_pid = raw_gantt[i]->pid;
            }
        }
        printf("   %2d\n", current_time);
    }
}
