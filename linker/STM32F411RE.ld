/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

MEMORY
{
  FLASH_HDR (rx) : ORIGIN = 0x08010000, LENGTH = 1K
  FLASH (rx) : ORIGIN = 0x08010400, LENGTH = 447K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Enforce emmition of the vector table. */
EXTERN (vector_table)

/* Define the entry point of the output file. */
ENTRY(reset_handler)

/* Define sections. */
SECTIONS
{
  .text (READONLY) : {
    *(.vectors)	/* Vector table */
    *(.text*)	/* Program code */
    . = ALIGN(4);
    *(.rodata*)	/* Read-only data */
    . = ALIGN(4);
  } > FLASH

  /* C++ Static constructors/destructors, also used for __attribute__
   * ((constructor)) and the likes */
  .preinit_array (READONLY) : {
    . = ALIGN(4);
    __preinit_array_start = .;
    KEEP (*(.preinit_array))
    __preinit_array_end = .;
  } > FLASH
  .init_array (READONLY) : {
    . = ALIGN(4);
    __init_array_start = .;
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    __init_array_end = .;
  } > FLASH
  .fini_array (READONLY) : {
    . = ALIGN(4);
    __fini_array_start = .;
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    __fini_array_end = .;
  } > FLASH

  /*
   * Another section used by C++ stuff, appears when using newlib with
   * 64bit (long long) printf support
   */
  .ARM.extab (READONLY) : {
    *(.ARM.extab*)
  } > FLASH
  .ARM.exidx (READONLY) : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > FLASH

  . = ALIGN(4);
  _etext = .;

  /* RAM, but not cleared on reset, eg boot/app comms */
  .noinit (NOLOAD) : {
    *(.noinit*)
  } > RAM
  . = ALIGN(4);

  .data : {
    _data = .;
    *(.data*)	/* Read-write initialized data */
    *(.ramtext*)    /* "text" functions to run in RAM */
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH
  _data_loadaddr = LOADADDR(.data);

  .bss : {
    *(.bss*)	/* Read-write zero initialized data */
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  /*
   * Put one byte 0xFF on the end of address for gap fill for cybersecurity
   */
  .flash_end ORIGIN(FLASH) + LENGTH(FLASH) - 1 :
  {
    BYTE(0xFF);
  } > FLASH

  /* Fill with 0xFF for header (0x08010000 - 0x08010400) */
  .header_gap :
  {
    FILL(0xFF);
    BYTE(0xFF);
    . = . + 0x3FF;
  } > FLASH_HDR

  /*
   * The .eh_frame section appears to be used for C++ exception handling.
   * You may need to fix this if you're using C++.
   */
  /DISCARD/ : {
    *(.eh_frame)
  }

  . = ALIGN(4);
  end = .;
}

PROVIDE(_stack = ORIGIN(RAM) + LENGTH(RAM));
