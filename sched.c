#include <stdio.h>
#include <stdlib.h>

#define frames 5
#define pages 10

typedef struct {
    int pg_no ;
    int v;
    int last;
} pt_entry ;

int free_fr(pt_entry pt[]) {
    for (int i = 0; i < frames; i++) {
        if (!pt[i].v)
            return i;
    }
    return -1;
}

int searchpage(pt_entry pt[], int pg ){
    for(int i = 0; i<= frames; i++){
        if(pt[i].v && pt[i].pg_no == pg){
            return i;
        }
    }
    return -1;
}
int fifo(pt_entry pt[]){
    int i ;
}
int optimal(pt_entry pt[]){
    int i;
}

int LRU(pt_entry pt[]){
    int ind = -1, m = 1e9;
    for(int i = 0; i<frames; i++ ){
        if(pt[i].v && pt[i].last < m){
            m = pt[i].last ;
            ind = i;
        }
    }
    return ind;
}
void printing(pt_entry pt[]) {
    printf("Frame\tPage\tv\tLastUsed\n");
    for (int i = 0; i < frames; i++) {
        printf("%d\t%d\t%d\t%d\n", i, pt[i].pg_no, pt[i].v, pt[i].last);
    }
    printf("\n");
}

int main() {
    int ref[20] = {0,7,2,1,3,4,7,8,1,0,2,1,7,7,7,0,2,4,0,3};
    pt_entry pt[frames];
    for (int i = 0; i < frames; i++) {
        pt[i].v= 0;
        pt[i].pg_no= -1;
        pt[i].last= -1;
    }
    int t= 0,pf= 0;
    for (int i = 0; i < 20; i++) { 
        int pg = ref[i];
        int ind = searchpage(pt, pg);

        if (ind != -1) {
            pt[ind].last = t;
        } else {
            pf++;
            int free = free_fr(pt);
            if (free != -1) {
                pt[free].pg_no = pg;
                pt[free].v = 1;
                pt[free].last = t;
            } else {
                int lru_ind = LRU(pt);
                pt[lru_ind].pg_no = pg;
                pt[lru_ind].last = t;
            }
        }
        t ++;
    }

    printing(pt);
    printf("total page faults occured: %d\n", pf);

    return 0;
}






// #include <stdio.h>
// #include <stdlib.h>

// #define FRAME_COUNT 3
// #define PAGE_COUNT 10
// #define REFERENCE_LEN 20

// typedef struct {
//     int page_number;
//     int valid;
//     int last_used;
// } PageTableEntry;

// int findPage(PageTableEntry page_table[], int page) {
//     for (int i = 0; i < FRAME_COUNT; i++) {
//         if (page_table[i].valid && page_table[i].page_number == page)
//             return i;
//     }
//     return -1;
// }

// int findLRU(PageTableEntry page_table[]) {
//     int lru_index = -1, min_used = 1e9;
//     for (int i = 0; i < FRAME_COUNT; i++) {
//         if (page_table[i].valid && page_table[i].last_used < min_used) {
//             min_used = page_table[i].last_used;
//             lru_index = i;
//         }
//     }
//     return lru_index;
// }

// int findFreeFrame(PageTableEntry page_table[]) {
//     for (int i = 0; i < FRAME_COUNT; i++) {
//         if (!page_table[i].valid)
//             return i;
//     }
//     return -1;
// }

// void printPageTable(PageTableEntry page_table[]) {
//     printf("Frame\tPage\tValid\tLastUsed\n");
//     for (int i = 0; i < FRAME_COUNT; i++) {
//         printf("%d\t%d\t%d\t%d\n", i, page_table[i].page_number, page_table[i].valid, page_table[i].last_used);
//     }
//     printf("\n");
// }

// int main() {
//     int reference_string[REFERENCE_LEN] = {0, 2, 1, 4, 2, 3, 0, 3, 2, 1, 5, 6, 2, 3, 4, 5, 3, 2, 1, 0};
//     PageTableEntry page_table[FRAME_COUNT];
//     for (int i = 0; i < FRAME_COUNT; i++) {
//         page_table[i].valid = 0;
//         page_table[i].page_number = -1;
//         page_table[i].last_used = -1;
//     }

//     int time = 0, page_faults = 0;

//     for (int i = 0; i < REFERENCE_LEN; i++) {
//         int page = reference_string[i];
//         int index = findPage(page_table, page);

//         if (index != -1) {
//             page_table[index].last_used = time;
//         } else {
//             page_faults++;
//             int free_index = findFreeFrame(page_table);
//             if (free_index != -1) {
//                 page_table[free_index].page_number = page;
//                 page_table[free_index].valid = 1;
//                 page_table[free_index].last_used = time;
//             } else {
//                 int lru_index = findLRU(page_table);
//                 page_table[lru_index].page_number = page;
//                 page_table[lru_index].last_used = time;
//             }
//         }
//         time++;
//     }

//     printPageTable(page_table);
//     printf("Total Page Faults: %d\n", page_faults);

//     return 0;
// }

