/* 
 * File:   clcd.h
 * Author: Mohamed Ihushan
 * Created on 11 August, 2026, 9:35 AM
 */

#ifndef CLCD_H
#define	CLCD_H

//frequency
#define _XTAL_FREQ                  20000000

//clcd config macros
#define CLCD_DATA_PORT_DDR          TRISD
#define CLCD_RS_DDR                 TRISE2
#define CLCD_EN_DDR                 TRISE1

//clcd config macros
#define CLCD_DATA_PORT              PORTD
#define CLCD_RS                     RE2
#define CLCD_EN                     RE1

//clcd data type macros
#define INST_MODE                   0
#define DATA_MODE                   1

//clcd data macros
#define HI                          1
#define LOW                         0

//clcd line macros
#define LINE1(x)                    (0x80 + x)
#define LINE2(x)                    (0xC0 + x)

//clcd mode macros
#define EIGHT_BIT_MODE              0x33
#define TWO_LINES_5x8_8_BIT_MODE    0x38
#define CLEAR_DISP_SCREEN           0x01
#define DISP_ON_AND_CURSOR_OFF      0x0C
#define DISP_ON_AND_CURSOR_ON       0x0E
#define DISP_ON_AND_CURSOR_BLINK    0x0F

//clcd initialize function
void init_clcd(void);

//clcd put char functiion
void clcd_putch(const char data, unsigned char addr);

//clcd print string function
void clcd_print(const char *str, unsigned char addr);

void clear_screen(void);

void clcd_write(unsigned char byte, unsigned char mode);
#endif	/* CLCD_H */

