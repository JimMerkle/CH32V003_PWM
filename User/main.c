/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 TIM1_CH1(PD2)
 PWM_MODE1: ccp defines active high pulse width
 PWM_MODE2: ccp defines active low pulse width

 For servo control, MODE1 is preferred.
   Loading larger values for ccp creates larger active high pulse width

*/

#include "debug.h"

/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
#define PWM_MODE PWM_MODE1
//#define PWM_MODE PWM_MODE2

/*********************************************************************
 * @fn      TIM1_OutCompare_Init
 *
 * @brief   Initializes TIM1 output compare.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

#if (PWM_MODE == PWM_MODE1)
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

#elif (PWM_MODE == PWM_MODE2)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_Cmd( TIM1, ENABLE );
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 * The following measured periods and active high times are for PWM_MODE2
 * TIM1_PWMOut_Init(100, 48000-1, 50) 202ms period, 100ms high
 * TIM1_PWMOut_Init( 100, 480-1, 50 ) 2.02ms period, 1.02ms high
 * TIM1_PWMOut_Init( 1000, 480-1, 50 ) 20.00ms period, 19ms high
 * TIM1_PWMOut_Init( 1000, 480-1, 900 ) 20.00ms period, 2.02ms high
 * TIM1_PWMOut_Init( 1000, 480-1, 925 ) 20.00ms period, 1.52ms high
 * TIM1_PWMOut_Init( 1000, 480-1, 926 ) 20.00ms period, 1.499ms high
 *
 * The following measured periods and active high times are for PWM_MODE1
 * TIM1_PWMOut_Init( 1000, 480-1, 75-1 ) 1.48ms high
 * TIM1_PWMOut_Init( 1000, 480-1, 75 ) 1.499ms high  - Center Servo - ccp units: 20us each
 * TIM1_PWMOut_Init( 1000, 480-1, 50 ) 999us high  - Servo Far Left
 * TIM1_PWMOut_Init( 1000, 480-1, 100 ) 1.99ms - Servo Far Right
 * TIM1_PWMOut_Init( 10000, 48-1, 750 ) 19.99ms period, 1.499ms high, 2us units for cpp
 *
 * TIM1_PWMOut_Init( 20000, 24-1, 1500 ) 19.99ms period, 1.499ms high, 1us units for ccp (BEST configuration!) Just change ccp for high time in micro seconds
 *
 * It appears something within this "application", has created an issue with Delay_Ms() function!
 */
int main(void)
{
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);

    TIM1_PWMOut_Init( 20000, 24-1, 1000 ); // 19.99ms period, 0.999ms high, 1us units for ccp

    // Read TIM1, CH1CVR value - display value (capture/compare channel 1 register)
    printf("TIM1->CH1CVR: %u\n",TIM1->CH1CVR);
    //Delay_Ms(5000); // this doesn't delay 5 seconds
    for(int i=0;i<500000;i++) Delay_Ms(1);
    TIM1->CH1CVR = 1500; // 1.5ms
    printf("TIM1->CH1CVR: %u\n",TIM1->CH1CVR);
    //Delay_Ms(5000);
    for(int i=0;i<500000;i++) Delay_Ms(1);
    TIM1->CH1CVR = 2000; // 2.0ms
    printf("TIM1->CH1CVR: %u\n",TIM1->CH1CVR);
    while(1);
}
