/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"        /* FatFs lower layer API */
#include "spi/bsp_spi_flash.h"

/* Definitions of physical drive number for each drive */
#define SD_CARD     0 /* 预留 SD 卡使用 */
#define SPI_FLASH   1 /* 外部 SPI FLASH */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
//    int result;

    switch (pdrv) {
    case SD_CARD :
//        result = RAM_disk_status();

        // translate the reslut code here

        return stat;

    case SPI_FLASH :
        if (Flash_JEDEC_ID() == sFLASH_ID) {
            stat = 0;
        } else {
            stat = STA_NOINIT;
        }
        return stat;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv                /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
//    int result;

    switch (pdrv) {
    case SD_CARD :
//        result = RAM_disk_initialize();

        // translate the reslut code here

        return stat;

    case SPI_FLASH :
        SPI_Flash_Init();
        stat = disk_status(SPI_FLASH);
        return stat;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,        /* Physical drive nmuber to identify the drive */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Start sector in LBA */
    UINT count        /* Number of sectors to read */
)
{
    DRESULT res;
//    int result;

    switch (pdrv) {
    case SD_CARD :
        // translate the arguments here

//        result = RAM_disk_read(buff, sector, count);

        // translate the reslut code here

        return res;

    case SPI_FLASH :
        // 每个扇区 4096 字节，偏移 512 扇区，即 2M 字节
        sector += 512;
        Flash_Read_Data(buff, sector << 12, count << 12);
        res = RES_OK;
        return res;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,            /* Physical drive nmuber to identify the drive */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Start sector in LBA */
    UINT count            /* Number of sectors to write */
)
{
    DRESULT res;
//    int result;

    switch (pdrv) {
    case SD_CARD :
        // translate the arguments here

//        result = RAM_disk_write(buff, sector, count);

        // translate the reslut code here

        return res;

    case SPI_FLASH :
        // 每个扇区 4096 字节，偏移 512 扇区，即 2M 字节
        sector += 512;
        Flash_Sector_Erase(sector << 12);
        Flash_Write_Data((uint8_t *)buff, sector << 12, count << 12);
        res = RES_OK;
        return res;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT res;
//    int result;

    switch (pdrv) {
    case SD_CARD :

        // Process of the command for the SD_CARD

        return res;

    case SPI_FLASH :
        // Process of the command for the SPI_FLASH
        switch (cmd) {
            case GET_SECTOR_COUNT :
                *(DWORD*)buff = 1536;   /* 6M 字节 */
                break;
            case GET_SECTOR_SIZE :
                *(WORD*)buff = 4096;
                break;
            case GET_BLOCK_SIZE :
                *(DWORD*)buff = 1;
                break;
        }
        res = RES_OK;
        return res;
    }

    return RES_PARERR;
}

