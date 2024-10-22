/*
 * textDisplay.h
 *
 *  Created on: Feb 26, 2024
 *      Author: qubec
 */

#ifndef SRC_PARKINGMETER_DRV_LCD_H_
#define SRC_PARKINGMETER_DRV_LCD_H_

#include "stm32f7xx_hal.h"

#define LCD_BACKLIGHT_OFF     0x00
#define LCD_BACKLIGHT_RED     0x01
#define LCD_BACKLIGHT_GREEN   0x02
#define LCD_BACKLIGHT_YELLOW  0x03

//printer compatible codes

#define _LCD_PL_a   "\xa5"
#define _LCD_PL_c   "\x86"
#define _LCD_PL_e   "\xa9"
#define _LCD_PL_o   "\xa2"
#define _LCD_PL_l   "\x88"
#define _LCD_PL_n   "\xe4"
#define _LCD_PL_s   "\x98"

uint32_t lcdInit( void );
uint32_t lcdDefineChar( uint32_t charNo, uint8_t *charDataArray );
uint32_t lcdRefresh( void );
uint32_t lcdSetBacklight( uint32_t backlightMode );
uint32_t lcdCls( void );
uint32_t lcdSetCursorPos( uint8_t x, uint8_t y );
uint32_t lcdPutChar( char c );
uint32_t lcdPrint( char *str );
uint32_t lcdPrintF( char *format, ... );

uint32_t lcdTest( void );


#endif /* SRC_PARKINGMETER_DRV_LCD_H_ */
