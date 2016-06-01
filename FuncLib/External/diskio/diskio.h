/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file  R0.04a   (C)ChaN, 2007
/-----------------------------------------------------------------------*/

#ifndef _DISKIO
#define _DISKIO

#define _READONLY	0	/* 1: Read-only mode */

#include <stdint.h>

/* Status of Disk Functions */
typedef uint8_t	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */
/*

DSTATUS disk_initialize (uint8_t);
DSTATUS disk_status (uint8_t);
DRESULT disk_read (uint8_t, uint8_t*, uint32_t, uint8_t);
DRESULT disk_write (uint8_t, const uint8_t*, uint32_t, uint8_t);
DRESULT disk_ioctl (uint8_t, uint8_t, void*);
*/



/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl() */

#define GET_SECTOR_COUNT	1
#define GET_SECTOR_SIZE		2
#define CTRL_SYNC			3
#define CTRL_POWER			4
#define CTRL_LOCK			5
#define CTRL_EJECT			6

#define MMC_GET_CSD			10
#define MMC_GET_CID			11
#define MMC_GET_OCR			12
#define MMC_GET_TYPE		13



#define disk_initialize(drv) SD_disk_Init()

#define disk_status(drv) SD_disk_status()

#define disk_read(drv, buff, sector, count) SD_Read(buff, sector, count)

#define disk_ioctl(drv, ctrl, buff) SD_disk_ioctl(ctrl, buff)

#if	_READONLY == 0
	#define disk_write(drv, buff, sector, count) SD_Write(buff, sector, count)
#endif



#endif
