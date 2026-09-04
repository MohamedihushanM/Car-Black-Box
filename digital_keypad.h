/* 
 * File:   digital_keypad.h
 * Author: Mohamed Ihushan
 *
 * Created on 26 August, 2026, 11:15 AM
 */
#ifndef DIGITAL_KEYPAD_H
#define	DIGITAL_KEYPAD_H

#define LEVEL_DETECTION         0
#define LEVEL                   0

#define STATE_DETECTION         1
#define STATE                   1

#define KEYPAD_PORT             PORTB
#define KEYPAD_PORT_DDR         TRISB

#define INPUT_LINES             0x3F

#define SW1                     0x3E
#define SW2                     0x3D
#define SW3                     0x3B
#define SW4                     0x37
#define SW5                     0x2F
#define SW6                     0x1F

#define L_SW4                   0xB7
#define L_SW5                   0xAF

#define ALL_RELEASED            0x3F

#define SHORT_PRESS_COUNT       34
#define DEBOUNCE_COUNT           1   

void init_digital_keypad(void);
unsigned char read_digital_keypad(unsigned char mode);
unsigned char check_long_short(unsigned char mode);

#endif	/* DIGITAL_KEYPAD_H */