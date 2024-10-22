/*
 * bootloader.c
 *
 *  Created on: Oct 22, 2024
 *      Author: qubec
 */

#include <stdio.h>

#include "bootloader.h"
#include "fatfs.h"
#include "lcd.h"

extern FATFS   SDFatFS;    /* File system object for SD logical drive */
extern FIL     SDFile;       /* File object for SD */

void (*programEntry)(void) = (void(*)())0x0;

static void sdTest( void )
{
   FRESULT  res;
   DIR      dir;
   FILINFO  fileInfo;

   res = f_opendir( &dir, "0:" );

   if( res != FR_OK )
   {
      printf( "f_opendir failed: %d\r\n ", res );
      return;
   }

   printf( "SD Dir:\r\n" );
   do
   {
      res = f_readdir( &dir, &fileInfo );

      if( ( res != FR_OK ) || ( fileInfo.fname[0] == '\0' ) )
      {
         break;
      }

      if(fileInfo.fattrib & AM_DIR)
      {
         printf( "DIR  %s\r\n", fileInfo.fname );
      }
      else
      {
         printf( "FILE %s\r\n", fileInfo.fname );
      }
   }while( 1 );

   res = f_closedir(&dir);
}


static uint32_t checkAndFlashImageFile( void )
{
   FRESULT                 res;
   FLASH_EraseInitTypeDef  flashEraseStruct;
   uint32_t                errorSector;
   uint32_t                flashWriteAddress;

   uint8_t                 fileReadBufB[256];
   uint32_t               *fileReadBufL;

   UINT                    bytesRead;
   uint32_t                i;

   const char spinner[] = "-\|/";

   lcdCls();
   lcdRefresh();


   res = f_open( &SDFile, "0:firmware.bin", FA_READ );

   if( res != FR_OK )
   {
      printf( "No new firmware file\r\n" );

      return 0;
   }

   printf( "Found new firmware file.\r\n" );

   //todo: check firmware file lrc

   lcdSetBacklight( LCD_BACKLIGHT_GREEN );

   lcdCls();
   lcdPrint( "Flash erase:" );
   lcdRefresh();

   //clear flash
   printf( "Flash erase:" );
   fflush( stdout );

   //1MB Flash in single bank mode
   flashEraseStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
   flashEraseStruct.Sector       = 4;                          //0x08020000
   flashEraseStruct.NbSectors    = 4;
   flashEraseStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;      //2.7 to 3.6V

   HAL_FLASH_Unlock();

   errorSector = 0;

   if( HAL_FLASHEx_Erase( &flashEraseStruct, &errorSector ) != HAL_OK )
   {
      lcdPrintF( "ERROR@%d\n", (int)errorSector );
      lcdRefresh();

      printf( "ERROR, sector:%d\r\n", (int)errorSector );

      f_close( &SDFile );

      HAL_FLASH_Lock();

      //fatal error

      lcdSetBacklight( LCD_BACKLIGHT_RED );

      do{}while( 1 );
   }


   printf( "done\r\n" );

   lcdCls();
   lcdPrint( "Flash write:" );
   lcdRefresh();

   printf( "Flash write:" );

   //user app start
   flashWriteAddress = 0x08020000;
   fileReadBufL      = (uint32_t*)fileReadBufB;

   do
   {

      bytesRead = 0;
      f_read( &SDFile, (void*)fileReadBufB, 256, &bytesRead );

      printf( "." );
      fflush( stdout );

      for( i = 0; i < 256 / 4; i++ )
      {
         //write 32-bit word
         HAL_FLASH_Program( FLASH_TYPEPROGRAM_WORD, flashWriteAddress, fileReadBufL[i] );
         flashWriteAddress += 4;
      }

   }while( !f_eof( &SDFile ) );

   lcdPrint( "done\n" );
   lcdRefresh();

   printf( "done\r\n" );

   HAL_FLASH_Lock();

   f_close( &SDFile );


   f_unlink( "0:firmware.cur" );
   f_rename( "0:firmware.bin", "0:firmware.cur" );

   return 0;
}

void bootloaderMain()
{
   uint32_t initRv;
   FRESULT  res;


   printf( "\r\n######################################################\r\n" );
   printf( "BOOTLOADER B20241022\r\n\r\n" );


   printf( "boot delay: " );
   HAL_Delay( 500 );
   printf( "done\r\n" );

   printf( "SD Card init: " );

   initRv = 0;

   if( disk_initialize( 0 ) )
   {
      printf( "error\r\n" );

      initRv |= 1;
   }
   else
   {
      printf( "ok\r\n" );
   }

   printf( "SD Card mount:" );

   res = f_mount( &SDFatFS, "", 0 );

   if( res != FR_OK )
   {
      printf( "failed: %d\r\n ", res );

      initRv |= 2;
   }
   else
   {
      printf( "ok\r\n" );
   }


   sdTest();

   checkAndFlashImageFile();


   //unmount sd card

   f_mount(0, "", 0);

    __disable_irq();

   //move irq vector
   SCB->VTOR = 0x8020000;

   //read initial pc
   programEntry = (void (*)(void)) *(__IO uint32_t*)0x8020004;

   //read and set initial stack
   __set_MSP(*(__IO uint32_t*) 0x8020000);

   //jump to app
   (*programEntry)();

}
