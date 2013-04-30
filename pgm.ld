OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
  . = 0x1000;
  .text : {
    *(.text)
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
