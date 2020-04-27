/*
 * COMP3500 Project 6: A Virtual Memory Manager
 * Isaiah Roh
 * Contributors: Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Dec. 11, 2019. Version 1.1
 *
 * Souce Code: address.c
 * This program demonstrates:
 * (1) how to derive page number and offset from a logical address
 * (2) how to create a physical address using frame number and offset
 *
 * Compile: make
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "address.h"
//#define DEBUG


 
void extract_logic_address(laddress_t logic_addr, page_t *pagenum, offset_t *offset) {
    *pagenum = logic_addr >> OFFSET_BITS;
    *offset = logic_addr & OFFSET_MASK;
}
byte_t read_physical_memory(paddress_t physical_addr, frame_entry_t physical_memory[NUM_FRAMES]) {
    byte_t output;
    offset_t offset = physical_addr & OFFSET_MASK;
    frame_t cur_frame = physical_addr >> OFFSET_BITS;
    output = physical_memory[cur_frame].block[offset];
    return output;
}
void store_address_value_pair(laddress_t logic_addr, paddress_t physical_addr, byte_t one_byte, char *output) {
    snprintf(output, CHAR_MAX, "Virtual Address: %u; Physical Address: %u; Value: %u \n", logic_addr, physical_addr, one_byte);
    return output;
}
void form_physical_address(frame_t frame_num, offset_t offset, paddress_t *physical_address) {
    *physical_address = frame_num *FRAME_SIZE + offset;
}


/*
 * convert machine number to human-readable binary string.
 * Returns: pointer to static string overwritten with each call.
 * Reference:
 * https://bytes.com/topic/c/answers/621985-print-binary-representation
 * */
char *itob(int x)
{
    static char buff[sizeof(int) * CHAR_BIT + 1];
    int i;
    int j = sizeof(int) * CHAR_BIT - 1;

    buff[j] = 0;
    for(i=0;i<sizeof(int) * CHAR_BIT; i++) {
        if(x & (1 << i))
            buff[j] = '1';
        else
            buff[j] = '0';
        j--;
    }
    return buff;
}

/*
 * Xiao Qin:
 * This is a mofified version of the above itob()
 * Only print 16 bits
 */
char *itob16(int x)
{
    static char buff[17];
    int i;
    int j = 15;

    buff[j] = 0;
    for(i=0;i<16; i++) {
        if(x & (1 << i))
            buff[j] = '1';
        else
            buff[j] = '0';
        j--;
    }
    return buff;
}

/*
 * Xiao Qin:
 * This is a mofified version of the above itob()
 * Only print 8 bits
 */
char *itob8(int x)
{
    static char buff[9];
    int i;
    int j = 7;

    buff[j] = 0;
    for(i=0;i<8; i++) {
        if(x & (1 << i))
            buff[j] = '1';
        else
            buff[j] = '0';
        j--;
    }
    return buff;
}
