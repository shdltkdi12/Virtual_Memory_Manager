# Virtual Memory Manager
A simple virtual memory manager that is responsible to 
(1) load a file containing a list of logical addresses, 
(2) translate logical addresses into physical addresses for a virtual address space of size 2^16 bytes
(3) output the value of the byte stored at the translated physical address.
This program uses a TLB, which is a table of 16 entries that acts like a cache for recently used physical addresses in the page table.
Whenever a TLB fault happens, the program will continue its search with the page table.
The program outputs information about TLB hits, TLB faults, page table hits, page table faults, along with the inputs and output in a file.
Whenever a page fault happens, the program will obtain a new frame from a BACKING_STORE which is a simulation of a real hard drive.
It will read byte by byte to form a new frame.
Dependecies:
gcc, bash
Make:
In the directory, run $make 
How to run:
run $./vm_sim <name_of_input_file>
For more options or usage, run $./vm_sim
