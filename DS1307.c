#include <xc.h>
#include "i2c.h"
#include "DS1307.h"

//initialize RTS
void init_ds1307(void)
{
    unsigned char dummy;
   
    dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & 0x7F;//make 7bit ch = 0
    write_ds1307(SEC_ADDR, dummy);// ch = 0
}

//read time from RTS
unsigned char read_ds1307(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

//set time into RTS
void write_ds1307(unsigned char addr, unsigned char data) // SEc_ADDR, data
{
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}

