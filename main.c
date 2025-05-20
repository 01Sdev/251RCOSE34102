#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "process.h"
#include "queue.h"
#include "fcfs_with_io.h"
#include "sjf_non_preemptive.h"
#include "sjf_preemptive.h"
#include "priority_non_preemptive.h"
#include "priority_preemptive.h"
#include "round_robin.h"

#define MAX_PROCESSES 5

int main(int argc, char* argv[]) {
    Process processes[MAX_PROCESSES];

    // Manual mode: user inputs process information at runtime
    if (argc == 2 && strcmp(argv[1], "-manual") == 0) {
        printf("🔧 수동 입력 모드입니다. 프로세스 정보를 입력하세요.\n");
        for (int i = 0; i < MAX_PROCESSES; i++) {
            processes[i].pid = i + 1;

            // Input basic process attributes
            printf("프로세스 %d\n", i + 1);
            printf("도착 시간: "); scanf("%d", &processes[i].arrival_time);
            printf("CPU Burst: "); scanf("%d", &processes[i].cpu_burst);
            processes[i].remaining_time = processes[i].cpu_burst;

            // Input I/O information if applicable
            printf("I/O 있음? (1: 있음, 0: 없음): "); scanf("%d", &processes[i].has_io);
            if (processes[i].has_io) {
                printf("  I/O 시작 시점: "); scanf("%d", &processes[i].io_start_time);
                printf("  I/O 지속 시간: "); scanf("%d", &processes[i].io_burst);
            } else {
                processes[i].io_start_time = -1;
                processes[i].io_burst = 0;
            }

            // Input scheduling priority
            printf("우선순위 (1~5): "); scanf("%d", &processes[i].priority);

            // Initialize additional attributes
            processes[i].io_done = 0;
            processes[i].io_remaining_time = 0;
            processes[i].completion_time = 0;
            processes[i].waiting_time = 0;
            processes[i].turnaround_time = 0;
            printf("\n");
        }
        printf("✅ 사용자 정의 프로세스 입력 완료!\n");

    // Otherwise use the default built-in process list
    } else {
        Create_Processes(processes, MAX_PROCESSES);
    }

    // Display all input process information
    Print_Processes(processes, MAX_PROCESSES);

    // === FCFS + I/O ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        fcfs_with_io(temp, MAX_PROCESSES);
    }

    // === SJF (Non-Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        sjf_non_preemptive(temp, MAX_PROCESSES);
    }

    // === SJF (Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        sjf_preemptive(temp, MAX_PROCESSES);
    }

    // === Priority (Non-Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        priority_non_preemptive(temp, MAX_PROCESSES);
    }

    // === Priority (Preemptive) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        priority_preemptive(temp, MAX_PROCESSES);
    }

    // === Round Robin (Time Quantum = 3) ===
    {
        Process temp[MAX_PROCESSES];
        for (int i = 0; i < MAX_PROCESSES; i++) temp[i] = processes[i];
        round_robin(temp, MAX_PROCESSES, 3);
    }

    return 0;
}
