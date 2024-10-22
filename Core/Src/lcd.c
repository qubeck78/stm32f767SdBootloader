/*
 * textDisplay.c
 *
 *  Created on: Feb 26, 2024
 *      Author: qubec
 */

#include "stm32f7xx_hal.h"
#include "main.h"
#include "lcd.h"

#include <string.h>
#include <stdarg.h>

static char lcdFrameBuffer[20 * 4];
static uint8_t lcdCursX;
static uint8_t lcdCursY;

#define _IS_UPPER(c)    (((c)>='A')&&((c)<='Z'))
#define _IS_LOWER(c)    (((c)>='a')&&((c)<='z'))
#define _IS_DIGIT(c)    (((c)>='0')&&((c)<='9'))

//https://arduinowo.pl/generatory/genhd4478/
//https://www.ia.pw.edu.pl/~jurek/js/kody/


static const uint8_t charCheckmark[8] = {
   0B00000,
   0B00001,
   0B00011,
   0B10110,
   0B11100,
   0B01000,
   0B00000,
   0B00000
   };

static const uint8_t charA[8] = {
   0B00000,
   0B01110,
   0B00001,
   0B01111,
   0B10001,
   0B01111,
   0B00010,
   0B00001
   };

static const uint8_t charC[8] = {
   0B00010,
   0B00100,
   0B01110,
   0B10000,
   0B10000,
   0B10001,
   0B01110,
   0B00000
   };

static const uint8_t charE[8] = {
   0B00000,
   0B01110,
   0B10001,
   0B11111,
   0B10000,
   0B01110,
   0B00010,
   0B00001
   };

static const uint8_t charO[8] = {
   0B00010,
   0B00100,
   0B01110,
   0B10001,
   0B10001,
   0B10001,
   0B01110,
   0B00000
   };

static const uint8_t charL[8] = {
   0B01100,
   0B00100,
   0B00110,
   0B00100,
   0B01100,
   0B00100,
   0B01110,
   0B00000
   };

static const uint8_t charS[8] = {
   0B00010,
   0B00100,
   0B01110,
   0B10000,
   0B01110,
   0B00001,
   0B11110,
   0B00000
   };

static const uint8_t charN[8] = {
   0B00010,
   0B00100,
   0B10110,
   0B11001,
   0B10001,
   0B10001,
   0B10001,
   0B00000
   };

#define LCD_BUS_DELAY_CYCLES 400

static void lcdSetRS( uint8_t on )
{
   if( on )
   {
      HAL_GPIO_WritePin( LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET );
   }
}

static void lcdSetRWn( uint8_t on )
{
   if( on )
   {
      HAL_GPIO_WritePin( LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET );
   }
}

static void lcdSetE( uint8_t on )
{
   if( on )
   {
      HAL_GPIO_WritePin( LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET );
   }
}

