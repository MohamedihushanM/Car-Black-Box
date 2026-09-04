#include <xc.h>
#include "digital_keypad.h"
#include "clcd.h"
#include "ADC.h"
#include "DS1307.h"
#include "i2c.h"
#include "cbb.h"
#include <string.h>
#include "eeprom.h"
#include "uart.h"

unsigned char clock_reg[3];
unsigned char return_time;
unsigned char sec;
unsigned char time[7];
unsigned char log[11];
unsigned char pos;
unsigned char log_count = 0;
unsigned char saved_password[4] = {'1','1','1','1'};

unsigned char *menu[] = {"View log","Clear log","Download log","Set time","Change pwd"};

//get time function
static void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
    //BCD Format 
    //clock_reg[0] = HH
    //clock_reg[1] = MM
    //clock_reg[2] = SS
   
    /* To store the time in HH:MM:SS format */
      // "HH:MM:SS"
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    clcd_putch(time[0],LINE2(0));
    clcd_putch(time[1],LINE2(1));
    clcd_putch(':',LINE2(2));
    
  
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    clcd_putch(time[2],LINE2(3));
    clcd_putch(time[3],LINE2(4));
    clcd_putch(':',LINE2(5));
    
  
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
    clcd_putch(time[4],LINE2(6));
    clcd_putch(time[5],LINE2(7));
    
  
}

//log event store into EEPROM function
void log_event(unsigned char event[],unsigned char speed)
{
    unsigned char add;
    unsigned char i;
    
    get_time();
    
    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    
    log[8] = speed/10 + '0';
    log[9] = speed%10 + '0';
    
    add = pos*10;
    
    for(i = 0; i < 10; i++)
    {
        ext_eeprom_24C02_byte_write(add + i, log[i]);
        __delay_ms(5);
    }
    
    pos++;
    if(pos == 10)
        pos = 0;

    if(log_count < 10)
        log_count++;
}

//display dashboard function
void display_dashboard(unsigned char event[],unsigned char speed)
{
    clcd_print("TIME      EV  SP",LINE1(0));
    get_time();
    clcd_print(event,LINE2(10));
    clcd_putch(speed/10 + '0',LINE2(14));
    clcd_putch(speed%10 + '0',LINE2(15));
}

//check password function
unsigned char check_password(unsigned char key,unsigned char reset_flag)
{
    static unsigned char user_password[4];
    static unsigned char i,attempt;
    if(reset_flag == RESET_PASSWORD)
    {
        attempt = 3;
        i = 0;
        user_password[0] = '\0';
        user_password[1] = '\0';
        user_password[2] = '\0';
        user_password[3] = '\0';

      
        return_time = 5;
        key = 0;
    
    }
    //read passsword
    //timeout, 5 sec
    if(return_time == 0)
    {
        return TIME_OUT;
    }
    
    if(key == SW4 && i < 4)
    {
        user_password[i] = '1';
        clcd_putch('*',LINE2(4+i));
        i++;
        return_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        user_password[i] = '0';
        clcd_putch('*',LINE2(4+i));
        i++;
        return_time = 5;
        
    }
    
    if(i == 4)
    {
        //compare
        if(strncmp(user_password,saved_password,4)== 0)
        {
          clear_screen();
            
            clcd_print("PASSWORD SUCCESS",LINE1(0));
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            TMR2ON = 0;
            return RETURN_SUCCESS;
        }
        
        else
        {
            //reduce attempt
            attempt--;
            if(attempt == 0)
            {
                //lock screen
                 clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("YOU ARE BLOCKED",LINE1(0));
                clcd_print("WAIT FOR",LINE2(0));
                sec = 60;
                while(sec != 0)
                {
                    clcd_putch(sec/10 + '0',LINE2(9));
                    clcd_putch(sec%10 + '0',LINE2(10));
                    
                }
                clcd_print("sec",LINE2(12));
                
                attempt = 3;
         
            }
            else
            {
                clear_screen();
                
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("WRONG PASSWORD",LINE1(0));
                clcd_print("attempt left",LINE2(0));
                clcd_putch(attempt + '0',LINE2(14));
                __delay_ms(1500);
                
            }
            clear_screen();
            clcd_print("ENTER PASSWORD",LINE1(0));
            clcd_write(DISP_ON_AND_CURSOR_BLINK, INST_MODE);
            __delay_us(100);
            i = 0;
            return_time = 5;
        }
    }
 
}

