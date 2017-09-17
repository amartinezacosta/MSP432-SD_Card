#include "GPIO_Driver.h"

void GPIO_Init(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setAsOutputPin(Port, Pins);
}

void GPIO_High(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setOutputHighOnPin(Port, Pins);
}

void GPIO_Low(uint32_t Port, uint16_t Pins)
{
    MAP_GPIO_setOutputLowOnPin(Port, Pins);
}
