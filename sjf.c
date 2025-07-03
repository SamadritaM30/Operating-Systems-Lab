#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char id[10];
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

void sjf_scheduling(Process processes[], int n) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0; 
    int current_time = 0; 
    int *completed = (int *)calloc(n, sizeof(int)); //0 -> no completed, 1 -> completed

    printf("Gantt Chart:\n");

    while (1) {
        int shortest_job_index = -1; 
        int shortest_burst_time = 999999; 

        //to find the process with shortest burst time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !completed[i] && processes[i].burst_time < shortest_burst_time) {
                shortest_burst_time = processes[i].burst_time;
                shortest_job_index = i;
            }
        }

        
        if (shortest_job_index == -1) {
            break;
        }

        completed[shortest_job_index] = 1;
        processes[shortest_job_index].waiting_time = current_time - processes[shortest_job_index].arrival_time;
        processes[shortest_job_index].turnaround_time = processes[shortest_job_index].waiting_time + processes[shortest_job_index].burst_time;

        total_waiting_time += processes[shortest_job_index].waiting_time;
        total_turnaround_time += processes[shortest_job_index].turnaround_time;

        printf("%s (%d-%d) | ", processes[shortest_job_index].id, current_time, current_time + processes[shortest_job_index].burst_time);

        current_time += processes[shortest_job_index].burst_time;
    }

    printf("\n");

    float avg_waiting_time = (float)total_waiting_time / n;
    float avg_turnaround_time = (float)total_turnaround_time / n;

    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);

    free(completed);
}

int main() {
    Process processes[] = {
        {"P1", 0, 8},
        {"P2", 1, 4},
        {"P3", 2, 9},
        {"P4", 3, 5}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    sjf_scheduling(processes, n);

    return 0;
}
