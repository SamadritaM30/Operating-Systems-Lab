#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int turnaround;
} Process;

void roundRobin(Process processes[], int n, int quantum) {
    int time = 0; //current system
    int completed = 0; //how many
    int *ganttChart = (int *)malloc(1000 * sizeof(int));
    int *startTimes = (int *)malloc(1000 * sizeof(int));
    int ganttIndex = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining = processes[i].burst;
    }

    while (completed < n) {
        int flag = 0; //to check if any process executes in the current cycle

        for (int i = 0; i < n; i++) {
            if (processes[i].remaining > 0 && processes[i].arrival <= time) {
                flag = 1;
                startTimes[ganttIndex] = time;

                int executeTime = (processes[i].remaining < quantum) ? processes[i].remaining : quantum;
                processes[i].remaining -= executeTime;
                time += executeTime;

                ganttChart[ganttIndex++] = processes[i].pid;
                startTimes[ganttIndex] = time;

                if (processes[i].remaining == 0) {
                    completed++;
                    processes[i].turnaround = time - processes[i].arrival;
                    processes[i].waiting = processes[i].turnaround - processes[i].burst;
                }
            }
        }

        if (flag == 0) {
            time++;
            ganttChart[ganttIndex++] = -1; //to indicate idle time
            startTimes[ganttIndex] = time;
        }
    }

    printf("Gantt Chart:\n");
    for (int i = 0; i < ganttIndex; i++) {
        if (ganttChart[i] == -1) {
            printf("| IDLE ");
        } else {
            printf("| P%d (%d-%d) ", ganttChart[i], startTimes[i], startTimes[i + 1]);
        }
    }
    printf("|\n");

    float totalWaiting = 0, totalTurnaround = 0;
    for (int i = 0; i < n; i++) {
        totalWaiting += processes[i].waiting;
        totalTurnaround += processes[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f\n", totalWaiting / n);
    printf("Average Turnaround Time: %.2f\n", totalTurnaround / n);

    free(ganttChart);
    free(startTimes);
}

int main() {
    int n, quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    Process processes[n];
    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d:\n", i + 1);
        processes[i].pid = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst);
    }

    roundRobin(processes, n, quantum);

    return 0;
}
