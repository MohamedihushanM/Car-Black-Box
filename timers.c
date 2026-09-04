#include <xc.h>
#include "timers.h"

//tmer2
void init_timer2(void)
{
    TMR2ON = 0;
    
    PR2 = 250;
    
    TMR2IE = 1;
    
    TMR2IF = 0;
}
