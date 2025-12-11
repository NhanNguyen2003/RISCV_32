OUTPUT_ARCH( "riscv" )
ENTRY( _start )
SECTIONS
{
  . = 0x00010000;
  .text : { *(.text) }
  . = ALIGN(0x1000);
  .rodata : { *(.rodata) }
  . = ALIGN(0x1000);
  .data : { *(.data) }
  .bss : { *(.bss) }
}
