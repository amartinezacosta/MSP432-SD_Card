#include <Hardware/SPI_Driver.h>
#include <Hardware/GPIO_Driver.h>
#include <Hardware/CS_Driver.h>
#include <Hardware/TIMERA_Driver.h>
#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include <Devices/MSPIO.h>

/* MISO = P1.7
 * MOSI = P1.6
 * SCK = P1.5
 * CS = P5.0
 * VCC = 3.3V
 * GND = GND
 */

/*Huge thanks to bluehash @ https://github.com/bluehash/MSP432Launchpad/tree/master/MSP432-Launchpad-FatFS-SDCard*/

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

FATFS FS;
DIR DI;
FILINFO FI;

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

    FRESULT r

    /*First we should mount the SD Card into the Fatfs file system*/
    r = f_mount(&FS, "0", 1);
    /*Check for errors. Trap MSP432 if there is an error*/
    if(r != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error mounting SD Card, check your connections\r\n");
        while(1);
    }

    /*Let's try to open the root directory on the SD Card*/
    r = f_opendir(&DI, "/");
    /*Check for errors. Trap MSP432 if there is an error*/
    if(r != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Could not open root directory\r\n");
        while(1);
    }

    /*Read everything inside the root directory*/
    do
    {
        /*Read a directory/file*/
        r = f_readdir(&DI, &FI);
        /*Check for errors. Trap MSP432 if there is an error*/
        if(r != FR_OK)
        {
           MSPrintf(EUSCI_A0_BASE, "Error reading file/directory\r\n");
           while(1);
        }

        /*Print the file to the serial terminal*/
        MSPrintf(EUSCI_A0_BASE, "%c%c%c%c%c %s\r\n",
               (FI.fattrib & AM_DIR) ? 'D' : '-',
               (FI.fattrib & AM_RDO) ? 'R' : '-',
               (FI.fattrib & AM_HID) ? 'H' : '-',
               (FI.fattrib & AM_SYS) ? 'S' : '-',
               (FI.fattrib & AM_ARC) ? 'A' : '-',
               ((char*)FI.fname));

    }while(FI.fname[0]);

    while(1)
    {

    }
	
}


