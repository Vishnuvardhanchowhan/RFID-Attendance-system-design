// TASK 1 : Create a separate header file, named "myLib.h" and put all the required headers in the that file
// No built-in library file should be in this code
#include "myLib.h"

// TASK 2 : Create a header filen named "lcd.h" and write the appropriate function declaration for the user defined function used over here
#include "lcd.h"


#define lcd_delay    SysCtlDelay(67000 *2) // changed

#define lcd_put_data(x)  GPIOPinWrite(GPIO_PORTB_BASE,(0xFF), x)

#define lcd_set_RS() (GPIOPinWrite(GPIO_PORTC_BASE,(GPIO_PIN_4),0x01<<4))
#define lcd_set_RW() (GPIOPinWrite(GPIO_PORTC_BASE,(GPIO_PIN_5),0x01<<5))
#define lcd_set_EN() (GPIOPinWrite(GPIO_PORTC_BASE,(GPIO_PIN_6),0x01<<6))
#define lcd_reset_RS() (GPIOPinWrite(GPIO_PORTC_BASE,(GPIO_PIN_4),~(0x01<<4)))
#define lcd_reset_RW() (GPIOPinWrite(GPIO_PORTC_BASE,(GPIO_PIN_5),~(0x01<<5)))
#define lcd_reset_EN() (GPIOPinWrite(GPIO_PORTC_BASE,(GPIO_PIN_6),~(0x01<<6)))

/*
 * lcd_portconfig();
 * Description: Sets LCD port pin as output
 * Datapins: PORTB pin 0 to pin8 (LCD is interfaced in 8 bit mode)
 * ControlPins: PORTC pin4,pin5 and pin6 is connected to RS, RW and EN respectively.
 * These connection is for TIVA booster board designed by WEL lab. Change Port connection for accroding to interface
 */

void lcd_portconfig(void)
{
    //Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //* * * * * * Setting Ouput for LCD * * * * * * * *
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);

    // * * * * * * LCD_Control Pin* * * * * * *
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, 0x70);
}

/*
 * lcd_init();
 * Description: Set ports pins as outputs. Initialize LCD in 8-bit mode.
 *
 * This should be the first function that must be called in main when using LCD
 */
void lcd_init()
{
    lcd_portconfig();

    lcd_reset_RW();
    lcd_reset_EN();

    lcd_command(0x38); // function set command
    lcd_command(0x0f); // display switch command
    lcd_command(0x0C);
    lcd_command(0x06); // input set command
    lcd_command(0x01); // clear screen command
    lcd_command(0x80); // Set cursor to first line starting
}

/*
 * lcd_command()
 * Description: Sends command on databus
 * usage: lcd_command(0x01) - clears the display
 */
void lcd_command(uint32_t cmd)
{
    lcd_put_data(cmd);
    lcd_reset_RS();
    lcd_delay;
    lcd_set_EN();
    lcd_delay;
    lcd_reset_EN();
    lcd_delay;
}

/*
 *lcd_char()
 *Description: Print single character
 *Example lcd_char('A'); prints letter A
 */
void lcd_char(char data)
{
    /* TASK 3 : Write the code here
       Hint: Somehow Similar to the function lcd_command()*/
    lcd_set_RS();
    lcd_put_data(data);
    lcd_delay;
    lcd_set_EN();
    lcd_delay;
    lcd_reset_EN();
    lcd_delay;
}

/*
 * lcd_string()
 * Description: Print string
 * Example: lcd_string("Hello World");
 */
void lcd_string(char *g){
    while(*g)
        lcd_char(*g++);
}

/*
 * Name: lcd_cursor (row, column). For setting cursor position in 16 by 2 lcd
 * Description: Position the LCD cursor at "row", "column"
 * row: 1,2
 * column: 1 to 16
 * Example: lcd_cursor(2,14) - Places cursor at 2nd line 14th column
*/

void lcd_cursor (int row, int column)
{
    /* TASK 4 : Write the code to set the cursor position*/
    uint32_t data;
    if(row==1){
        data = 0x80 + (column-1);
    }
    else{
        data = 0xC0 + (column-1);
    }
    lcd_command(data);
}

/*
 * Name: lcd_print (value, digit). Print value (a numeric number).
 * Description: Print number
 * value: Numeric number
 * digit: number of digits in number. Maximum allowed digit is 7
 * Example: lcd_print(456,3) - Print 456 on LCD
*/

void lcd_print (int row, int column, uint32_t value, int digits)
{
    /* row    : Input Cursor position for the row position
       column : Input Cursor position for the column position
       value  : Input integer value to be displayed on LCD
       digits : Input number to depict the number of decimal places u want to show*/

    // To set the LCD cursor position
    lcd_cursor(row,column);
    int remainder;
    int va = value;
    int i;
    for(i=digits; i>0; i--){
        int vb = va/pow(10, i-1);
        remainder = vb%10;
        int ASCII = 48+remainder;
        lcd_char(ASCII);
    }

    /* TASK 5 : Write the code to convert the integer number into ASCII format so
       that can passed to the LCD for display */
    /* You can declare your own variables also*/
}



void lcd_hex(char *g){
    int size = sizeof(g)/sizeof(*g);
    size-=size%4;
    int i;
    for(i=0; i<size; i++){
        lcd_hex_char(*g);
        *g++;
    }
}

void lcd_hex_char(uint8_t g){
    int r;
    int q;
    r = g%16;
    q = g/16;
    if(q>9){
        lcd_char(55+q);
    }
    else{
        lcd_char(48+q);
    }
    if(r>9){
        lcd_char(55+r);
    }
    else{
        lcd_char(48+r);
    }
    lcd_char(32);
}