//menu screen select function
unsigned char menu_screen(unsigned char key, unsigned char reset_flag)
{
    //scroll up and down wil menu option
    static unsigned char menu_pos = 0;
    static unsigned char select_pos;
    
    if(reset_flag == RESET_MENU_SCREEN)
    {
        menu_pos = 0;
        select_pos = 0;
        
    }
    if(key == SW5 && menu_pos < 4)
    {
        clear_screen();
        menu_pos++;
        if(select_pos < 2)
            select_pos++;
    }
    else if(key == SW4 && menu_pos > 0)
    {
        clear_screen();
        menu_pos--;
        if(select_pos > 1)
            select_pos--;
    }
    if(select_pos == 1 || menu_pos == 0)
    {
        clcd_putch('*',LINE1(0));
        
        clcd_print(menu[menu_pos],LINE1(1));//view log    //clear                                      
        clcd_print(menu[menu_pos + 1], LINE2(1)); //clear log   //download
    }
    
    else
    {
        clcd_putch('*',LINE2(0));
        
        clcd_print(menu[menu_pos - 1],LINE1(1));
        clcd_print(menu[menu_pos],LINE2(1));
    }
    //return menu position which is select
    return menu_pos;
}


//view log in screen function
void view_log_screen(unsigned char key, unsigned char reset_flag)
{
    static unsigned char entry_index = 0;
    unsigned char i;
    unsigned char addr;
    unsigned char actual_slot;

    if(reset_flag == RESET_VIEW_LOG)
    {
        entry_index = 0;
        clear_screen();
    }

    if(log_count == 0)
    {
        clcd_print("NO LOG AVAILABLE",LINE1(0));
        return;
    }

    //up scroll
    if(key == SW4)
    {
        if(entry_index < log_count - 1)
            entry_index++;
        clear_screen();
    }
    
    //down scroll
    else if(key == SW5)
    {
        if(entry_index > 0)
            entry_index--;
        clear_screen();
    }

    
    if(log_count < 10)
    {
        actual_slot = entry_index;
    }
    else
    {
        actual_slot = (pos + entry_index) % 10; 
    }
    
    //read from EEPROM
    addr = actual_slot * 10;
    for(i = 0; i < 10; i++)
    {
        log[i] = ext_eeprom_24C02_read(addr + i);
    }
    log[10] = '\0';

    clcd_print("#  TIME    EV SP",LINE1(0));

    clcd_putch(entry_index + '0', LINE2(0));

    clcd_putch(log[0],LINE2(2));
    clcd_putch(log[1],LINE2(3));
    clcd_putch(':',LINE2(4));
    clcd_putch(log[2],LINE2(5));
    clcd_putch(log[3],LINE2(6));
    clcd_putch(':',LINE2(7));
    clcd_putch(log[4],LINE2(8));
    clcd_putch(log[5],LINE2(9));

    clcd_putch(log[6],LINE2(11));
    clcd_putch(log[7],LINE2(12));

    clcd_putch(log[8],LINE2(14));
    clcd_putch(log[9],LINE2(15));
}

//clear the all vlog in screen
unsigned char clear_log_screen(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_LOG)
    {
        pos = 0;
        log_count = 0;

        clear_screen();
        clcd_print("LOG CLEARED",LINE1(0));
        clcd_print("SUCCESSFULLY",LINE2(0));

        return_time = 5;   
        TMR2ON = 1;
    }

    if(return_time == 0)
    {
        TMR2ON = 0;
        return 1;   
    }

    return 0;
}

//transfer logs to tera term
unsigned char download_log_screen(unsigned char key, unsigned char reset_flag)
{
    unsigned char index;
    unsigned char i;
    unsigned char addr;

    if(reset_flag == RESET_LOG)
    {
        clear_screen();

        if (log_count == 0) 
        {
            uart_puts("No logs to download\r\n");
        }
        
        //transfer logs to tera term
        else 
        {
            for (index = 0; index < log_count; index++) 
            {
                addr = index * 10;
                for (i = 0; i < 10; i++) 
                {
                    log[i] = ext_eeprom_24C02_read(addr + i);
                }
                log[10] = '\0';

                uart_putchar(index + '0');
                uart_putchar(' ');

                uart_putchar(log[0]);
                uart_putchar(log[1]);
                uart_putchar(':');
                uart_putchar(log[2]);
                uart_putchar(log[3]);
                uart_putchar(':');
                uart_putchar(log[4]);
                uart_putchar(log[5]);
                uart_putchar(' ');

                uart_putchar(log[6]);
                uart_putchar(log[7]);
                uart_putchar(' ');

                uart_putchar(log[8]);
                uart_putchar(log[9]);

                uart_putchar('\r');
                uart_putchar('\n');
            }
        }
        

        clear_screen();
        clcd_print("DOWNLOAD",LINE1(0));
        clcd_print("COMPLETE",LINE2(0));

        return_time = 5;
        TMR2ON = 1;
    }

    if(return_time == 0)
    {
        TMR2ON = 0;
        return 1; 
    }

    return 0;
}

