/*
 * COMP3500 Project 6: A Virtual Memory Manager
 * Author: Isaiah Roh
 * Contributors: Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Dec. 11, 2019. Version 1.1
 *
 * Souce Code: vm_sim.c
 * This program demonstrates how to read and print bytes in the backing store.
 *
 * Compile: make
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "tlb.h"
#define DEBUG

/* There is no byte data type in C */
typedef unsigned char byte_t;
typedef unsigned int uint_t;
typedef unsigned char byte;

void welcome_message(void);
//void output_list(FILE *fp,laddress_list_t *logicAddrList, paddress_list_t *physAddrList, byte_t *values, int count);
u_int_t load_logic_address(char *filename, laddress_list_t laddress_list);
int displayAddresses(bool displayAddrChoice, int count, laddress_list_t logicAddrList, paddress_list_t physAddrList, byte *valueList);



int main ( int argc, char *argv[] ) {
    /* argc should be 2 for correct execution */
    if (argc != 2) {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s, <input logic address file>\n", argv[0]);
        return 0;
    }
    welcome_message();
    FILE* fp_output = fopen("vm_sim_output.txt", "w");
    if(fp_output == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
    //Variables
    tlb_t my_tlb;
    page_t page_num;
    page_table_t page_table;
    frame_t frame_num;
    offset_t offset;
    laddress_t logic_address;
    paddress_t physical_address;
    byte_t value;
    physical_memory_t physical_memory;
    bool isTlbHit;
    bool isPageFault;
    u_int_t lookups = 0;
    u_int_t tlb_hits = 0;
    u_int_t page_faults = 0;
    u_int_t count;
    laddress_list_t laddress_list;
    paddress_list_t paddress_list;
    byte_t value_list[LOGIC_ADDR_LIST_SIZE];

    bool isDisplaying;
    printf("Display physical addresses? [y or n] \n");
    char displayAddrChoice;
    scanf("%c", &displayAddrChoice);
    getchar();
    if (displayAddrChoice == 'y') {
        isDisplaying = TRUE;
    }
    else {
        isDisplaying = FALSE;
    }

    bool isLRU;
    printf("Which replacement method? 1:LRU, 2: FIFO [1 or 2] \n");
    char methodchoice;
    scanf("%c", &methodchoice);
    
    if(methodchoice == '1') {
        isLRU = TRUE;
    }
    else {
        isLRU = FALSE;
    }
    //Initialization
    tlb_init(&my_tlb);
    init_page_table(page_table);
    init_physical_address(physical_memory);

    //load_logic_addr
    count = load_logic_address(argv[1], laddress_list); 

    //virtual memory manager
    int i;
    for(i = 0; i < count; i++) {
        char output[CHAR_MAX];
        extract_logic_address(laddress_list[i], &page_num, &offset);
        search_tlb(page_num, &my_tlb, &isTlbHit, &frame_num, i);
        if(isTlbHit) {
            tlb_hits++;
            form_physical_address(frame_num, offset, &physical_address);
        }
        else {
            search_page_table(page_num, &isPageFault, &frame_num, page_table);
            if(isPageFault) {
                page_faults++;
                handle_page_fault(page_num, &my_tlb, physical_memory, page_table, &frame_num);
                form_physical_address(frame_num, offset, &physical_address);
                if(isLRU) {
                    TLB_replacement_LRU(page_num, frame_num, &my_tlb, i);
                }
                else {
                    TLB_replacement_FIFO(page_num, frame_num, &my_tlb);
                }
            }
            else {
                form_physical_address(frame_num, offset, &physical_address);
                if(isLRU) {
                    TLB_replacement_LRU(page_num, frame_num, &my_tlb, i);
                }
                else {
                    TLB_replacement_FIFO(page_num, frame_num, &my_tlb);
                }
            }
        }
        value = read_physical_memory(physical_address, physical_memory);
        store_address_value_pair(laddress_list[i], physical_address, value, output);
        fputs(output, fp_output);
        paddress_list[i] = physical_address;
        value_list[i] = value;
        isTlbHit = FALSE; //reset
        isPageFault = FALSE; //reset
    }

    //display output
    displayAddresses(isDisplaying, count, laddress_list, paddress_list, value_list);
    double pageFaultPercent = ((double)page_faults / count) * 100;
    double tlbHitPercent = ((double)tlb_hits / count) * 100;
    
    printf("\nPage Fault Rate: %f %%", pageFaultPercent);
    printf("\nTLB Hit Rate: %f %%\n\n", tlbHitPercent);
    printf("Check the results in the outputfile: vm_sim_output.txt\n\n");
    close(fp_output);
    return 0;
}



void welcome_message(void) {
    printf("Welcome to Isaiah Roh's VM Simulator\n");
    printf("Number of logical pages: %d\n", NUM_PAGES);
    printf("Number of physical frames: %d\n", NUM_FRAMES);
    printf("Page size: %d\n", PAGE_SIZE);
    printf("TLB size: %d\n", TLB_SIZE);
}

int output_all_lists(laddress_list_t logicAddrList, paddress_list_t physAddrList, byte_t *valueList, int count) {
    void;
}

int displayAddresses(bool displayAddrChoice, int count, laddress_list_t logicAddrList, paddress_list_t physAddrList, byte *valueList) {
    if (displayAddrChoice == TRUE) {
        int i;
        for (i = 0; i < count; i++) {
            printf("Virtual Address: %u; Physical Address: %u; Value: %u \n", logicAddrList[i], physAddrList[i], valueList[i]);
        }
    }
    
    return 0;
}



u_int_t load_logic_address(char *filename, laddress_list_t laddress_list) {
    FILE *fp;
    int line_length;
    u_int_t count = 0;
    size_t buf_length = 0; 
    char* one_byte = NULL; 
    fp= fopen(filename, "r");
    //fopen returns NULL pointer on failure
    if(fp == 0) {
      printf("Could not open file: %s.\n", filename);
       return NULL;
    }
    int counter = 0;
    while((line_length = getline(&one_byte,&buf_length, fp))!= -1) {
        int bit_address = atoi(one_byte);
        laddress_list[count] = bit_address;
        count++;
    }
    fclose(fp); 
    return count;

}
