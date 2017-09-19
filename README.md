# **MSP432 SD Card**

This is an SD Card interface using the [FatFs](http://elm-chan.org/fsw/ff/00index_e.html) FAT/exFAT file system.  
***

### **Project Setup**
***
In order to use the project you will need to have the [MSP432 SimpleLink SDK](http://www.ti.com/tool/SIMPLELINK-MSP432-SDK) library installed. To install it follow the link and select the SIMPLELINK-MSP432-SDK option, after the installation is complete import this project into Code Composer's Workspace. 

The SD card project is divided into two folders:

- **Hardware**: This folder contains all hardware peripheral code

- **FatFs**: This folder contains all of the source code for the FatFs file system

- **Devices**: This folder contains [MSPIO](https://github.com/amartinezacosta/MSPIO)

**IMPORTANT**: This project will require a serial terminal on a computer. Use your preferred serial terminals.

### **How to use it**
***
Include the following header files
```c
#include <Hardware/SPI_Driver.h>
#include <Hardware/GPIO_Driver.h>
#include <Hardware/CS_Driver.h>
#include <Hardware/TIMERA_Driver.h>
#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include <Devices/MSPIO.h>
```
Initialize all the necessary hardware peripherals using the following configuration variables:
```c
/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
eUSCI_UART_Config UART0Config =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

/* SPI Configuration Parameter. These are the configuration parameters to
 * make the eUSCI B SPI module to operate with a 500KHz clock.*/
eUSCI_SPI_MasterConfig SPI0MasterConfig =
{
     EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
     3000000,
     500000,
     EUSCI_B_SPI_MSB_FIRST,
     EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
     EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,
     EUSCI_B_SPI_3PIN
};

/* Timer_A UpMode Configuration Parameters */
Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/1 = 3MHz
        30000,                                  // 1 ms tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

```
Declare the following File System variables:
```c
FATFS FS;
DIR DI;
FILINFO FI;
```

Initialize the hardware for the SD card (SPI, GPIO and the TIMERA peripherals, UART is optional)
```c
void main(void)
{
    WDT_A_holdTimer();

    CS_Init();

    /*Initialize all hardware required for the SD Card*/
    SPI_Init(EUSCI_B0_BASE, SPI0MasterConfig);
    UART_Init(EUSCI_A0_BASE, UART0Config);
    GPIO_Init(GPIO_PORT_P5, GPIO_PIN0);
    TIMERA_Init(TIMER_A1_BASE, UP_MODE, &upConfig, disk_timerproc);

    Interrupt_enableMaster();
  }
```
Mount the SD card and check if it succeeded 
```c
FRESULT r

/*First we should mount the SD Card into the Fatfs file system*/
r = f_mount(&FS, "0", 1);
/*Check for errors. Trap MSP432 if there is an error*/
if(r != FR_OK)
{
	MSPrintf(EUSCI_A0_BASE, "Error mounting SD Card, check your connections\r\n");
	while(1);
}
```
If your SD card was mounted properly then you are ready to read, write and create files. You can see the available API functions in the ff.h file, or click this [link](http://elm-chan.org/fsw/ff/00index_e.html) 

### **Other Details and Future Work**
***
There is a weird bug whenever the directories and files are printed to the serial console. For some reason the names will get cut and they will not display properly. I am trying to fix this, but it is one of those bugs that will take days to track. If you can help with these I will appreciate it! :smiley:

The SPI communication interface is working without using interrupts, which is kind of bad. I plan to do an interrupt interface for SPI. Also it will be good to add DMA for files transfers between the MSP432 memory and FatFs file system. 
***
