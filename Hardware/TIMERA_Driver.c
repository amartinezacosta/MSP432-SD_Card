#include "TIMERA_Driver.h"

void (*TIMER_A0_CB)(void) = 0x0000000;
void (*TIMER_A1_CB)(void) = 0x0000000;

void TIMERA_Init(uint32_t TIMER, uint32_t Mode, void *Config, void(*TIMER_CB)(void))
{
    Timer_A_ContinuousModeConfig *cmc;
    Timer_A_UpModeConfig *upc;
    Timer_A_UpDownModeConfig *udmc;

    switch(Mode)
    {
    case CONTINUOS_MODE:
        cmc = (Timer_A_ContinuousModeConfig*)Config;
        MAP_Timer_A_configureContinuousMode(TIMER, cmc);
        break;
    case UP_MODE:
        upc = (Timer_A_UpModeConfig*)Config;
        MAP_Timer_A_configureUpMode(TIMER, upc);
        break;
    case UPDOWN_MODE:
        udmc = (Timer_A_UpDownModeConfig*)Config;
        MAP_Timer_A_configureUpDownMode(TIMER, udmc);
        break;
    case CAPTURE_MODE:
        break;
    case COMPARE_MODE:
        break;
    case PWM_MODE:
        break;
    default:
        break;
    }

    if(TIMER_CB)
    {
        switch(TIMER)
        {
        case TIMER_A0_BASE:
            TIMER_A0_CB = TIMER_CB;
            MAP_Interrupt_enableInterrupt(INT_TA0_0);
            MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
            break;
        case TIMER_A1_BASE:
            TIMER_A1_CB = TIMER_CB;
            MAP_Interrupt_enableInterrupt(INT_TA1_0);
            MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
            break;
        default:
            break;
        }
    }
}

void TA0_0_IRQHandler(void)
{
    if(TIMER_A0_CB)
    {
        TIMER_A0_CB();
    }
}

void TA1_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
               TIMER_A_CAPTURECOMPARE_REGISTER_0);
    if(TIMER_A1_CB)
    {
        TIMER_A1_CB();
    }
}

