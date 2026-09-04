#include <xc.h>

extern unsigned char return_time;
extern unsigned char sec;
unsigned char ch;

__interrupt() void isr(void)
{
    static unsigned int count;
 
    
    if(TMR2IF)
    {
        if(++count == 20000)
        {
            count = 0;
            if(return_time > 0)
            {
                return_time--;
            }
            if(sec > 0)
            {
                sec--;
            }
        }
        TMR2IF = 0;
    }
    
    //interrupt for UART
    if (RCIF == 1)
    {
        if (OERR == 1)
            OERR = 0;
        
        ch = RCREG;
        
        RCIF = 0;
    }
}



