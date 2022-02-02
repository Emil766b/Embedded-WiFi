/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26K22
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "I2C_header.h"
#include "StateMachine.h"

/*
                         Main application
 */

uint8_t buff[64];
uint8_t index;
uint8_t temp;

event_t event = Ready_Event;
stateMachine_t stateMachine;

void main(void) {
    
    // Initialize the device
    SYSTEM_Initialize();

    StateMachine_Init(&stateMachine);
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // Indstil ssp modul og kør som master
    SSP2CON1 = 0x28;
    // Brug dette regnestykke til at køre som master
    SSP2ADD = (_XTAL_FREQ / (100000 * 4)) - 1;

    // Reset
    printf("AT+RST\r\n");
    // Mens 
    while (1) {
        //Hvis EUSART1_is_rx_ready, tømmer den bufferen og sætter indholdet af buffer til index
        // Ellers start forfra
        if (EUSART1_is_rx_ready()) {
            temp = EUSART1_Read();
            buff[index] = temp;
            index++;
            buff[index] = 0x00;
            //Hvis index er større end 64 reduceres den til den er 64
            if (index >= 64) {
                index--;
            }
            // Hvis der skrives en ny linje bliver de sammenligenet
            if (temp == '\n') {
                // String Compare om det er ready
                if (!strcmp(buff, "ready\r\n")) {
                    StateMachine_RunIteration(&stateMachine, Ready_Event);
                }
                // String Compare om det er OK
                else if (!strcmp(buff, "OK\r\n")) {
                    StateMachine_RunIteration(&stateMachine, ok_Event);
                }
                // String Compare om det er ERROR
                else if (!strcmp(buff, "ERROR\r\n")) {
                    StateMachine_RunIteration(&stateMachine, error_Event);
                } 
                // String Compare om det er FAIL
                else if (!strcmp(buff, "FAIL\r\n")) {
                    StateMachine_RunIteration(&stateMachine, Fail_Event);
                } 
                // String Compare om det er 0,CONNECCT
                else if (!strcmp(buff, "0,CONNECT\r\n")) {
                    StateMachine_RunIteration(&stateMachine, Connect_Event);
                } 
                // String Compare om det er SEND OK
                else if (!strcmp(buff, "SEND OK\r\n")) {
                    StateMachine_RunIteration(&stateMachine, Send_Event);
                } 
                index = 0;
            }
        }
        /*
        // Komando til at læse temperaturen
        char read_temp[] = {0xE3};
        // Buffer af 2 bit
        char buffer[2];

        //Skriv til sensoren 7021 for at få en værdi
        i2c_write(0b1000000, read_temp, 1);

        //Læs den værdi en put den ind i en buffer
        i2c_read(0b1000000, buffer, 2);

        //Flyt least significant bit 8 pladser tilbage
        float temp = buffer[0] << 8 | buffer[1];

        //Regn ud hvad temperaturen er fra den værdi sensoren gav
        temp = ((temp / 65536) * 172.72 - 46.85);

        // Float humidity
        float hum = buffer[0] << 8 | buffer[1];

        // Udregn hvad humidity er ud fra den værdi sensoren kommer med
        hum = ((hum / 65536) * 125 - 6);

        // Adresse til at initiate Display
        char display[] = {0x00, 0x38, 0x0d, 0x06};
        // Adresse til at clear display
        char clear[] = {0x00, 0b1};
        // Adresse til at skrive på linje 2 i displayet
        char line[] = {0x00, 0xC0};

        // Buffer der indeholder temperatur 
        char temp_buffer[18];
        // Buffer der indeholder humidity 
        char hum_buffer[18];

        // lager længde, tekst, 1 decimal, linje skift og temperaturen i temp buffer
        int print_temp = snprintf(temp_buffer, 18, "@Temp: %.1fC \r\n", temp);
        // lager længde, tekst, 1 decimal, linje skift og humidity i hum buffer
        int print_hum = snprintf(hum_buffer, 18, "@humidity: %.1f%% \r\n", hum);

        // Initiate Display
        i2c_write(0b0111100, display, 4);

        __delay_ms(100);
        // Skriv temperatur på display
        i2c_write(0b0111100, temp_buffer, 12);
        __delay_ms(2000);

        i2c_write(0b0111100, line, 2);

        __delay_ms(100);
        // Skriv humidity på display
        i2c_write(0b0111100, hum_buffer, 16);
        __delay_ms(2000);

        // Clear Display
        i2c_write(0b0111100, clear, 2);
         */

    }
}


/**
 End of File
 */