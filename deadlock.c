#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int available[MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int request[MAX_PROCESSES][MAX_RESOURCES];
bool finish[MAX_PROCESSES];
int work[MAX_RESOURCES];
int num_processes, num_resources;

bool is_deadlock() {
    for (int i = 0; i < num_resources; i++) work[i] = available[i]; //copying available res to work
    for (int i = 0; i < num_processes; i++) {
        finish[i] = true; //assumes true
        for (int j = 0; j < num_resources; j++) {
            if (allocation[i][j] != 0) { //if process has any allocated res
                finish[i] = false; //mark unfinished
                break;
            }
        }
    }
    bool found;
    //finish processes by allocating req res
    do {
        found = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) { //check unfinished process
                bool can_finish = true;
                for (int j = 0; j < num_resources; j++) {
                    if (request[i][j] > work[j]) { //if req cannot be granted
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) { //if process can finish
                    for (int j = 0; j < num_resources; j++) work[j] += allocation[i][j]; //relrease allocated res
                    finish[i] = true;
                    found = true; //a process was finished so repeat checking
                }
            }
        }
    } while (found); //repeat untill no more processes can finish

    for (int i = 0; i < num_processes; i++) {
        if (!finish[i]) return true; //if unfinished -> deadlock
    }
    return false; //no deadlock
}

void find_smallest_set() {
    int min_set_size = num_processes + 1;
    int best_set[MAX_PROCESSES];
    int current_set[MAX_PROCESSES]; //temp
    //mask -> binary number where each bit represents whether a process is there in the subset or not
    for (int mask = 1; mask < (1 << num_processes); mask++) { //checks which processes are in the current subset
        int set_size = 0;
        for (int i = 0; i < num_processes; i++) {
            if (mask & (1 << i)) current_set[set_size++] = i;
        }
        //temporarily terminate the selected res
        for (int i = 0; i < set_size; i++) {
            int process = current_set[i];
            for (int j = 0; j < num_resources; j++) {
                available[j] += allocation[process][j]; //release res
                allocation[process][j] = 0;
                request[process][j] = 0;
            }
        }
        //checks if deadlock resolved
        if (!is_deadlock() && set_size < min_set_size) {
            min_set_size = set_size;
            for (int i = 0; i < set_size; i++) best_set[i] = current_set[i]; //if the current set is smaller than the previous, update the best set
        }
        //restore the original state to continue testing other sets
        for (int i = 0; i < set_size; i++) {
            int process = current_set[i];
            for (int j = 0; j < num_resources; j++) {
                available[j] -= allocation[process][j];
                allocation[process][j] = request[process][j] = 0;
            }
        }
    }

    if (min_set_size <= num_processes) {
        printf("Deadlock exists. Smallest set of processes to terminate: ");
        for (int i = 0; i < min_set_size; i++) printf("P%d ", best_set[i]);
        printf("\n");
    } else {
        printf("No deadlock.\n");
    }
}

int main() {
    printf("Enter number of resources: ");
    scanf("%d", &num_resources);
    printf("Enter number of instances for each resource: ");
    for (int i = 0; i < num_resources; i++) scanf("%d", &available[i]);

    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter allocation matrix:\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
            available[j] -= allocation[i][j];
        }
    }

    printf("Enter request matrix:\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &request[i][j]);
        }
    }

    if (is_deadlock()) find_smallest_set();
    else printf("No deadlock.\n");

    return 0;
}