static void lcdSetDOut( uint8_t dataOut )
{
   if( dataOut & 1 )
   {
      HAL_GPIO_WritePin( LCD_D0_GPIO_Port, LCD_D0_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D0_GPIO_Port, LCD_D0_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 2 )
   {
      HAL_GPIO_WritePin( LCD_D1_GPIO_Port, LCD_D1_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D1_GPIO_Port, LCD_D1_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 4 )
   {
      HAL_GPIO_WritePin( LCD_D2_GPIO_Port, LCD_D2_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D2_GPIO_Port, LCD_D2_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 8 )
   {
      HAL_GPIO_WritePin( LCD_D3_GPIO_Port, LCD_D3_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D3_GPIO_Port, LCD_D3_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 16 )
   {
      HAL_GPIO_WritePin( LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 32 )
   {
      HAL_GPIO_WritePin( LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 64 )
   {
      HAL_GPIO_WritePin( LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_RESET );
   }

   if( dataOut & 128 )
   {
      HAL_GPIO_WritePin( LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_RESET );
   }

}

static void lcdSendCmd( uint8_t command )
{
   volatile uint32_t j;

   lcdSetE( 0 );

   lcdSetRS( 0 );
   lcdSetRWn( 0 );

   lcdSetDOut( command );

   for( j = 0; j < LCD_BUS_DELAY_CYCLES; j++ )
      ;

   lcdSetE( 1 );

   for( j = 0; j < LCD_BUS_DELAY_CYCLES; j++ )
      ;

   lcdSetE( 0 );

   for( j = 0; j < LCD_BUS_DELAY_CYCLES; j++ )
      ;

}

static void lcdWriteData( uint8_t command )
{
   volatile uint32_t j;

   lcdSetE( 0 );

   lcdSetRS( 1 );
   lcdSetRWn( 0 );

   lcdSetDOut( command );

   for( j = 0; j < LCD_BUS_DELAY_CYCLES; j++ )
      ;

   lcdSetE( 1 );

   for( j = 0; j < LCD_BUS_DELAY_CYCLES; j++ )
      ;

   lcdSetE( 0 );

   for( j = 0; j < LCD_BUS_DELAY_CYCLES; j++ )
      ;

}

uint32_t lcdInit()
{

   //https://web.alfredstate.edu/faculty/weimandn/lcd/lcd_initialization/lcd_initialization_index.html

   lcdSetBacklight( LCD_BACKLIGHT_OFF );

   lcdSetRWn( 0 );

   lcdSetE( 0 );
   lcdSetRS( 0 );

   HAL_Delay( 100 );

   lcdSendCmd( 0x30 );

   HAL_Delay( 100 );

   lcdSendCmd( 0x30 );

   HAL_Delay( 10 );

   lcdSendCmd( 0x38 );

   HAL_Delay( 10 );

   lcdSendCmd( 0x08 );

   HAL_Delay( 10 );

   lcdSendCmd( 0x01 );

   HAL_Delay( 10 );

   lcdSendCmd( 0x06 );

   HAL_Delay( 10 );

   lcdSendCmd( 0x0c );

   HAL_Delay( 10 );


   lcdDefineChar( 1, (uint8_t *)charCheckmark );
   lcdDefineChar( 2, (uint8_t *)charA );
   lcdDefineChar( 3, (uint8_t *)charC );
   lcdDefineChar( 4, (uint8_t *)charE );
   lcdDefineChar( 5, (uint8_t *)charO );
   lcdDefineChar( 6, (uint8_t *)charL );
   lcdDefineChar( 7, (uint8_t *)charS );
   lcdDefineChar( 0, (uint8_t *)charN );

   lcdCls();
   lcdRefresh();

   return RV_OK;
}

uint32_t lcdDefineChar( uint32_t charNo, uint8_t *charDataArray )
{
   uint32_t i;

   if( charNo > 7 )
   {
      return RV_ERROR;
   }

   if( charDataArray == NULL )
   {
      return RV_ERROR;
   }

   //set cgram address

   lcdSendCmd( charNo << 3 | 64 );

   HAL_Delay( 10 );

   for( i = 0; i < 8; i++ )
   {
      lcdWriteData( charDataArray[i] );

   }

   //set ddram address

   lcdSendCmd( 0x80 );

   HAL_Delay( 10 );

   return RV_OK;
}

uint32_t lcdRefresh()
{

   uint32_t i;

   //home cursor
   lcdSendCmd( 0x02 );
   HAL_Delay( 1 );

   //0
   for( i = 0; i < 20; i++ )
   {
      lcdWriteData( lcdFrameBuffer[i] );
   }

   //2
   for( i = 0; i < 20; i++ )
   {
      lcdWriteData( lcdFrameBuffer[40 + i] );
   }

   //1
   for( i = 0; i < 20; i++ )
   {
      lcdWriteData( lcdFrameBuffer[20 + i] );
   }

   //3
   for( i = 0; i < 20; i++ )
   {
      lcdWriteData( lcdFrameBuffer[60 + i] );
   }

   return RV_OK;
}

uint32_t lcdSetBacklight( uint32_t backlightMode )
{

   if( backlightMode & LCD_BACKLIGHT_RED )
   {
      HAL_GPIO_WritePin( LCD_BL_R_GPIO_Port, LCD_BL_R_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_BL_R_GPIO_Port, LCD_BL_R_Pin, GPIO_PIN_RESET );
   }

   if( backlightMode & LCD_BACKLIGHT_GREEN )
   {
      HAL_GPIO_WritePin( LCD_BL_G_GPIO_Port, LCD_BL_G_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( LCD_BL_G_GPIO_Port, LCD_BL_G_Pin, GPIO_PIN_RESET );
   }

   return RV_OK;
}

uint32_t lcdCls()
{
   uint32_t i;

   for( i = 0; i < sizeof( lcdFrameBuffer ); i++ )
   {
      lcdFrameBuffer[i] = 32;
   }

   lcdCursX = 0;
   lcdCursY = 0;

   return RV_OK;

}

uint32_t lcdSetCursorPos( uint8_t x, uint8_t y )
{
   if( x > 19 )
   {
      return 1;
   }
   if( y > 3 )
   {
      return 1;
   }

   lcdCursX = x;
   lcdCursY = y;

   return RV_OK;
}

uint32_t lcdPutChar( char c )
{

   //CP1250
   switch( c )
   {
      case 0xa5:     //ą
         c = 0x02;
         break;

      case 0x86:     //ć
         c = 0x03;
         break;

      case 0xa9:     //ę
         c = 0x04;
         break;

      case 0xa2:     //ó
         c = 0x05;
         break;

      case 0x88:     //ł
         c = 0x06;
         break;

      case 0xe4:     //ń
         c = 0x00;
         break;

      case 0x98:     //ś
         c = 0x07;
         break;

   }

   if( c != 10 )
   {
      lcdFrameBuffer[lcdCursX + 20 * lcdCursY] = c;

      lcdCursX++;
   }
   else
   {
      //new line, force by increasing x cursor pos out of bounds
      lcdCursX = 20;
   }

   if( lcdCursX > 19 )
   {
      lcdCursX = 0;

      lcdCursY++;

      if( lcdCursY > 3 )
      {
         lcdCursY = 0;
      }
   }

   return RV_OK;
}

uint32_t lcdPrint( char *str )
{
   uint32_t i;
   char c;

   if( str == NULL )
   {
      return 1;
   }

   for( i = 0; i < 20 * 4; i++ )
   {
      c = *str++;
      if( c == 0 )
      {
         break;
      }
      else
      {
         lcdPutChar( c );
      }
   }

   return RV_OK;
}

uint32_t lcdPrintF( char *format, ... )
{
   va_list   arp;
   uint8_t   flag;
   uint32_t  radix;
   uint32_t  width;
   uint32_t  value;
   uint8_t   scratch[16];
   uint8_t  *pointer;

   uint8_t   c;
   uint8_t   d;
   uint32_t  i;
   uint32_t  j;

   va_start( arp, format );

   for( ;; )
   {

      c = *format++;

      if( c == 0 )
         break; /* End of string */

      if( c != '%' )
      {

         /* Non escape character */
         lcdPutChar( c );

         continue;
      }

      width = 0;
      flag = 0;
      c = *format++;

      if( c == '0' )
      {
         /* Flag: '0' padding */

         flag = 1;
         c = *format++;

      }
      else
      {

         if( c == '-' )
         {

            /* Flag: left justified */
            flag = 2;
            c = *format++;
         }
      }

      while( _IS_DIGIT( c ) )
      {

         /* Precision */
         width = width * 10 + c - '0';
         c = *format++;
      }

      if( ( c == 'l' ) || ( c == 'L' ) )
      {
         /* Prefix: Size is long int */

         flag |= 4;
         c = *format++;
      }

      if( !c )
         break;

      d = c;

      if( _IS_LOWER( d ) )
         d -= 0x20;

      switch( d )
      {

         /* Type is... */

         case 'S':

            /* String */

            pointer = va_arg( arp, unsigned char* );

            for( j = 0; pointer[j]; j++ )
               ;

            if( !( flag & 2 ) )
            {
               while( j++ < width )
               {
                  lcdPutChar( ' ' );

               }
            }

            while( *pointer )
            {
               lcdPutChar( *pointer++ );

            }
            while( j++ < width )
            {
               lcdPutChar( ' ' );
            }
            continue;

         case 'C': /* Character */

            lcdPutChar( (char) va_arg( arp, int ) );

            continue;

         case 'B': /* Binary */

            radix = 2;
            break;

         case 'O': /* Octal */
            radix = 8;
            break;

         case 'D': /* Signed decimal */
         case 'U': /* Unsigned decimal */

            radix = 10;
            break;

         case 'X': /* Hexdecimal */

            radix = 16;
            break;

         default: /* Unknown type (pass-through) */

            lcdPutChar( c );

            continue;
      }

      /* Get an argument and put it in numeral */

      value =
            ( flag & 4 ) ?
                  (uint32_t) va_arg( arp, long ) :
                  ( ( d == 'D' ) ?
                        (uint32_t) (long) va_arg( arp, int ) :
                        (uint32_t) va_arg( arp, unsigned int ) );

      if( d == 'D' && ( value & 0x80000000 ) )
      {
         value = 0 - value;
         flag |= 8;
      }

      i = 0;

      do
      {
         d = (char) ( value % radix );
         value /= radix;

         if( d > 9 )
            d += ( c == 'x' ) ? 0x27 : 0x07;

         scratch[i++] = d + '0';

      } while( value && i < sizeof scratch / sizeof scratch[0] );

      if( flag & 8 )
         scratch[i++] = '-';

      j = i;
      d = ( flag & 1 ) ? '0' : ' ';

      while( !( flag & 2 ) && j++ < width )
      {
         lcdPutChar( d );
      }

      do
      {

         lcdPutChar( scratch[--i] );

      } while( i );

      while( j++ < width )
      {
         lcdPutChar( d );
      }
   }

   va_end( arp );

   return RV_OK;
}

uint32_t lcdTest()
{
   static uint8_t cnt = 0;
   static uint8_t bl  = 0;
   uint32_t i;



   lcdCls();
   lcdPrintF( (char*) "Test LCD\nAla ma kota i psa\nBobika." );

   for( i = 0; i < 20; i++ )
   {
      lcdSetCursorPos( i, 3 );

      if( i != cnt )
      {
         lcdPutChar( 32 );
      }
      else
      {
         lcdPutChar( '*' );
      }
   }

   cnt++;

   if( cnt == 20 )
   {
      bl++;

      if( bl > 3 )
      {
         bl = 0;
      }

   }
   cnt %= 20;


   switch( bl )
   {
      case 0:
         lcdSetBacklight( LCD_BACKLIGHT_OFF );
         break;

      case 1:
         lcdSetBacklight( LCD_BACKLIGHT_RED );
         break;

      case 2:
         lcdSetBacklight( LCD_BACKLIGHT_GREEN );
         break;

      case 3:
         lcdSetBacklight( LCD_BACKLIGHT_YELLOW );
         break;
   }


   lcdRefresh();

   return RV_OK;
}
