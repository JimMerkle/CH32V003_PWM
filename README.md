# PWM_Output

Example PWM project for the CH32V003

Implement an example PWM project that creates PWM waveforms to drive a servo

### Getting Started

        The base code provided for the nanoCH32V003 provides a good
        starting point for a project.
        To better use this base code, the developer needs to define
        their clock source and if appropriate, their USART pin
        configuration.
        1) File: system_ch32v00x.c, define clock source by uncommenting
        the appropriate #define.
        In my case, since my development board had no external crystal,
        I chose the following for HSI, 48MHz:
        
        //#define SYSCLK_FREQ_8MHz_HSI    8000000
        //#define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE
        #define SYSCLK_FREQ_48MHZ_HSI   48000000
        //#define SYSCLK_FREQ_8MHz_HSE    8000000
        //#define SYSCLK_FREQ_24MHz_HSE   HSE_VALUE
        //#define SYSCLK_FREQ_48MHz_HSE   48000000
        
        2) File: debug.h, Define debug USART pins
        I chose no remapping
        /* UART Printf Definition */
        #define DEBUG_UART1_NoRemap   1  //Tx-PD5  Rx-PD6
        #define DEBUG_UART1_Remap1    2  //Tx-PD0  Rx-PD1
        #define DEBUG_UART1_Remap2    3  //Tx-PD6  Rx-PD5
        #define DEBUG_UART1_Remap3    4  //Tx-PC0  Rx-PC1
        
        /* DEBUG USART Definition */
        #ifndef DEBUG
        #define DEBUG   DEBUG_UART1_NoRemap
        #endif


### TIM1_PWMOut_Init()
        
        Using TIM1_PWMOut_Init, generate a servo control waveform.
        20ms period
        1.0ms to 2.0ms high time


