/*
 * File: main.c
 * Author: Mohamed Ihushan
 * Title: project - 1
 * Description : Implement car black box using PIC16f877A
 * Created on 11 August, 2026, 9:34 AM
 */

// PORTb PORTD TRISB predefined as adress
#include <xc.h>
#include "digital_keypad.h"
#include "clcd.h"
#include "ADC.h"
#include "DS1307.h"
#include "i2c.h"
#include "cbb.h"
#include "timers.h"
#include <string.h>
#include <stdio.h>
#include "uart.h"

//turn off WDT
#pragma config WDTE = OFF

void init_config() 
{
    init_digital_keypad();
    
    init_adc();
    
    init_clcd();
    
    init_i2c(100000);
    
    init_ds1307();
    
    init_timer2();
    
    init_uart(9600);
    
    PEIE = 1;
    GIE  = 1;
}

unsigned char *gear[] = {"GN","GR","G1","G2","G3","G4"};

void main(void) 
{
    unsigned char screen_flag = DASH_BOARD;
    unsigned char key;
    unsigned char reset_flag = RESET_NOTHING;
    unsigned char event[3] = "ON";
    unsigned char gr = 0;
    unsigned char speed = 0;unsigned char menu_pos;
    init_config();
    while (1) 
    {
        //read speed
        speed = (unsigned char)(read_adc()/10);//0 to 99
        if(speed >= 99)
            speed = 99;
        
       
        //reading the events
        key = read_digital_keypad(STATE);

        if (screen_flag == DASH_BOARD) 
        {
            if (key == SW1) 
            {
                //c
                strcpy(event, "C ");
                log_event(event, speed);

            } 
            
            else if (key == SW2) 
            {
                if (gr < 5)
                    gr++; //event,gn,gr,g1,g2,...

                strcpy(event, gear[gr]);
                log_event(event, speed);
            } 
            
            else if (key == SW3) 
            {
                if (gr > 0)
                    gr--;

                strcpy(event, gear[gr]);
                log_event(event, speed);
            }
        }
        
        //enter into password screen
        if((key == SW4 || key == SW5) && screen_flag == DASH_BOARD)
        {
            screen_flag = PASSWORD_SCREEN;
            
            reset_flag = RESET_PASSWORD;
            
            clear_screen();
            
            clcd_print("ENTER PASSWORD",LINE1(0));
            clcd_print(" ",LINE2(3));
            clcd_write(DISP_ON_AND_CURSOR_BLINK, INST_MODE);
            __delay_us(100);
            TMR2ON = 1;
            
        } 
        
        //enter into sub menu screen
        else if (key == L_SW4 && screen_flag == MENU_SCREEN) 
        {
            switch (menu_pos) 
            {
                case 0:
                    screen_flag = VIEW_LOG_SCREEN;
                    reset_flag = RESET_VIEW_LOG;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                    break;
                case 1:
                    screen_flag = CLEAR_LOG_SCREEN;
                    reset_flag = RESET_LOG;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                    break;
                case 2:
                    screen_flag = DOWNLOAD_LOG_SCREEN;
                    reset_flag = RESET_LOG;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                    break;
                case 3:
                    screen_flag = SET_TIME_SCREEN;
                    reset_flag = RESET_TIME;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                    break;
                case 4:
                    screen_flag = CHANGE_PASSWORD_SCREEN;
                    reset_flag = RESET_PASSWORD;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                    break;
            }
        }
        
        //back to menu screen
        else if (key == L_SW4 && screen_flag == VIEW_LOG_SCREEN) 
        {
            screen_flag = MENU_SCREEN;
            reset_flag = RESET_MENU_SCREEN;
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
        } 
        
        //back to dashboard screen
        else if (key == L_SW5 && (screen_flag == MENU_SCREEN || screen_flag == VIEW_LOG_SCREEN)) 
        {
            screen_flag = DASH_BOARD;
            reset_flag = RESET_NOTHING;
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            TMR2ON = 0;
        }
       
            
         //display screen based on screen flag
        switch(screen_flag)
        {
            case DASH_BOARD:
                 display_dashboard(event, speed);
                 break;

            case PASSWORD_SCREEN:
                switch(check_password(key,reset_flag))//timeout -> dash or success -> menu
                {
                    case TIME_OUT:
                        screen_flag = DASH_BOARD;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case RETURN_SUCCESS:
                        screen_flag = MENU_SCREEN;
                        reset_flag = RESET_PASSWORD;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                        
                break;
                }
                break;
            
            case MENU_SCREEN:
                menu_pos = menu_screen(key, reset_flag);
                break;

            case VIEW_LOG_SCREEN:
                view_log_screen(key, reset_flag); 
                break;

            case CLEAR_LOG_SCREEN:
                 if (clear_log_screen(key, reset_flag) == 1) 
                 {
                    screen_flag = MENU_SCREEN;
                    reset_flag = RESET_MENU_SCREEN;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                }
                break;

            case DOWNLOAD_LOG_SCREEN:
               if (download_log_screen(key, reset_flag) == 1) 
               {
                    screen_flag = MENU_SCREEN;
                    reset_flag = RESET_MENU_SCREEN;
                    clear_screen();
                    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                    __delay_us(100);
                }
                break;

            case SET_TIME_SCREEN:
                 unsigned char t_result = set_time_screen(key, reset_flag);
                if (t_result == RETURN_SUCCESS) 
                {
                    __delay_ms(2000);
                    screen_flag = MENU_SCREEN;
                    reset_flag = RESET_MENU_SCREEN;
                    clear_screen();
                }
                break;

            case CHANGE_PASSWORD_SCREEN:
                 unsigned char result = change_password_screen(key, reset_flag);
                if (result == RETURN_SUCCESS || result == TIME_OUT) 
                {
                    __delay_ms(2000);
                    screen_flag = MENU_SCREEN;
                    reset_flag = RESET_MENU_SCREEN;
                    clear_screen();
                }
                break;

        }     
        
        reset_flag = RESET_NOTHING;
        __delay_ms(5);
    }
    
    return;

}
