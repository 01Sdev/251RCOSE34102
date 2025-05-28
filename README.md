# CPU Scheduling Simulator with I/O Support

This program simulates various CPU scheduling algorithms with support for I/O operations. It helps visualize how different strategies perform based on waiting time, turnaround time, and completion time.

## Features

- Implements the following scheduling algorithms:
  - First-Come, First-Served (FCFS) with I/O
  - Shortest Job First (SJF) – Non-Preemptive
  - Shortest Job First (SJF) – Preemptive
  - Priority Scheduling – Non-Preemptive
  - Priority Scheduling – Preemptive
  - Round Robin (with configurable time quantum, default: 3)
- Supports I/O operations: processes can be temporarily moved to a waiting queue
- Calculates and prints metrics (WT, TT, CT) and average values
- Gantt chart generation for each algorithm
- Manual and automatic process creation

## File Structure

```
.
├── main.c                         # Program entry point
├── process.c / process.h          # Process creation and printing
├── queue.c / queue.h              # Queue operations
├── fcfs_with_io.c / .h            # FCFS algorithm with I/O
├── sjf_non_preemptive.c / .h      # SJF non-preemptive implementation
├── sjf_preemptive.c / .h          # SJF preemptive implementation
├── priority_non_preemptive.c / .h # Priority scheduling non-preemptive
├── priority_preemptive.c / .h     # Priority scheduling preemptive
├── round_robin.c / .h             # Round Robin algorithm
├── scheduler.h                    # Shared function declarations
```

## How to Compile

```bash
gcc main.c process.c queue.c fcfs_with_io.c sjf_non_preemptive.c sjf_preemptive.c     priority_non_preemptive.c priority_preemptive.c round_robin.c -o main
```

## How to Run

### Automatic Mode (default values)

```bash
./main
```

### Manual Mode (user input)

```bash
./main -manual
```

Manually enter the following values per process:
- Arrival time
- CPU burst time
- I/O (whether it exists or not)
- I/O start time (if applicable)
- I/O burst duration (if applicable)
- Priority (1 = highest, 5 = lowest)

## Output

- Each algorithm prints:
  - Metrics for each process: Waiting Time (WT), Turnaround Time (TT), Completion Time (CT)
  - Average WT and TT
  - A Gantt chart showing the execution timeline

## Notes

- This project is structured for readability and easy grading.
- The codebase is fully commented in English.
- Output is presented in Korean to match the original context.
- Designed for academic use and process scheduling demonstration.
