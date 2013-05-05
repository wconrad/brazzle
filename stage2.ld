OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
  . = 0x2000;  /* Must match stage2_addr */
  .text : {
    *(.text)
    *(.rodata*)
  }
  .data : {
    *(.data)
  }
  .bss : {
    *(.bss)
  }
  /DISCARD/ : {
    *(.comment)
    *(.eh_frame)
  }
}
