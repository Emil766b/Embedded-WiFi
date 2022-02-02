#include "mcc_generated_files/mcc.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "I2C_header.h"

char address;
char *data;
char len;

void i2c_write (char address, char *data, char len){
    //Start condition
    i2c_start();
    //Send adressen med et 0 for write
    i2c_send(address << 1);
    //Hvis den f�r et ACK forts�tter den
    if (SSP2CON2bits.ACKSTAT == 0) {
        //Hvis dataen den f�r er indenfor det den gerne vil have forts�tter for loopet
        for (int i = 0; i < len; i++){
            //Hvis den er kommet igennem sender den dataen i loopet
            i2c_send(data[i]);
            //Hvis den f�r et NAC stopper den
            if (SSP2CON2bits.ACKSTAT == 1) {
                break;
            };
        };
         
    }
        //Stop condition
        i2c_end();
};

void i2c_init (void) {
    SSP2CON1 = 0x28;
    SSP2ADD = (_XTAL_FREQ / (100000 * 4)) -1;
};

//Adresse 
void i2c_send (char address) {
    // SSP2BUF er en data buffer hvor der kan skrives til og fra
    SSP2BUF = address;
    //S� l�nge at den er 1 skriver micro controlleren
    while(SSP2STATbits.R_nW == 1);
};


// Start condition
void i2c_start (void) {
    // SSP2CON2bits bliver brugt som en start bit
    // Ved at s�tte SEN til 1 fort�ller den at der skal laves en start condition 
    SSP2CON2bits.SEN = 1;
    while(SSP2CON2bits.SEN == 1)
    {}
};

//Stop condition
void i2c_end(void) {
    // SSP2CON2bits bliver brugt som en stop bit
    // Ved at s�tte PEN til 1 fort�ller den at der skal laves en stop condition 
    SSP2CON2bits.PEN = 1;
    while(SSP2CON2bits.PEN == 1);
}
