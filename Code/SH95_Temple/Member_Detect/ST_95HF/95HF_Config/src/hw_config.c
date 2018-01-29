/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  MMY Application Team
  * @version V2.0.0
  * @date    06/12/2012
  * @brief   Hardware Configuration & Setup of the MCU
	* @brief   GPIO used to receive 95HF interruption is declared in this file
	******************************************************************************
  * @copyright
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b>
 */

/** @addtogroup HW_Config
 * 	@{
 * @brief      This file defines a set of command to initialize the MCU
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static __IO uint16_t					counter_delay_ms;

/* Private functions Prototype -----------------------------------------------*/
static void TimerDelay_us_Config	( void );
static void TimerDelay_ms_Config	( void );

#ifdef USE_MSD_DRIVE
static void IntToUnicode (uint32_t value, uint8_t *pbuf, uint8_t len);
#endif /*USE_MSD_DRIVE*/

/** @addtogroup HW_Config_Private_Functions
 * 	@{
 */

/**
 *	@brief Structure configuration for the Timer2 in ms
 *  @param  None
 *  @retval None
 */
static void TimerDelay_ms_Config( void )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* --------------------------------------------------------------------------
     * Delay TIMER configuration
     * -------------------------------------------------------------------------- */
    TIM_TimeBaseStructure.TIM_Period 			= TIMER_DELAY_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_DELAY_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMER_DELAY, &TIM_TimeBaseStructure);

    TIM_UpdateRequestConfig(TIMER_DELAY, TIM_UpdateSource_Global);

    TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);

    /* Enable TIMER Update interrupt */
    TIM_ITConfig(TIMER_DELAY, TIM_IT_Update, ENABLE);


    /* Disable timer	*/
    TIM_Cmd(TIMER_DELAY, DISABLE);
}

/**
 *	@brief Structure configuration for the Timer2 in us
 *  @param  None
 *  @retval None
 */
static void TimerDelay_us_Config( void )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* --------------------------------------------------------------------------
    * Delay TIMER configuration (us)
    * -------------------------------------------------------------------------- */
    TIM_TimeBaseStructure.TIM_Period 			= TIMER_US_DELAY_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_US_DELAY_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMER_US_DELAY, &TIM_TimeBaseStructure);

    TIM_UpdateRequestConfig(TIMER_US_DELAY, TIM_UpdateSource_Global);

    TIM_ClearITPendingBit(TIMER_US_DELAY, TIM_IT_Update);

    /* Enable TIMER Update interrupt */
    TIM_ITConfig(TIMER_US_DELAY, TIM_IT_Update, ENABLE);

    /* Disable timer	*/
    TIM_Cmd(TIMER_DELAY, DISABLE);

}

/**
 *	@brief  Configures the interrupts
 *  @param  None
 *  @retval None
 */
