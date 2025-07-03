#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>  

#define MAX 1000

// Function to calculate absolute seek time
int calc_seek(int a, int b) {
    return abs(a - b);
}

// Comparator for qsort (ascending order)
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Print 
void print_stats(const char* name, int sequence[], int n, int total_seek) {
    printf("\n%s Disk Scheduling:\n", name);
    printf("Seek Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", sequence[i]);
    }
    printf("\nTotal Seek Time: %d", total_seek);
    printf("\nAverage Seek Time: %.2f\n", (float)total_seek / n);
}

// FCFS Algorithm
void fcfs(int request[], int n, int head) {
    int total_seek = 0;
    int sequence[MAX];
    for (int i = 0; i < n; i++) {
        sequence[i] = request[i];
        total_seek += calc_seek(head, request[i]);
        head = request[i];
    }
    print_stats("FCFS", sequence, n, total_seek);
}

// SSTF Algorithm
void sstf(int request[], int n, int head) {
    int total_seek = 0, completed = 0;
    bool visited[MAX] = {false};
    int sequence[MAX];

    while (completed < n) {
        int min_dist = 1e9, index = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int dist = calc_seek(head, request[i]);
                if (dist < min_dist) {
                    min_dist = dist;
                    index = i;
                }
            }
        }
        visited[index] = true;
        sequence[completed++] = request[index];
        total_seek += min_dist;
        head = request[index];
    }
    print_stats("SSTF", sequence, n, total_seek);
}

// SCAN Algorithm
void scan(int request[], int n, int head, int disk_size, const char* direction) {
    int total_seek = 0, seq_index = 0;
    int sequence[MAX];
    int left[MAX], right[MAX], l = 0, r = 0;

    for (int i = 0; i < n; i++) {
        if (request[i] < head) left[l++] = request[i];
        else right[r++] = request[i];
    }

    qsort(left, l, sizeof(int), compare);
    qsort(right, r, sizeof(int), compare);

    if (strcmp(direction, "up") == 0) {
        for (int i = 0; i < r; i++) {
            sequence[seq_index++] = right[i];
            total_seek += calc_seek(head, right[i]);
            head = right[i];
        }
        if (head != disk_size - 1) {
            total_seek += calc_seek(head, disk_size - 1);
            head = disk_size - 1;
        }
        for (int i = l - 1; i >= 0; i--) {
            sequence[seq_index++] = left[i];
            total_seek += calc_seek(head, left[i]);
            head = left[i];
        }
    } else {
        for (int i = l - 1; i >= 0; i--) {
            sequence[seq_index++] = left[i];
            total_seek += calc_seek(head, left[i]);
            head = left[i];
        }
        if (head != 0) {
            total_seek += calc_seek(head, 0);
            head = 0;
        }
        for (int i = 0; i < r; i++) {
            sequence[seq_index++] = right[i];
            total_seek += calc_seek(head, right[i]);
            head = right[i];
        }
    }

    print_stats("SCAN", sequence, seq_index, total_seek);
}

int main() {
    int disk_size = 200;
    int head = 50;
    int request[] = {98, 183, 37, 122, 14, 124, 65, 67, 44, 45, 73};
    int n = sizeof(request) / sizeof(request[0]);

    printf("Initial Head Position: %d\n", head);
    printf("Request Queue: ");
    for (int i = 0; i < n; i++) printf("%d ", request[i]);
    printf("\nDisk Size: 0 to %d\n", disk_size - 1);

    fcfs(request, n, head);
    sstf(request, n, head);
    scan(request, n, head, disk_size, "up"); 

    return 0;
}