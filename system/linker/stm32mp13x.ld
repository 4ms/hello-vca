OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_Reset)

INCLUDE system/linker/memory.ld


MEMORY
{
    CODE (rx)			: ORIGIN = A7_CODE_,		LENGTH = A7_CODE_SZ_
    RAM (rx)			: ORIGIN = A7_RAM_,			LENGTH = A7_RAM_SZ_
	HEAP (rw)			: ORIGIN = A7_HEAP_,		LENGTH = A7_HEAP_SZ_
	DMABUF (rw)			: ORIGIN = DMABUF_,			LENGTH = DMABUF_SZ_ 
}

/* A7 stacks are at end of RAM */

SECTIONS
{
    .text : {
        __TEXT_START__ = .;
        *(.vector_table)
		*(.resethandler)
		*(.irqhandler)
        *(.text)
        *(.text*)

		*(.glue_7)         /* glue arm to thumb code */
		*(.glue_7t)        /* glue thumb to arm code */
		*(.eh_frame)
		KEEP (*(.init))
		KEEP (*(.fini))

        . = ALIGN(8);
        __TEXT_END__ = .;
     } > CODE

	.rodata :
	{
		/*. = ALIGN(4096);*/
        __RO_START__ = .;
		*(.rodata)         /* .rodata sections (constants, strings, etc.) */
		*(.rodata*)
		*(.binary*)
		/*. = ALIGN(4096);*/
        __RO_END__ = .;
	} > CODE

	.ARM.extab	 : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >CODE
	.ARM : { __exidx_start = .; *(.ARM.exidx*) __exidx_end = .; } >CODE

	.preinit_array :
	{
		. = ALIGN(8);
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		. = ALIGN(8);
	} > CODE

	.init_array :
	{
		. = ALIGN(8);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
		. = ALIGN(8);
	} > CODE

	.fini_array :
	{
		. = ALIGN(8);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
		. = ALIGN(8);
	} > CODE

    .data : 
    {
        . = ALIGN(8);
        _data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(8);
        _data_end = .;
    } > CODE

    .bss : {
        _bss_start = .;
        *(.bss)
        *(.bss*)   /* required for some malloc calls */
		*(COMMON)  /* required for libc, such as __lock___atexit_recursive_mutex */
        . = ALIGN(8);
        _bss_end = .;
		. = ALIGN(256);
		_ram_aligned_end = .;
    } > RAM

	.heap (NOLOAD):
	{
		. = ALIGN(8);
		_sheap = .;
		. += A7_HEAP_SZ_;
		_eheap = .;
	} > HEAP

	.noncachable (NOLOAD):
	{
		. = ALIGN(8);
        *(.noncachable)
	} > DMABUF

    _user_stack_start = _ram_aligned_end;
    _user_stack_end = _user_stack_start + 0x10000;  /* 64kB */

    _svc_stack_start = _user_stack_end;
    _svc_stack_end = _svc_stack_start + 0x8; 

    _irq_stack_start = _svc_stack_end;
    _irq_stack_end = _irq_stack_start + 0x8; 

    _fiq_stack_start = _irq_stack_end;
    _fiq_stack_end = _fiq_stack_start + 0x8;

  /* Remove information from the compiler libraries */
  /* /DISCARD/ : */
  /* { */
  /*   libc.a ( * ) */
  /*   libm.a ( * ) */
  /*   libgcc.a ( * ) */
  /* } */

	.ARM.attributes 0 : { *(.ARM.attributes) }
}
