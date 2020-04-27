/*
 * COMP3500 Project 6: A Virtual Memory Manager
 * Author: Isaiah Roh
 * Contributors: Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Dec. 11, 2019. Version 1.1
 *
 * Souce Code: address.h
 * This is a header file of address.c
 */
#ifndef ADDRESS_H
#define ADDRESS_H
#include "system.h"
#include <stdio.h>
#include <stdlib.h>
/* offset is used to address the location within a page */
#define OFFSET_BITS 8

/* mask is used to extract the offset from a logical address */
#define OFFSET_MASK 0xFF

/* There is no byte data type in C */
typedef unsigned char byte;


typedef unsigned int page_t;
typedef unsigned int frame_t;
typedef unsigned int offset_t;
typedef unsigned int laddress_t;
typedef unsigned int paddress_t;
typedef unsigned char byte_t;
typedef laddress_t laddress_list_t[LOGIC_ADDR_LIST_SIZE];
typedef paddress_t paddress_list_t[LOGIC_ADDR_LIST_SIZE];
typedef frame_t page_table_t[NUM_PAGES];
typedef struct {
    byte_t block[PAGE_SIZE];
    bool isValid;
} frame_entry_t;
typedef frame_entry_t physical_memory_t[NUM_FRAMES];
void extract_logic_address(laddress_t logic_addr, page_t *pagenum, offset_t *offset);
void form_physical_address(frame_t frame_num, offset_t offset, paddress_t *physical_address);
byte_t read_physical_memory(paddress_t physical_addr, frame_entry_t physical_memory[NUM_FRAMES]);
void store_address_value_pair(laddress_t logic_addr, paddress_t physical_addr, byte_t one_byte, char *output);

char *itob(int x);
char *itob16(int x);
char *itob8(int x);
#endif
