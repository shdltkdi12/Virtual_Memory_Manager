/*
 * COMP3500 Proje/ct 6: A Virtual Memory Manager
 * Isaiah Roh
 * Contributors: Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Dec. 11, 2019. Version 1.1

 * Souce Code: tlb.c
 * This program demonstrates the usage of the TLB data structure
 *
 * Compile: make
 */
#include <stdio.h>
#include <limits.h>
#include "tlb.h"
//#define DEBUG
const char backing_store[] = "BACKING_STORE";
int tlb_init(tlb_t *tlb) {
    u_int_t i;
    for (i = 0; i < TLB_SIZE; i++) {
        tlb->tlb_entry[i].isValid = FALSE;
    }
    tlb->next_tlb_ptr = 0;
    tlb->tlb_entry[i].last_referenced = 0;
    return 0; /* success */
}
void tlb_entry_init(tlb_entry_t *tlb_entry, page_t page_num, frame_t frame_num) {
	tlb_entry->page_num = page_num;
	tlb_entry->frame_num = frame_num;
	tlb_entry->isValid = TRUE;
}
void init_physical_address(physical_memory_t physical_memory) {
    int i;
    for(i = 0; i < NUM_FRAMES; i++) {
        physical_memory[i].isValid = FALSE;
    }
}
void init_page_table(page_table_t pagetable) {
    int i;
    for(i=0; i< NUM_PAGES; i++) {
        pagetable[i] = EMPTY_PAGE;
    }
}

int TLB_replacement_FIFO(page_t page_num, frame_t frame_num, tlb_t* tlb) {
    int i;
    int max = -1;

    //TLB is not full, if full get the oldest age
    for(i = 0; i < TLB_SIZE; i++) {
        if(tlb->tlb_entry[i].isValid == FALSE) {
            tlb->tlb_entry[i].page_num = page_num;
            tlb->tlb_entry[i].frame_num = frame_num;
            tlb->tlb_entry[i].isValid = TRUE;
            return 0;
        }
    }
    //replace that which has the oldest age
    tlb->tlb_entry[tlb->next_tlb_ptr].page_num = page_num;
    tlb->tlb_entry[tlb->next_tlb_ptr].frame_num = frame_num;
    tlb->tlb_entry[tlb->next_tlb_ptr].isValid = TRUE;
    if(tlb->next_tlb_ptr == TLB_SIZE - 1) {
        tlb->next_tlb_ptr = 0;
    }
    else {
        tlb->next_tlb_ptr++;
    }
    return 0;
}
int TLB_replacement_LRU(page_t page_num, frame_t frame_num, tlb_t* tlb, u_int_t curr_time) {
    int i;
    int min = INT_MAX;
    //TLB is not full, if full get the least recently used in time units
    for(i = 0; i < TLB_SIZE; i++) {
        if(tlb->tlb_entry[i].isValid == FALSE) {
            tlb->tlb_entry[i].page_num = page_num;
            tlb->tlb_entry[i].frame_num = frame_num;
            tlb->tlb_entry[i].isValid = TRUE;
            tlb->tlb_entry[i].last_referenced = curr_time;
            return 0;
        }
        else {
            if(min > tlb->tlb_entry[i].last_referenced) {
                min = tlb->tlb_entry[i].last_referenced;
            }
        }
    }
    for(i=0; i<TLB_SIZE; i++) {
        if(tlb->tlb_entry[i].last_referenced == min) {
            tlb->tlb_entry[i].page_num = page_num;
            tlb->tlb_entry[i].frame_num = frame_num;
            tlb->tlb_entry[i].last_referenced = curr_time;
            return 0;
        }
    }
    

}
int tlb_display(tlb_t tlb) {
    u_int_t i;

    for (i = 0; i < TLB_SIZE; i++) {
        printf("TLB entry %d, page num: %d, frame num: %d, ", i, tlb.tlb_entry[i].page_num, tlb.tlb_entry[i].frame_num);
        if (tlb.tlb_entry[i].isValid == FALSE)
            printf("Invalid\n");
        else printf("Valid\n");
    }

    return 0;
}

/*
 * Traverses through whole tlb, searches for the target and updates the ones that are not the target
 */
void search_tlb(page_t pagenum, tlb_t *tlb, bool *is_tlb_hit, frame_t *frame_num, u_int_t curr_time) {
    
    u_int_t i;
    for(i=0; i< TLB_SIZE; i++) {
        if(tlb->tlb_entry[i].isValid) {
            if(tlb->tlb_entry[i].page_num == pagenum) {
                *is_tlb_hit = TRUE;
                *frame_num = tlb->tlb_entry[i].frame_num;
                tlb->tlb_entry[i].last_referenced = curr_time;
            }
        }
    }
    return;
    
}
void search_page_table(page_t pageNum, bool * isPageFault, frame_t * frameNum, page_table_t page_Table) {
    if (page_Table[pageNum] == EMPTY_PAGE) {
        *isPageFault = TRUE;
    }
    else {
        *frameNum = page_Table[pageNum];
    }
}
void handle_page_fault(page_t page_num, tlb_t *tlb, physical_memory_t physical_memory, \
            page_table_t page_table,frame_t *frame_num) {
    load_frame_from_backing_store(page_num, physical_memory, frame_num);
    page_table[page_num] = *frame_num;
    
}

void load_frame_from_backing_store(page_t page_num, physical_memory_t physical_memory, frame_t *frame_num) {
    FILE *fp = fopen(backing_store, "r");
    fpos_t pos;
    byte one_byte;
    if (fp == 0) {
        printf("Could not open file: %s.\n", backing_store);
    }

    else {
        
        fseek(fp, page_num * FRAME_SIZE, SEEK_SET); //seek_set = beginning of file. second argument indicates number of bytes to offset from seek_set
        frame_t cur_frame;
        for(cur_frame = 0; cur_frame < 256; cur_frame++) {
            if(physical_memory[cur_frame].isValid == FALSE) {
                physical_memory[cur_frame].isValid = TRUE;
                break;
            }
        }
        int i;
        for(i = 0; i < PAGE_SIZE; i++) {
            fread(&one_byte, 1, 1, fp);
            physical_memory[cur_frame].block[i] = one_byte;
        }
        fclose(fp);
        *frame_num = cur_frame;
    }
}

