#include <xc.h>
#include "ADC.h"

void init_adc(void)
{
    //right justification
    ADFM = 1;
    
    //select refernce voltage and select i/o of pins analog or digital
    //default clock freq 10mhz
    //select input chnnel(AN0) potentiometer
 
    
    //turn on adc mode
    ADON = 1;
}

unsigned int read_adc(void)
{
    unsigned int adc_val;
    //start conversion
    GO = 1;
    
    //wait for conversion to be complete
    while(GO == 1);
    
    //read converted value
    adc_val = ADRESH << 8 | ADRESL;
    return adc_val;
}
