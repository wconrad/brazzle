OUTPUT_FORMAT("binary")
SECTIONS
{
  . = 0x2000;  /* Must match loader_addr */
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
