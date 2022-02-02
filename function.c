
#include "mcc_generated_files/mcc.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "I2C_header.h"
#include "StateMachine.h"

char HTML_Header[160];
char HTML_Page[160];

void Idle(){
    
}

void Second_Idle() {
    
}

void CWMODE() {
    printf("AT+CWMODE=1\r\n");
}

void CWJAP() {
    printf("AT+CWJAP=\"SSID\",\"PASS\"\r\n");
}

void IP_Read() {
    printf("AT+CIFSR\r\n");
}

void TCP() {
    printf("AT+CIPMUX=1\r\n");
}

void Maxcon() {
    printf("AT+CIPSERVERMAXCONN=1\r\n");
}

void Server() {
    printf("AT+CIPSERVER=1\r\n");
}

void Web() {
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

        // Buffer der indeholder temperatur 
        char temp_buffer[18];
        // Buffer der indeholder humidity 
        char hum_buffer[18];
        
        sprintf(HTML_Header,"HTTP/1.1 200 OK\n content-Type: text/HTML\n content-Length:85\n connection:close\n\n %d", strlen(HTML_Header));
        sprintf(HTML_Page,"<!DOCTYPE html><html><head><title>Title</title><head><body><h1>State Machine</h1> <h2>@humidity: %.1f%% </h2> <h2>@Temp: %.1fC \r\n</h2> </body><html> %d", 22, 44);
}

void ATSEND1() {
    printf("AT+CIPSEND=0,%d\r\n",strlen(HTML_Header));
}

void Header() {
    printf(HTML_Header);
}

void ATSEND2() {
    printf("AT+CIPSEND=0,%d\r\n",strlen(HTML_Page));
}

void HTML() {
    printf(HTML_Page);
}

void Close() {
    printf("AT+CIPCLOSE=0\r\n");

}
