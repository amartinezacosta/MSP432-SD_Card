#ifndef HARDWARE_SPI_DRIVER_H_
#define HARDWARE_SPI_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/spi.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

void SPI_Init(uint32_t SPI, eUSCI_SPI_MasterConfig SPIConfig);
void SPI_Write(uint32_t SPI, uint8_t *Data, uint32_t Size);
void SPI_Read(uint32_t SPI, uint8_t *Data, uint32_t Size);

#endif /* HARDWARE_SPI_DRIVER_H_ */
