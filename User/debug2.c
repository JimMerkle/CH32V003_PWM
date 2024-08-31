/********************************** (C) COPYRIGHT *******************************
 * File Name          : debug2.c
 * Author             : Jim Merkle
 * Version            : V1.0.0
 * Date               : 2024/08/22
 * Description        : Implement alternate debug.c functionality.
 *                    : This is a supplement to debug.c
 * Copyright (c) 2024 Jim Merkle
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include <debug.h>

/*********************************************************************
 * @fn      USART_Printf_Init2
 *
 * @brief   Initializes the USARTx peripheral for both TX and RX.
 *
 * @param   baudrate - USART communication baud rate.
 *
 * @return  None
 */
void USART_Printf_Init2(uint32_t baudrate)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    // USART TX - output pin: D5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // USART RX - input pin: D6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

/*********************************************************************
 * @fn      USART_ReadByte
 *
 * @brief   Check UART1 for Receive Not Empty
 *          If no data available, return EOF, else, return data byte.
 *
 * @return  EOF (-1) : No character available
 *          else 8-bit serial character read from UART
 */
__attribute__((used))
int USART_ReadByte(void)
{
    if( USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
        return USART_ReceiveData(USART1);
    else
        return EOF;
}

