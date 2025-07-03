#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 1024 //1KB
#define NUM_PAGES 256  //Virtual space = 256 KB
#define NUM_FRAMES 128  //total physical memory = 128 KB
#define TLB_SIZE 16  //TLB has 16 entries 
#define MEMORY_ACCESS_COUNT 1000  //1000 memory accesses

typedef struct { 
    int frame_number;
    int valid;
    int modified;  
    int referenced; 
} PageTableEntry;

typedef struct {  
    int virtual_page;
    int frame_number;
    int valid;
    int time_of_use;  
} TLBEntry;

PageTableEntry page_table[NUM_PAGES];
TLBEntry tlb[TLB_SIZE];
char physical_memory[NUM_FRAMES * PAGE_SIZE];   

int time_counter = 0;
int page_faults = 0, tlb_hits = 0, tlb_misses = 0;

void init_memory() { 
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i].valid = 0;
    }
    
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].valid = 0;
    }
}

int extract_page_number(int virtual_address) {
    return (virtual_address / PAGE_SIZE) % NUM_PAGES;
}

int extract_offset(int virtual_address) {
    return virtual_address % PAGE_SIZE;
}

int check_tlb(int page_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].virtual_page == page_number) {
            tlb_hits++;
            tlb[i].time_of_use = time_counter++;
            return tlb[i].frame_number;
        }
    }

    tlb_misses++;
    return -1;
}

void update_tlb(int page_number, int frame_number) {
    int replace_index = 0, min_time = time_counter;

    for (int i = 0; i < TLB_SIZE; i++) {
        if (!tlb[i].valid) { 
            replace_index = i;
            break;
        }

        if (tlb[i].time_of_use < min_time) { 
            min_time = tlb[i].time_of_use;
            replace_index = i;
        }
    }

    tlb[replace_index].virtual_page = page_number;
    tlb[replace_index].frame_number = frame_number;
    tlb[replace_index].valid = 1;
    tlb[replace_index].time_of_use = time_counter++;
}

int translate_address(int virtual_address) {
    int page_number = extract_page_number(virtual_address);
    int offset = extract_offset(virtual_address);

    int frame_number = check_tlb(page_number);

    if (frame_number == -1) { 
        if (!page_table[page_number].valid) { 
            page_table[page_number].frame_number = page_number % NUM_FRAMES; 
            page_table[page_number].valid = 1; 
            page_faults++;
        }

        frame_number = page_table[page_number].frame_number;
        update_tlb(page_number, frame_number);
    }

    return (frame_number * PAGE_SIZE) + offset;
}

void generate_memory_accesses() {
    for (int i = 0; i < MEMORY_ACCESS_COUNT; i++) {
        int virtual_address = rand() % (NUM_PAGES * PAGE_SIZE); 
        translate_address(virtual_address);
    }
}

void ans() {
    printf("TLB Hits: %d\n", tlb_hits);
    printf("TLB Misses: %d\n", tlb_misses);
    printf("Page Faults: %d\n", page_faults);
    printf("TLB Hit Rate: %.2f%%\n", (tlb_hits * 100.0) / MEMORY_ACCESS_COUNT);
}

int main() {
    init_memory();
    generate_memory_accesses();
    ans();
    return 0;
}