#include "plib.h"
#include "ds18s20.h"

//function to reset the sensor
char Reset_1820(void){
    mPORTASetPinsDigitalOut(BIT_0);     // Set as output
    mPORTAClearBits(BIT_0);             // Clear Bit
    delay_us(500);                      // Delay
    mPORTASetBits(BIT_0); 
    mPORTASetPinsDigitalIn(BIT_0);      // Set as input, meanwhile releasing
    delay_us(65);                       // Wait for the interval
    if(mPORTAReadBits(BIT_0)==0){       // There is response from DS18B20
        delay_us(480);
        return 1;                       // Successfully Reset 
    }
    else{                               // No response
        delay_us(480);
        return 0;                       // Reset Unsuccessful
    }
}

//function to write 8 bits to sensor
void Write_1820(unsigned char cmd){
    char i;

    mPORTASetPinsDigitalIn(BIT_0);      // Set as input, meanwhile releasing

    for(i=0;i<8;i++){ 
        if(((cmd>>i) & (0x01))==1){        // Write 1            
            mPORTASetPinsDigitalOut(BIT_0);     // Set as output
            mPORTAClearBits(BIT_0);             // Clear Bit
            delay_us(5);
            mPORTASetPinsDigitalIn(BIT_0);      // Set as input, meanwhile releasing
            delay_us(65);
        }
   
        else{                           // Write 0
            mPORTASetPinsDigitalOut(BIT_0);     // Set as output
            mPORTAClearBits(BIT_0);             // Clear Bit
            delay_us(65);
            mPORTASetPinsDigitalIn(BIT_0);      // Set as input, meanwhile releasing
            delay_us(5);
        }
    }
}

//function to read 8 bits from sensor
unsigned char Read_1820(void){
    char i;
    unsigned char temp=0;
    mPORTASetPinsDigitalIn(BIT_0);      // Set as input, meanwhile releasing
    for(i=0;i<8;i++){
        mPORTASetPinsDigitalOut(BIT_0);     // Set as output
        mPORTAClearBits(BIT_0);             // Clear Bit
        delay_us(5);
        mPORTASetBits(BIT_0);
        mPORTASetPinsDigitalIn(BIT_0);      // Set as input, meanwhile releasing
        delay_us(2);                       // Wait a while before reading
        if((mPORTAReadBits(BIT_0) & (0x01))==1)        // Reading bit is non-zero
            temp = (temp) | (1<<i);
        delay_us(65);
    }
    return temp;
}