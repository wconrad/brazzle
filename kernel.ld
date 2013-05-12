OUTPUT_FORMAT("binary")
SECTIONS
{
  . = 0x3000;  /* Must match kernel_addr */
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
