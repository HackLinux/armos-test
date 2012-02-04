.extern swi_handler
.extern prefetch_abort_handler

interrupt_vector_table:
    b . @ Reset
    b . @ undefined instruction 
    b . @ swi_handler @ SWI instruction
    b . @prefetch_abort_handler @ prefetch abort
    b . @ data abort
    b . @ unused (was address exception on 26-bit arm)
    b . @ irq
    b . @ fiq

.comm stack, 0x10000 @ Reserve 64k stack in the BSS
_start:
    .globl _start
    ldr sp, =stack+0x10000 @ Set up the stack
    bl main @ Jump to the main function
1: 
    b 1b @ Halt
