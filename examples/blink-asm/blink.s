.section .text
.org 0x0000
    rjmp main
.org 0x0034
    rjmp timer1_ovf_vect

.org 0x0068
.global main
main:
    ; initialize stack pointer
    ldi r16, 0xff
    out 0x3d, r16
    ldi r16, 0x04
    out 0x3e, r16

    ; create a zero register
    eor r0, r0

    ; set PORTB0 and PORTB1 as outputs (digital 8 and 9)
    sbi 0x04, 0
    sbi 0x04, 1

    ; set TCCR1A
    mov XH, r0
    ldi XL, 0x80
    st X, r0
    ; set TCCR1B
    ldi XL, 0x81
    ldi r16, 0x03
    st X, r16
    ; reset timer 1
    ldi XL, 0x84
    st X, r0
    ldi XL, 0x85
    st X, r0

    ; enable timer 1 overflow interrupt
    ldi XL, 0x6f
    ldi r16, 0x01
    st X, r16

    ; loop forever (blinking a light)
    sei
    eor r24, r24
    eor r25, r25
loop:
    adiw r24, 0x01
    brcc loop

    sbic 0x5, 0
    rjmp light_off
light_on:
    sbi 0x5, 0
    rjmp loop
light_off:
    cbi 0x5, 0
    rjmp loop

; timer1 overflow handler
timer1_ovf_vect:
    sbic 0x5, 1
    rjmp light_off_intr
light_on_intr:
    sbi 0x5, 1
    reti
light_off_intr:
    cbi 0x5, 1
    reti
