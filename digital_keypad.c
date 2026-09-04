
#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

//read digital keypad function
unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char prev_key = ALL_RELEASED;
    static unsigned char check_key = ALL_RELEASED;
    static unsigned char debounce_counter = 0;
    static unsigned int  press_count = 0;
    static unsigned char long_press = 0;

    unsigned char raw_key = KEYPAD_PORT & INPUT_LINES;

    // LEVEL DETECTION
    if(mode == LEVEL_DETECTION)
    {
        return raw_key;
    }

    //for debounce
    if(raw_key == check_key)
    {
        if(debounce_counter < DEBOUNCE_COUNT)
        {
            debounce_counter++;
        }
    }
    else
    {
        check_key = raw_key;
        debounce_counter = 0;
    }

    if(debounce_counter < DEBOUNCE_COUNT)
    {
        return ALL_RELEASED;
    }

    unsigned char key = check_key; /*released key */

    /*SW1, SW2, SW3-> short-press only, no long*/
    if(key == SW1 || key == SW2 || key == SW3 || key == SW6)
    {
        prev_key = key;
        return ALL_RELEASED;
    }

    /*SW4, SW5 -> short and long press */
    if(key == SW4 || key == SW5)
    {
        if(prev_key == ALL_RELEASED)
        {
            prev_key = key;
            press_count = 1;
            long_press = 0;
        }
        else if(prev_key == key)
        {
            if(press_count < 65535)
            {
                press_count++;
            }

            /*return long-press the moment threshold is crossed, only once*/
            if(press_count >= SHORT_PRESS_COUNT && !long_press)
            {
                long_press = 1;
                return (key | 0x80);   /* L_SW4 or L_SW5 */
            }
        }
        return ALL_RELEASED;
    }

    /*Switch release and return pressed key*/
    if(key == ALL_RELEASED)
    {
        /* SW1 ,SW2 , SW3 */
        if(prev_key == SW1 || prev_key == SW2 || prev_key == SW3 || prev_key == SW6)
        {
            key = prev_key;
            prev_key = ALL_RELEASED;
            return key;
        }

        /* SW4 / SW5 */
        if(prev_key == SW4 || prev_key == SW5)
        {
            key = prev_key;
            prev_key = ALL_RELEASED;

            if(long_press)
            {
                press_count = 0;
                long_press = 0;
                return ALL_RELEASED;
            }
            else
            {
                press_count = 0;
                return key;
            }
        }
    }

    return ALL_RELEASED;
}