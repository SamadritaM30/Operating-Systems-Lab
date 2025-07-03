#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define MEMORY_SIZE 262144
#define NUM_FRAMES (MEMORY_SIZE / PAGE_SIZE)
#define NUM_PAGES 64

typedef struct {
    int frame;
    int valid;
} PageTableEntry;

PageTableEntry pageTable[NUM_PAGES];
char physicalMemory[MEMORY_SIZE];

void initPageTable() {
    for (int i = 0; i < NUM_PAGES; i++) {
        pageTable[i].valid = 0;
        pageTable[i].frame = -1;
    }
}

void accessMemory(int logicalAddress) {
    int page = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;

    if (page >= NUM_PAGES || !pageTable[page].valid) {
        printf("Invalid Address\n");
        return;
    }

    int physicalAddress = (pageTable[page].frame * PAGE_SIZE) + offset;
    printf("Logical Address: %d -> Physical Address: %d\n", logicalAddress, physicalAddress);
}

int main() {
    int logicalAddress;
    initPageTable();

    while (1) {
        printf("Enter logical address (-1 to exit): ");
        scanf("%d", &logicalAddress);
        if (logicalAddress == -1) break;
        accessMemory(logicalAddress);
    }

    return 0;
}
