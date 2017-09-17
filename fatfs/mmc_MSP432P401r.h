#ifndef FATFS_MMC_MSP432P401R_H_
#define FATFS_MMC_MSP432P401R_H_

#include "integer.h"
#include "diskio.h"
#include "Hardware/SPI_Driver.h"
#include "Hardware/GPIO_Driver.h"

DSTATUS MMC_disk_initialize (void);
DSTATUS MMC_disk_status (void);
DRESULT MMC_disk_read (BYTE* buff, DWORD sector, UINT count);
DRESULT MMC_disk_write (const BYTE* buff, DWORD sector, UINT count);
DRESULT MMC_disk_ioctl (BYTE cmd, void* buff);

#endif /* FATFS_MMC_MSP432P401R_H_ */