void Interrupts_Config (void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

#ifdef SPI_INTERRUPT_MODE_ACTIVATED
    /* Enable and set RF transceiver IRQ to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel 										= EXTI_RFTRANS_95HF_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= EXTI_RFTRANS_95HF_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= EXTI_RFTRANS_95HF_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif /* SPI_INTERRUPT_MODE_ACTIVATED */

    /* Enable and set TIMER IRQ used for timeout */
    NVIC_InitStructure.NVIC_IRQChannel 											= TIMER_TIMEOUT_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_TIMEOUT_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= TIMER_TIMEOUT_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable and set TIMER IRQ used for appli timeout */
    NVIC_InitStructure.NVIC_IRQChannel 											= APPLI_TIMER_TIMEOUT_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= APPLI_TIMER_TIMEOUT_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= APPLI_TIMER_TIMEOUT_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable and set TIMER IRQ used for delays */
    NVIC_InitStructure.NVIC_IRQChannel 					 						= TIMER_DELAY_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= TIMER_DELAY_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the EXTI9_5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel 											= EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 					= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel 											= EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 					= 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

#ifdef USE_MSD_DRIVE

/**
 *	@brief  This function creates the serial number string descriptor.
 */
void Get_SerialNum(void)
{

    uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

    Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
    Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
    Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

    Device_Serial0 += Device_Serial2;

    if (Device_Serial0 != 0)
    {
        IntToUnicode (Device_Serial0, &COMPOSITE_StringSerial[2], 6);
        IntToUnicode (Device_Serial1, &COMPOSITE_StringSerial[18], 6);
    }
}

/**
 *	@brief  Convert Hex 32Bits value into char.
 */
static void IntToUnicode (uint32_t value, uint8_t *pbuf, uint8_t len)
{
    uint8_t idx = 0;

    for( idx = 0 ; idx < len ; idx ++)
    {
        if( ((value >> 28)) < 0xA )
        {
            pbuf[ 2* idx] = (value >> 28) + '0';
        }
        else
        {
            pbuf[2* idx] = (value >> 28) + 'A' - 10;
        }

        value = value << 4;

        pbuf[ 2* idx + 1] = 0;
    }
}

/**
 *	@brief  MAL_layer configuration
 *  @param  None
 *  @return None
 */
void MAL_Config(void)
{
    MAL_Init(0);
}
#endif /*USE_MSD_DRIVE*/


/**
 *	@brief  This function configurates the disconnects pin
 *  @param  None
 *  @retval None
 */
void USB_Disconnect_Config(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable USB_DISCONNECT GPIO clock */
    RCC_APB2PeriphClockCmd(USB_DISCONNECT_GPIO_CLOCK, ENABLE);

//    /* USB_DISCONNECT_PIN used as USB pull-up */
//    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
}

/**
 *	@brief  This function configures the Timers
 *  @param  None
 *  @retval None
 */
void Timer_Config( void )
{
    Timer_RCC_Config( );
    Timer_Structure_Config( );
}

/**
 *	@brief  This function configures RCC for the Timers
 *  @param  None
 *  @retval None
 */
void Timer_RCC_Config( void )
{
    /*	enable TIM2 & TIM3 & TIM4 */
    RCC_APB1PeriphClockCmd(		TIMER_TIMEOUT_CLOCK 	|
                                APPLI_TIMER_TIMEOUT_CLOCK 	|
                                TIMER_DELAY_CLOCK 		,
                                ENABLE);
}

/**
 *	@brief  Structure configuration for the Timers
 *  @param  None
 *  @retval None
 */
void Timer_Structure_Config( void )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM3 driver timeout */
    drvInt_TimeoutTimerConfig();

    /* TIM4 appli timeout */
    drvInt_AppliTimeoutTimerConfig();

    /* --------------------------------------------------------------------------
     * Delay TIMER configuration
     * --------------------------------------------------------------------------
     * 72 MHz / 72 = 1MHz (1�s)
     * 1�s * 1000 + 1�s ~= 1ms
     * -------------------------------------------------------------------------- */
    TIM_TimeBaseStructure.TIM_Period 			= TIMER_DELAY_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler 		= (TIMER_DELAY_PRESCALER-1);
    TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMER_DELAY, &TIM_TimeBaseStructure);

    TIM_UpdateRequestConfig(TIMER_DELAY, TIM_UpdateSource_Global);

    TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);

    /* Enable TIMER Update interrupt */
    TIM_ITConfig(TIMER_DELAY, TIM_IT_Update, ENABLE);
}

/**
 *	@brief  Time delay in millisecond
 *  @param  delay : delay in ms.
 *  @retval none
 */
void delay_ms(uint16_t delay)
{
    counter_delay_ms = (delay+1);

    TimerDelay_ms_Config ();

    TIM_SetCounter(TIMER_DELAY, 0);
    /* TIM2 enable counter */
    TIM_Cmd(TIMER_DELAY, ENABLE);
    /* Wait for 'delay' milliseconds */
    while(counter_delay_ms != 0);
    /* TIM2 disable counter */
    TIM_Cmd(TIMER_DELAY, DISABLE);
}


/**
 *	@brief  Time delay in microsecond
 *  @param  delay : delay in us.
 *  @retval none
 */
void delay_us(uint16_t delay)
{
    counter_delay_ms = delay;

    TimerDelay_us_Config ();

    TIM_SetCounter(TIMER_US_DELAY, 0);
    /* TIM2 enable counter */
    TIM_Cmd(TIMER_US_DELAY, ENABLE);
    /* Wait for 'delay' us */
    while(counter_delay_ms != 0);
    /* TIM2 disable counter */
    TIM_Cmd(TIMER_US_DELAY, DISABLE);
}



/**
 *	@brief 	Time delay in millisecond. The default priority are changed for this function.
 * 					This function is called by reset function to guarantee the execution time
 *  @param  delay : delay in ms.
 *  @retval none
 */
void delayHighPriority_ms(uint16_t delay)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    counter_delay_ms = (delay+1);

    /* Enable and set TIMER IRQ used for delays. High priority*/
    NVIC_InitStructure.NVIC_IRQChannel 					 						= TIMER_DELAY_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_HIGHPRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 		= TIMER_DELAY_SUB_HIGHPRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									=	 ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);
    TIM_SetCounter(TIMER_DELAY, 0);
    /* TIM2 enable counter */
    TIM_Cmd(TIMER_DELAY, ENABLE);
    /* Wait for 'delay' milliseconds */
    while(counter_delay_ms != 0);
    /* TIM2 disable counter */
    TIM_Cmd(TIMER_DELAY, DISABLE);

    /* Enable and set TIMER IRQ used for delays. Default priority */
    NVIC_InitStructure.NVIC_IRQChannel 					 						= TIMER_DELAY_IRQ_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 			= TIMER_DELAY_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd 									=	 ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 *	@brief  This function decrements the counter every millisecond used by the function delay_ms
 *  @param  None
 *  @retval None
 */
void decrement_delay(void)
{
    if(counter_delay_ms != 0)
    {
        /* Decrements the counter */
        counter_delay_ms--;
    }
}

/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/