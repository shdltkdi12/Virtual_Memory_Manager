/*
 * COMP3500 Project 6: A Virtual Memory Manager
 * Author: Isaiah Roh
 * Contributors: Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Dec. 11, 2019. Version 1.1
 *
 * Souce Code: system.h
 * This program demonstrates how to read and print bytes in the backing store.

 * Sample header file for the virtual memory manager
 */
#ifndef SYSTEM_H
#define SYSTEM_H

#define NUM_PAGES   256	/* total number of pages in virtual memory space */
#define NUM_FRAMES  256	/* total number of frames in physical memory space */

#define EMPTY_PAGE 3435973836

#define PAGE_SIZE   256	/* number of bytes in a page or frame */
#define FRAME_SIZE 256
#define TLB_SIZE    16	/* number of entries in the TLB */

#define OFFSET_BITS 8	/* number of bits required to address a byte in a page */
#define LOGIC_ADDR_LIST_SIZE 1024
#define PAGE_OFFSET_MASK 0xFF /* mask to extract the offset from a logical address */

const char backing_store[];	/* a simulated hard disk */

typedef enum{FALSE, TRUE} bool;
typedef unsigned int u_int_t;

#endif