//set time function
unsigned char set_time_screen(unsigned char key, unsigned char reset_flag)
{
    static unsigned char hh, mm, ss;
    static unsigned char field;        
    static unsigned int  blink_counter;
    static unsigned char blink_visible;

    if(reset_flag == RESET_TIME)
    {
        unsigned char raw_h = read_ds1307(HOUR_ADDR);
        unsigned char raw_m = read_ds1307(MIN_ADDR);
        unsigned char raw_s = read_ds1307(SEC_ADDR);

        hh = ((raw_h >> 4) & 0x03) * 10 + (raw_h & 0x0F);
        mm = ((raw_m >> 4) & 0x07) * 10 + (raw_m & 0x0F);
        ss = ((raw_s >> 4) & 0x07) * 10 + (raw_s & 0x0F);

        field = 0;
        blink_counter = 0;
        blink_visible = 1;

        clear_screen();
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);   
        __delay_us(100);
        clcd_print("TIME (HH:MM:SS)",LINE1(0));
        return 0;
    }

    //update time and back to menu screen
    if(key == L_SW4)
    {
        unsigned char bcd_h = ((hh / 10) << 4) | (hh % 10);
        unsigned char bcd_m = ((mm / 10) << 4) | (mm % 10);
        unsigned char bcd_s = ((ss / 10) << 4) | (ss % 10);

        write_ds1307(HOUR_ADDR, bcd_h);
        write_ds1307(MIN_ADDR, bcd_m);
        write_ds1307(SEC_ADDR, bcd_s);

        clear_screen();
        clcd_print("TIME UPDATED",LINE1(0));
        clcd_print("SUCCESSFULLY",LINE2(0));
        return RETURN_SUCCESS;
    }

    //increament value
    if(key == SW4)
    {
        if(field == 0) { hh = (hh + 1) % 24; }
        else if(field == 1) { mm = (mm + 1) % 60; }
        else if(field == 2) { ss = (ss + 1) % 60; }
    }
    
    //change field
    else if(key == SW5)
    {
        field++;
        if(field > 2)
            field = 0;

        blink_counter = 0;
        blink_visible = 1;  
    }


    //blinking the selected field
    blink_counter++;
    if(blink_counter >= BLINK_TOGGLE_COUNT)
    {
        blink_counter = 0;
        blink_visible = !blink_visible;
    }

    /* Hour field */
    if(field == 0 && !blink_visible)
    {
        clcd_print("  ", LINE2(0));
    }
    else
    {
        clcd_putch(hh/10 + '0', LINE2(0));
        clcd_putch(hh%10 + '0', LINE2(1));
    }
    clcd_putch(':', LINE2(2));

    /* Minute field */
    if(field == 1 && !blink_visible)
    {
        clcd_print("  ", LINE2(3));
    }
    else
    {
        clcd_putch(mm/10 + '0', LINE2(3));
        clcd_putch(mm%10 + '0', LINE2(4));
    }
    clcd_putch(':', LINE2(5));

    /* Second field */
    if(field == 2 && !blink_visible)
    {
        clcd_print("  ", LINE2(6));
    }
    else
    {
        clcd_putch(ss/10 + '0', LINE2(6));
        clcd_putch(ss%10 + '0', LINE2(7));
    }

    return 0;
}

//change password function
unsigned char change_password_screen(unsigned char key, unsigned char reset_flag)
{
    static unsigned char new_pw[4], confirm_pw[4];
    static unsigned char stage;
    static unsigned char i;

    if(reset_flag == RESET_PASSWORD)
    {
        stage = 0;
        i = 0;
        return_time = 5;
        clear_screen();
        clcd_print("ENTER NEW PWD",LINE1(0));
        clcd_print(" ",LINE2(3));
        clcd_write(DISP_ON_AND_CURSOR_BLINK, INST_MODE);
        __delay_us(100);
        return 0;   
    }

    if(return_time == 0)
    {
        TMR2ON = 0;
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
        __delay_us(100);
        return TIME_OUT;
    }

    if((key == SW4 || key == SW5 || key == L_SW4 || key == L_SW5) && i < 4)
    {
        unsigned char digit = (key == SW4 || key == L_SW4) ? '1' : '0';

        if(stage == 0)
        {
            new_pw[i] = digit;
        }
        else
        {
            confirm_pw[i] = digit;
        }

        clcd_putch('*', LINE2(4+i));
        i++;
        return_time = 5;
    }

    if(i == 4)
    {
        if(stage == 0)
        {
            stage = 1;
            i = 0;
            clear_screen();
            clcd_print("RE-ENTER PWD",LINE1(0));
            clcd_print(" ",LINE2(3));
            return_time = 5;
        }
        else
        {
            if(strncmp(new_pw, confirm_pw, 4) == 0)
            {
                saved_password[0] = new_pw[0];
                saved_password[1] = new_pw[1];
                saved_password[2] = new_pw[2];
                saved_password[3] = new_pw[3];

                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("PASSWORD",LINE1(0));
                clcd_print("CHANGED",LINE2(0));
                TMR2ON = 0;
                return RETURN_SUCCESS;
            }
            else
            {
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("MISMATCH",LINE1(0));
                clcd_print("TRY AGAIN",LINE2(0));
                __delay_ms(1500);

                stage = 0;
                i = 0;
                clear_screen();
                clcd_print("NEW PASSWORD",LINE1(0));
                clcd_write(DISP_ON_AND_CURSOR_BLINK, INST_MODE);
                __delay_us(100);
                return_time = 5;
            }
        }
    }

    return 0;
}


