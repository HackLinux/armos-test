ENTRY (_start)

SECTIONS
{
    . = 0x82000000;
    .text : { *(.text*) *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON*) }
}
