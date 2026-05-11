.section .text
.equ GPIOA,  0x40010800
.equ GPIOB,  0x40010C00
.equ GPIO_PP, 0x0           // Push-Pull (för LED)
.equ GPIO_50, 0x3           // Max hastighet
.equ GPIO_UD, 0x8           // Input med Pull-up/down (bra för switchar)
.equ PAEN,   0x4            // Clock enable för Port A
.equ PBEN,   0x8            // Clock enable för Port B
.equ RCU,    0x40021000
.equ APB2EN, 0x18
.equ CTR0,   0x00
.equ ISTAT,  0x08
.equ BOP,    0x10


.global switch_hw_init 
switch_hw_init:
    addi sp, sp, -4
    sw ra, 0(sp)

    // 1. Starta klockan för Port A och B
    li a0, PAEN+PBEN
    call rcu2en

    // 2. Konfigurera GPIOA Pin 5 som Input (Switch)
    li a0, GPIOA
    li a1, GPIO_UD          // Configuration bits
    li a2, 0x0              // Mode: Input
    li a3, 5               // Pin 5
    call gpioi

    // 3. Konfigurera GPIOB Pin 0 som Output (LED)
    li a0, GPIOB //ändrat A-->B
    li a1, GPIO_PP //push pull 
    li a2, GPIO_50 //  50MHz
    li a3, 1        // Pin B1 lyser 
    call gpioi

    lw ra, 0(sp)
    addi sp, sp, 4
    ret

// --- LÄS SWITCH ---
.global get_switch_status
get_switch_status:
    addi sp, sp, -4
    sw ra, 0(sp)

    li a0, GPIOA
    call gpiois             // Läser ISTAT till a1 (enligt din main-2.S)
    
    srli a1, a1, 5          // Flytta pin 5 till position 0
    andi a0, a1, 1          // Maskera ut allt utom sista biten, spara i a0 (returvärde)

    lw ra, 0(sp)
    addi sp, sp, 4
    ret
.global set_led_status
set_led_status:
  .global set_led_status
set_led_status:
    // a0 innehåller 'state' (1 eller 0)
    slli a1, a0, 1          // SKIFTA: Flytta 1:an från bit 0 till bit 1
    li a0, GPIOB            // B1 lyser 
    li a2, 2                // Mask för pinne 1 (bit 1 satt)
    j gpiobo               // Använd din befintliga Bit Operation-rutin

rcu2en: li t0,RCU           // t0 points to RCU...
    lw t1,APB2EN(t0)    // ...retrieve current configuration...
    or t1,t1,a0         // ...and update the configuration...
    sw t1,APB2EN(t0)    // ...then save back the updated config...
    ret 
gpioi:  
    mv t0, a0           // t0 points to GPIOX...
    mv t4, a3           //    t4 marks selected pin...
    li t1,0x8           //    Use port control register 0 or 1,
    blt t4,t1, 1f       //    thus is pin > 7?
    addi t0, t0, 4      //       Yes, adjust base address,
    addi t4,t4,-8       //            and adjust selected pin.
1:      
    slli t4,t4,2        //    Multiply pin nb by 4 to get steps to shift!
    li t1, 0xF          //    Create inverted configuration mask...
    sll t1, t1, t4      //    ...and move it into right position.
    not t1, t1          //    ...invert it back, then...
    mv t2, a1           //    Create configuration bits...
    or t2, t2, a2       //    
    sll t2, t2, t4      //    ...and move it into right position.

    lw t3,CTR0(t0)      //    retreive port control register 0(/1)...
    and t3,t3,t1        //    ...clear out selected pin's config/mode bits...
    or t3, t3, t2       //    ...configure new config/mode bits...
    sw t3,CTR0(t0)      //    ...and write it back.
    ret  
  
gpiois: 
    lw a1, ISTAT(a0)    // Execute request (return value ought to be in a0)...
    ret  
gpiobo: 
    and a1, a1, a2      // (Clear all pattern bits that are not target bits)
    xor t0, a2, a1      // Find zeros in targeted bits...
    slli t0, t0, 16     // ...position clear requests...
    or a1, a1, t0       // ...add set requests...
    sw a1, BOP(a0)      // ...then execute...
    ret  