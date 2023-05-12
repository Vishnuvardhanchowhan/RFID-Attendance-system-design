/*
 * lcd.h
 *
 *  Created on: 09-Feb-2023
 *      Author: Aksh garg
 */

#ifndef LCD_H_
#define LCD_H_

void lcd_portconfig(void);
void lcd_command(uint32_t);
void lcd_char(char);
void lcd_string(char *);
void lcd_hex(char *);
void lcd_hex_char(uint8_t);
void lcd_cursor (int, int);
void lcd_print (int, int, uint32_t, int);
void lcd_init(void);

#endif /* LCD_H_ */
