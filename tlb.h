/*
 * COMP3500 Project 6: A Virtual Memory Manager
 * Author: Isaiah Roh
 * Contributors: Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Dec. 11, 2019. Version 1.1

 * Souce Code: tlb.h
 * This is a sample header file for TLB 
 */
#ifndef TLB_H
#define TLB_H
#include "address.h"

typedef struct {
    page_t  page_num;
    frame_t frame_num;
    bool    isValid;      /* indicate if this entry is valid */
    u_int_t last_referenced;
}tlb_entry_t;

typedef struct {
    tlb_entry_t tlb_entry[TLB_SIZE];
    u_int_t       next_tlb_ptr;  /* next candidate entry to be replaced/used */
}tlb_t;

int TLB_init(tlb_t* tlb);
void tlb_entry_init(tlb_entry_t *tlb_entry, page_t pagae_num, frame_t frame_num);
void init_physical_address(physical_memory_t physical_memory);
void init_page_table(page_table_t pagetable);
int TLB_replacement_FIFO(page_t page_num, frame_t frame_num, tlb_t* tlb);
int TLB_replacement_LRU(page_t page_num, frame_t frame_num, tlb_t* tlb, u_int_t curr_time);
int TLB_display(tlb_t tlb);
void search_tlb(page_t pagenum, tlb_t *tlb, bool *is_tlb_hit, frame_t *frame_num, u_int_t curr_time);
void search_page_table(page_t pageNum, bool * isPageFault, frame_t * frameNum, page_table_t page_Table);
void handle_page_fault( page_t page_num, tlb_t *tlb, physical_memory_t physical_memory, page_table_t page_table,frame_t *frame_num);
void load_frame_from_backing_store(page_t page_num, physical_memory_t physical_memory, frame_t *frame_num);
#endif

