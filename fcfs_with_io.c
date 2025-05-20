#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define MAX_PROCESSES 10

// Updates the waiting queue by decreasing I/O time
// If a process finishes its I/O, move it back to the ready queue
void update_waiting_queue(Queue* waiting, Queue* ready) {
    int n = waiting->size;
    for (int i = 0; i < n; i++) {
        Process* p = dequeue(waiting);
        if (p == NULL) continue;

        p->io_remaining_time--;

        if (p->io_remaining_time <= 0) {
            p->io_done = 1;
            enqueue(ready, p);
        } else {
            enqueue(waiting, p);
        }
    }
}

// First-Come, First-Served scheduling algorithm with I/O support
void fcfs_with_io(Process processes[], int count) {
    Queue ready_queue, waiting_queue;
    init_queue(&ready_queue);
    init_queue(&waiting_queue);

    // Sort processes by arrival time (ascending)
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
    Process* gantt[MAX_PROCESSES * 5];
    int timeline[MAX_PROCESSES * 5];
    int step = 0;

    printf("\n=== FCFS + I/O 결과 ===\n");

    // Main simulation loop
    while (completed < count) {
        printf("[시간 %d] 루프 시작\n", current_time);

        // Add newly arrived processes to ready queue
        for (int i = 0; i < count; i++) {
            if (!inserted[i] && processes[i].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[i]);
                inserted[i] = 1;
                printf("  → 프로세스 %d 도착\n", processes[i].pid);
            }
        }

        // Handle I/O completions
        update_waiting_queue(&waiting_queue, &ready_queue);

        if (is_empty(&ready_queue)) {
            printf("  → Ready Queue 비어있음, 시간 증가\n");
            current_time++;
            continue;
        }

        // Select next process
        Process* p = dequeue(&ready_queue);
        if (p == NULL) {
            current_time++;
            continue;
        }

        printf("  → 프로세스 %d 실행 중\n", p->pid);

        // Handle I/O request when it's time
        if (p->has_io && p->io_done == 0 && p->cpu_burst - p->remaining_time == p->io_start_time) {
            printf("  → I/O 요청 발생: 프로세스 %d\n", p->pid);
            p->io_remaining_time = p->io_burst;
            enqueue(&waiting_queue, p);
            current_time++;
            continue;
        }

        // Record execution for Gantt chart
        gantt[step] = p;
        timeline[step] = current_time;
        step++;

        // Run for one time unit
        p->remaining_time--;
        current_time++;

        // If process finishes
        if (p->remaining_time == 0) {
            printf("  → 프로세스 %d 완료\n", p->pid);
            p->completion_time = current_time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->cpu_burst;
            completed++;
        } else {
            // Put back into ready queue
            enqueue(&ready_queue, p);
        }
    }

    // Print per-process results
    for (int i = 0; i < count; i++) {
        printf("PID %d: WT=%d, TT=%d, CT=%d\n",
               processes[i].pid,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }

    // Print average metrics
    int total_wt = 0, total_tt = 0;
    for (int i = 0; i < count; i++) {
        total_wt += processes[i].waiting_time;
        total_tt += processes[i].turnaround_time;
    }
    printf("\nAverage WT = %.2f, Average TT = %.2f\n",
           (float)total_wt / count, (float)total_tt / count);

    // Print Gantt chart
    if (step > 0) {
        printf("\nGantt Chart:\n ");
        for (int i = 0; i < step; i++) {
            if (gantt[i] != NULL)
                printf("| P%d ", gantt[i]->pid);
            else
                printf("| ?? ");
        }
        printf("|\n");

        printf("%d", timeline[0]);
        for (int i = 1; i < step; i++) {
            printf("   %2d", timeline[i]);
        }
        printf("   %2d\n", current_time);
    }
}
