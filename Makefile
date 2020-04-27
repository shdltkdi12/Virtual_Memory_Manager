#*Name: Makefile
#*About this file: Build Script

CFLAGS = -fno-stack-protector -Wno-error=format-truncation -O2

all: vm_sim 
vm_sim: vm_sim.o tlb.o address.o
	gcc -g -o vm_sim tlb.o address.o vm_sim.c ${CLAGS}
vm_sim.o:tlb.h  vm_sim.c
	gcc -g -c -I~/Comp3500/project6/src vm_sim.c ${CFLAGS}
tlb.o: tlb.c tlb.h address.h
	gcc -g -c -I~/Comp3500/project6/src tlb.c ${CFLAGS}
address.o: address.h system.h address.c
	gcc -g  -c -I~/Comp3500/project6/src address.c ${CFLAGS}
clean:
	rm *.o vm_sim 
