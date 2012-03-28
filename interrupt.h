/*

Some links to get this started:

http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=62216
http://code.google.com/p/arduino-tiny/source/browse/trunk/libraries/PinChangeInterrupt/PinChangeInterrupt.cpp
http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=105493

We'll need to use PCINT10 (pin 5) to match what we've implemented in hardware for the MFP (1hz)

Switches connected to PCINT8 and PCINT9

================================================

9.2 External Interrupts

External Interrupts are triggered by the INT0 pin or any of the PCINT[11:0] pins. Observe that, if enabled, the interrupts will trigger even if the INT0 or PCINT[11:0] pins are configured as outputs. This feature provides a way of generating a software interrupt. Pin change 0 interrupts PCI0 will trigger if any enabled PCINT[7:0] pin toggles. Pin change 1 interrupts PCI1 will trigger if any enabled PCINT[11:8] pin toggles. The PCMSK0 and PCMSK1 Registers control which pins contribute to the pin change interrupts. Pin change interrupts on PCINT[11:0] are detected asynchronously, which means that these interrupts can be used for waking the part also from sleep modes other than Idle mode.

The INT0 interrupt can be triggered by a falling or rising edge or a low level. This is set up as shown in “MCUCR – MCU Control Register” on page 50. When the INT0 interrupt is enabled and configured as level triggered, the interrupt will trigger as long as the pin is held low. Note that recognition of falling or rising edge interrupts on INT0 requires the presence of an I/O clock, as described in “Clock Sources” on page 25.  


GIMSK – General Interrupt Mask Register

• Bits 7, 3:0 – Res: Reserved Bits

These bits are reserved in the ATtiny24A/44A and will always read as zero.

• Bit 6 – INT0: External Interrupt Request 0 Enable

When the INT0 bit is set (one) and the I-bit in the Status Register (SREG) is set (one), the external pin interrupt is enabled. The Interrupt Sense Control bits (ISC01 and ISC00) in the External

Interrupt Control Register A (EICRA) define whether the external interrupt is activated on rising

and/or falling edge of the INT0 pin or level sensed. Activity on the pin will cause an interrupt

request even if INT0 is configured as an output. The corresponding interrupt of External Interrupt

Request 0 is executed from the INT0 Interrupt Vector.

• Bit 5 – PCIE1: Pin Change Interrupt Enable 1

When the PCIE1 bit is set (one) and the I-bit in the Status Register (SREG) is set (one), pin

change interrupt 1 is enabled. Any change on any enabled PCINT[11:8] pin will cause an interrupt. The corresponding interrupt of Pin Change Interrupt Request is executed from the PCI1

Interrupt Vector. PCINT[11:8] pins are enabled individually by the PCMSK1 Register.

• Bit 4 – PCIE0: Pin Change Interrupt Enable 0

When the PCIE0 bit is set (one) and the I-bit in the Status Register (SREG) is set (one), pin

change interrupt 0 is enabled. Any change on any enabled PCINT[7:0] pin will cause an interrupt. The corresponding interrupt of Pin Change Interrupt Request is executed from the PCI0

Interrupt Vector. PCINT[7:0] pins are enabled individually by the PCMSK0 Register.

GIFR – General Interrupt Flag Register

• Bits 7, 3:0 – Res: Reserved Bits

These bits are reserved in the ATtiny24A/44A and will always read as zero.

• Bit 6 – INTF0: External Interrupt Flag 0

When an edge or logic change on the INT0 pin triggers an interrupt request, INTF0 becomes set

(one). If the I-bit in SREG and the INT0 bit in GIMSK are set (one), the MCU will jump to the corresponding  Inter rupt  Vector .  The  f lag  is clea red when  the  inter rupt   rout ine  is executed.

Alternatively, the flag can be cleared by writing a logical one to it. This flag is always cleared

when INT0 is configured as a level interrupt.

• Bit 5 – PCIF1: Pin Change Interrupt Flag 1

When a logic change on any PCINT[11:8] pin triggers an interrupt request, PCIF1 becomes set

(one). If the I-bit in SREG and the PCIE1 bit in GIMSK are set (one), the MCU will jump to the

corresponding Interrupt Vector. The flag is cleared when the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it.

• Bit 4 – PCIF0: Pin Change Interrupt Flag 0

When a logic change on any PCINT[7:0] pin triggers an interrupt request, PCIF becomes set

(one). If the I-bit in SREG and the PCIE0 bit in GIMSK are set (one), the MCU will jump to the

corresponding Interrupt Vector. The flag is cleared when the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it.




*/

void    INT_attachInterruptA(uint8_t);
void    INT_attachInterruptB(uint8_t);



