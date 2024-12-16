# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#include "demo_system_regs.h"

.section .text


reset_handler:

  /* set all registers to zero */
  mv  x1, x0
  mv  x2, x1
  mv  x3, x1
  mv  x4, x1
  mv  x5, x1
  mv  x6, x1
  mv  x7, x1
  mv  x8, x1
  mv  x9, x1
  mv x10, x1
  mv x11, x1
  mv x12, x1
  mv x13, x1
  mv x14, x1
  mv x15, x1
  mv x16, x1
  mv x17, x1
  mv x18, x1
  mv x19, x1
  mv x20, x1
  mv x21, x1
  mv x22, x1
  mv x23, x1
  mv x24, x1
  mv x25, x1
  mv x26, x1
  mv x27, x1
  mv x28, x1
  mv x29, x1
  mv x30, x1
  mv x31, x1

  /* stack initilization */
  la   sp, _stack_start



#abilitare interruzoni

/* Imposta mtvec per il vectored mode */

la x5, 0x0           # Carica l'indirizzo di base della vector table
li x6, 1             # Setta la modalità Vectored
or x5, x5, x6        # Combina l'indirizzo base con la modalità
csrw mtvec, x5       # Scrive il valore combinato in mtvec

csrs mstatus, 0x8    # Abilito MIE nel mstatus

# Set dei bit  MEI (External Interrupt), MSI (Software Interrupt) e MTI (Timer Interrupt) del registro MIE
li x6, 0x0888
csrs mie, x6





_start:
  .global _start

  /* clear BSS */
  la x26, _bss_start
  la x27, _bss_end

  bge x26, x27, zero_loop_end

zero_loop:
  sw x0, 0(x26)
  addi x26, x26, 4
  ble x26, x27, zero_loop
zero_loop_end:

/* jump to main program entry point (argc = argv = 0) */
  addi x10, x0, 0
  addi x11, x0, 0

  jal x1, main

tim_handler:
  la x5, 0x20000
  li x6, 0x1
  sw x6, 0(x5)
  mret

sw_handler:
  la x5, 0x20000
  li x6, 0x2
  sw x6, 0(x5)
  mret

ext_handler:
    # Carica l'indirizzo base del PLIC ed il claim/complete register
    li x5, 0x4000000          # x5 = indirizzo base PLIC
    li x7, 0x200004           # Carica 0x200004 in x7
    add x5, x5, x7            # Somma x5 e x7 per ottenere l'indirizzo del claim/complete register (0x400000 + 0x200004)

    # Leggi l'ID della sorgente di interrupt
    lw x6, 0(x5)               # x6 = ID dell'interrupt

    # Controlla se l'ID è 1 (GPIO)
    li x7, 1                  # x7 = 1 (ID per GPIO)
    beq x6, x7, handle_gpio   # Se x6 == 1, vai a handle_gpio

    # Controlla se l'ID è 2 (Timer)
    li x7, 2                  # x7 = 2 (ID per Timer)
    beq x6, x7, handle_timer  # Se x6 == 2, vai a handle_timer

    # Completa l'interrupt e ritorna
complete_interrupt:
    sw x6, 0(x5)             # Scrivi l'ID per completare l'interrupt
    mret                     # Ritorna dall'interrupt

handle_gpio:
    # Accendi il primo LED )
    li x8, 0x20000            # x8 = indirizzo base GPIO_out (0x20000)
    li x9, 1                  # x9 = valore per accendere il LED (bit 0)
    lw x10, 0(x8)             # Carica lo stato dei LED in x10
    # Usa un registro temporaneo per invertire il bit
    li x11, 1                  # x11 = 1 (valore per inverte bit 0)
    xor x10, x10, x11          # Inverte il bit 0 (LED 1) -> Accende se spento e viceversa
    sw x10, 0(x8)              # Scrivi lo stato modificato dei LED

    # Reset dell'interrupt GPIO
    li x8, 0x30000            # x8 = indirizzo base GPIO_in (0x30000)
    li x7, 0x120              # Carica 0x120 in x7
    add x8, x8, x7            # Somma x8 e x7 per ottenere l'indirizzo dell'IP Interrupt Status Register (0x20000 + 0x120)
    li x9, 1                  # x9 = valore per resettare bit 0 (1)
    sw x9, 0(x8)              # Scrivi sul registro IP per resettare l'interrupt

    j complete_interrupt      # Vai a completare l'interrupt

handle_timer:
   
  # Reset dell'interrupt Timer (scrivi 1 sul bit T0INT nel TCSR0)
    li x12, 0x40000             # x12 = indirizzo base Timer
    li x9, 0x100               # x9 = valore per resettare bit 8 (1 << 8)
    sw x9, 0(x12)               # Scrivi al registro TCSR0 per resettare l'interrupt

  # Toggle del LED del timer
   li x8, 0x20000             # x8 = indirizzo base GPIO_out (0x20000)
   li x9, 2                   # x9 = 2 (LED 2)
   lw x10, 0(x8)              # Carica lo stato attuale dei LED in x10
   xor x10, x10, x9           # Inverte il bit associato al LED 2 (acceso/spento)
   sw x10, 0(x8)              # Scrivi lo stato invertito al registro GPIO output

   li x9, 0xD2                #ripristino lo status register alterato dalla reset di T0INT
   sw x9, 0(x12)

   j complete_interrupt       # Vai a completare l'interrupt


default_exc_handler:
  la x5, 0x20000
  li x6, 0xff
  sw x6, 0(x5)
  j default_exc_handler

/* =================================================== [ exceptions ] === */
/* This section has to be down here, since we have to disable rvc for it  */

  .section .isr_vector, "ax"
  .option norvc;

  /* All unimplemented interrupts/exceptions go to the default_exc_handler. */
  # Two reset handler jumps to work both in simulation and in syntesis. However the first one is not required in synthesis and can be replaced
  
  .org 0x00
  jal x0, reset_handler      

  .rept 2
    jal x0, default_exc_handler  
  .endr

  jal x0, sw_handler      

  .rept 3
    jal x0, default_exc_handler  
  .endr

  jal x0, tim_handler        

  .rept 3
    jal x0, default_exc_handler  
  .endr

  jal x0, ext_handler      

  .rept 20
    jal x0, default_exc_handler  
  .endr
