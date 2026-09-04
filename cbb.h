/* 
 * File:   cbb.h
 * Author: Mohamed Ihushan
 *
 * Created on 11 August, 2026, 11:04 AM
 */

#ifndef CBB_H
#define	CBB_H


#define   DASH_BOARD              0x01
#define   PASSWORD_SCREEN         0x02
#define   MENU_SCREEN             0x03
#define   VIEW_LOG_SCREEN         0x04
#define   CLEAR_LOG_SCREEN        0x05
#define   DOWNLOAD_LOG_SCREEN     0x06
#define   SET_TIME_SCREEN         0x07
#define   CHANGE_PASSWORD_SCREEN  0x08

#define   RESET_PASSWORD          0xF1
#define   RESET_MENU_SCREEN       0xF3
#define   RESET_NOTHING           0xFF
#define   RESET_VIEW_LOG          0xF4
#define   RESET_TIME              0xF5
#define   RESET_LOG               0xF6

#define   TIME_OUT                0X11
#define   RETURN_SUCCESS          0x22
#define BLINK_TOGGLE_COUNT        10

void display_dashboard(unsigned char event[],unsigned char speed);

unsigned char check_password(unsigned char key,unsigned char reset_flag);

void log_event(unsigned char event[],unsigned char speed);

unsigned char menu_screen(unsigned char key, unsigned char reset_flag);



void view_log_screen(unsigned char key, unsigned char reset_flag);

unsigned char clear_log_screen(unsigned char key, unsigned char reset_flag);

unsigned char download_log_screen(unsigned char key, unsigned char reset_flag);

unsigned char set_time_screen(unsigned char key, unsigned char reset_flag);

unsigned char change_password_screen(unsigned char key, unsigned char reset_flag);


#endif	/* CBB_H */

