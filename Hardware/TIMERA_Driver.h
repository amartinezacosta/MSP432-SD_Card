#ifndef HARDWARE_TIMERA_DRIVER_H_
#define HARDWARE_TIMERA_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>

#define CONTINUOS_MODE  0
#define UP_MODE         1
#define UPDOWN_MODE     2
#define CAPTURE_MODE    3
#define COMPARE_MODE    4
#define PWM_MODE        5

void TIMERA_Init(uint32_t TIMER, uint32_t Mode, void *Config, void(*TIMER_CB)(void));


#endif /* HARDWARE_TIMERA_DRIVER_H_ */
