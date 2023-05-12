/*
 * sample.c
 *
 *  Created on: 19-Dec-2022
 *      Author: Aksh Garg
 */

//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "inc/hw_ssi.h"
#include "driverlib/pin_map.h"
#include "Mfrc522.h"
#include "lcd.h"
#include <string.h>
#include "driverlib/uart.h"
#include "driverlib/debug.h"
#include "driverlib/eeprom.h"
/*PIN Connections:
 * Used SSI0 (Module 0)
 *
 *
 * SDA / CS / FSS ------------ PA3
 * SCK  / CLK     ------------ PA2
 * MOSI / TX      ------------ PA5
 * MISO /  RX     ------------ PA4
 *
 * RST            ------------ PF4 *
 *
 */

#define redLED   0x00000002
#define blueLED  0x00000004
#define greenLED 0x00000008

#define CARD_LENGTH 10


void hardware_init(void);



uint8_t Version;
uint8_t AntennaGain;
uint8_t status;
uint32_t readTeste;
unsigned char str[MAX_LEN];
unsigned char cardID[CARD_LENGTH];
unsigned char key[6];
unsigned char first_name[MAX_LEN];
unsigned char last_name[MAX_LEN];
unsigned char rollNo[MAX_LEN];



//Library modified to work with CCS



int main(void) {


    hardware_init();
    lcd_init();
//    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);

    Init();   // initialize MFRC522

//    Version = ReadReg(VersionReg);
//    AntennaGain = ReadReg(PICC_REQIDL) & (0x07<<4);
    int te = ReadReg(TxControlReg);
//    lcd_string("Version: ");
//    lcd_print(1, 10, Version, 5);
//    lcd_cursor(2,1);
//    lcd_string("Gain: ");
//    lcd_print(2, 7, AntennaGain, 3);
//    lcd_print(2, 11, te, 3);
    if(te==131){
        lcd_string("Scan your card");
        lcd_cursor(2,1);
        lcd_string("to Register");
    }

    int i;
    for(i=0; i<6; i++){
        key[i] = 0xFF;
    }

    while(1){
        status = Request(PICC_REQIDL, str);
        if(status == MI_OK){


            status = Anticoll(str);
            memcpy(cardID, str, 10);
            if(status == MI_OK){
                lcd_init();
                GPIOPinWrite(GPIO_PORTF_BASE, blueLED, blueLED);
                lcd_string("Card Detected...");
                lcd_cursor(2,1);
                lcd_string("Please wait...");
                GPIOPinWrite(GPIO_PORTF_BASE, blueLED, 0);


                uint32_t numcards;
                uint32_t IDstored;
                int cardFound = 0;
                uint32_t scannedCard = 0;
                EEPROMRead(&numcards, 0x0, 4);
                int i;
                scannedCard = scannedCard | (cardID[0]) << 24;
                scannedCard = scannedCard | (cardID[1]) << 16;
                scannedCard = scannedCard | (cardID[2]) << 8;
                scannedCard = scannedCard | (cardID[3]);
                for(i=1; i<=numcards; i++){
                    EEPROMRead(&IDstored, i*4, 4);
                    if(IDstored == scannedCard){
                        cardFound=1;
                        break;
                    }
                }
                if(cardFound){
                    GPIOPinWrite(GPIO_PORTF_BASE, redLED, redLED);
                    lcd_init();
                    lcd_string("You are already");
                    lcd_cursor(2,1);
                    lcd_string("Registered here");
                    SysCtlDelay(SysCtlClockGet()); //Delay 3 sec
                    GPIOPinWrite(GPIO_PORTF_BASE, redLED, 0);
                    te = ReadReg(TxControlReg);
                    if(te==131){
                        lcd_init();
                        lcd_string("Scan your card");
                        lcd_cursor(2,1);
                        lcd_string("to Register");
                    }
                    continue;
                }

                unsigned char roll[MAX_LEN];
                unsigned char first[MAX_LEN];
                unsigned char last[MAX_LEN];
                int counts = 0;
                UARTCharPut(UART0_BASE, 'E');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, 'e');
                UARTCharPut(UART0_BASE, 'r');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, 'R');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, 'l');
                UARTCharPut(UART0_BASE, 'l');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, 'N');
                UARTCharPut(UART0_BASE, 'o');
                UARTCharPut(UART0_BASE, '.');
                UARTCharPut(UART0_BASE, ' ');
                while(1){
                   unsigned char inp = UARTCharGet(UART0_BASE);
                   if(inp==13 | inp=='\n' | inp=='\t') break;
                   if(counts < MAX_LEN){
                       roll[counts++] = inp;
                   }
                }
                for(;counts<MAX_LEN;counts++){
                    roll[counts] = 0;
                }
                UARTCharPut(UART0_BASE, '\n');

                counts = 0;
                UARTCharPut(UART0_BASE, 'E');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, 'e');
                UARTCharPut(UART0_BASE, 'r');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, 'F');
                UARTCharPut(UART0_BASE, 'i');
                UARTCharPut(UART0_BASE, 'r');
                UARTCharPut(UART0_BASE, 's');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, 'N');
                UARTCharPut(UART0_BASE, 'a');
                UARTCharPut(UART0_BASE, 'm');
                UARTCharPut(UART0_BASE, 'e');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                while(1){
                   unsigned char inp = UARTCharGet(UART0_BASE);
                   if(inp==13 | inp=='\n' | inp=='\t') break;
                   if(counts < MAX_LEN){
                       first[counts++] = inp;
                   }
                }
                for(;counts<MAX_LEN;counts++){
                    first[counts] = 0;
                }
                UARTCharPut(UART0_BASE, '\n');

                counts = 0;
                UARTCharPut(UART0_BASE, 'E');
                UARTCharPut(UART0_BASE, 'n');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, 'e');
                UARTCharPut(UART0_BASE, 'r');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, 'L');
                UARTCharPut(UART0_BASE, 'a');
                UARTCharPut(UART0_BASE, 's');
                UARTCharPut(UART0_BASE, 't');
                UARTCharPut(UART0_BASE, ' ');
                UARTCharPut(UART0_BASE, 'N');
                UARTCharPut(UART0_BASE, 'a');
                UARTCharPut(UART0_BASE, 'm');
                UARTCharPut(UART0_BASE, 'e');
                UARTCharPut(UART0_BASE, ':');
                UARTCharPut(UART0_BASE, ' ');
                while(1){
                   unsigned char inp = UARTCharGet(UART0_BASE);
                   if(inp==13 | inp=='\n' | inp=='\t') break;
                   if(counts < MAX_LEN){
                       last[counts++] = inp;
                   }
                }
                for(;counts<MAX_LEN;counts++){
                    last[counts] = 0;
                }
                UARTCharPut(UART0_BASE, '\n');


                int size = SelectTag(cardID);

                status = Auth(0x61, 15, key, cardID);
                if(status==MI_OK){
                    status = WriteBlock(12, first);
                    if(status!=MI_OK){
                        GPIOPinWrite(GPIO_PORTF_BASE, redLED, redLED);
                        lcd_init();
                        lcd_string("Please try again");
                        lcd_cursor(2,1);
                        lcd_string("Itni kya jaldi h");
                    }
                    status = WriteBlock(13, last);
                    if(status!=MI_OK){
                        GPIOPinWrite(GPIO_PORTF_BASE, redLED, redLED);
                        lcd_init();
                        lcd_string("Please try again");
                        lcd_cursor(2,1);
                        lcd_string("Itni kya jaldi h");
                    }
                    status = WriteBlock(14, roll);
                    if(status!=MI_OK){
                        GPIOPinWrite(GPIO_PORTF_BASE, redLED, redLED);
                        lcd_init();
                        lcd_string("Please try again");
                        lcd_cursor(2,1);
                        lcd_string("Itni kya jaldi h");
                    }

                    status = ReadBlock(14, str);
                    if(status!=MI_OK){
                        GPIOPinWrite(GPIO_PORTF_BASE, redLED, redLED);
                        lcd_init();
                        lcd_string("Please try again");
                        lcd_cursor(2,1);
                        lcd_string("Itni kya jaldi h");
                    }
                    else{
                        memcpy(rollNo, str, 16);
                        GPIOPinWrite(GPIO_PORTF_BASE, greenLED, greenLED);
                        lcd_init();
                        lcd_string((char *)rollNo);
                        lcd_cursor(2,1);
                        lcd_string("RegistrationDone");
                        numcards++;
                        EEPROMProgram(&numcards, 0x0, 4);
                        EEPROMProgram(&scannedCard, numcards*4, 4);
                    }






                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
                    Init();
                }
                else{
                    UARTCharPut(UART0_BASE, 'A');
                    UARTCharPut(UART0_BASE, 'u');
                    UARTCharPut(UART0_BASE, 't');
                    UARTCharPut(UART0_BASE, 'h');
                    UARTCharPut(UART0_BASE, ' ');
                    UARTCharPut(UART0_BASE, 'F');
                    UARTCharPut(UART0_BASE, 'a');
                    UARTCharPut(UART0_BASE, 'i');
                    UARTCharPut(UART0_BASE, 'l');
                    UARTCharPut(UART0_BASE, 'e');
                    UARTCharPut(UART0_BASE, 'd');
                    UARTCharPut(UART0_BASE, '\n');
                    GPIOPinWrite(GPIO_PORTF_BASE, redLED, redLED);
                    lcd_init();
                    lcd_string("Authentication");
                    lcd_cursor(2,1);
                    lcd_string("Failed!");
                }


                SysCtlDelay(SysCtlClockGet()); //Delay 3 sec
                GPIOPinWrite(GPIO_PORTF_BASE, redLED, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, greenLED, 0);
                te = ReadReg(TxControlReg);
                if(te==131){
                    lcd_init();
                    lcd_string("Scan your card");
                    lcd_cursor(2,1);
                    lcd_string("to Register");
                }
            }
        }


    }
}





void hardware_init(void){
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);  // 40MHz


    // SPI Initialize
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  //SDA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);  // SPI
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // RESET
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
//    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3); //chipSelectPin
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //NRSTPD and LED Ports
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 9600, 8);
    SSIEnable(SSI0_BASE);




    // to initialize UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // initializing EEPROM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    EEPROMInit();
}

