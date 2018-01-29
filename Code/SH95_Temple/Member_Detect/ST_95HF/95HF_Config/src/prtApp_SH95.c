/**
  ******************************************************************************
  * @file    main.c 
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    03/21/2013
  * @brief   Main program body
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
#include "prtApp_SH95.h"

extern ISO14443B_CARD 	ISO14443B_Card;
extern ISO14443A_CARD 	ISO14443A_Card;
extern uint8_t TagUID[16];
extern FELICA_CARD 	FELICA_Card;

extern ARM_DRIVER_USART Driver_USART1;

osThreadId tid_devApp95HF_Thread;
osThreadDef(devApp95HF_Thread,osPriorityNormal,	1,	2048);

/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b> 
 */
 
/** @addtogroup Main
 * 	@{
 * @brief      This file contains the entry point of this demonstration firmeware
 */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static __IO uint32_t TimingDelay = 0;

/* Variables for the different modes */
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;

/**
* @brief  buffer to exchange data with the RF tranceiver.
*/
extern uint8_t				u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];

__IO uint32_t SELStatus = 0;

bool 		USB_Control_Allowed = false;

extern int8_t	HIDTransaction;
/* bmp saved in well defined area, initialize pointer on this bmp file */
/* adress is forced in header file */
  
 /* Private function prototypes -----------------------------------------------*/
 
 
/** @addtogroup Main_Private_Functions
 * 	@{
 */

/**
  * @}
  */


/**
  * @brief  convert a ASCII coding byte to it's representing char.
  * @param  pDataIn : pointer on the byte array to translate
	* @param  NumberOfByte : the size of the array
	* @param  pString : pointer on the string created
  * @retval None
  */
  
void Hex2Char( u8* pDataIn, u16 NumberOfByte, char* pString )
{
	u8 data;
	uint8_t i=0;
	
	for(i=0; i<NumberOfByte; i++)
	{
		/* First char */
		data = (*pDataIn & 0xF0)>>4;
		if( data < 0x0A)
			*pString = data + 0x30;  /* ASCII offset for number */
		else
			*pString = data + 0x37; 	/* ASCII offset for letter */
	
		pString++;
	
		/* Second char */
		data = (*pDataIn & 0x0F);
		if( data < 0x0A)
			*pString = data + 0x30;  /* ASCII offset for number */
		else
			*pString = data + 0x37; 	/* ASCII offset for letter */
	
		pString++;
		pDataIn++;
	}
}

/** @addtogroup Main_Public_Functions
 * 	@{
 */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void USART1_callback(uint32_t event){

	;
}

void USART1_dev95HF_Init(void){

    /*Initialize the USART driver */
    Driver_USART1.Initialize(USART1_callback);
    /*Power up the USART peripheral */
    Driver_USART1.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 115200 Bits/sec */
    Driver_USART1.Control(ARM_USART_MODE_ASYNCHRONOUS |
                          ARM_USART_DATA_BITS_8 |
                          ARM_USART_PARITY_NONE |
                          ARM_USART_STOP_BITS_1 |
                          ARM_USART_FLOW_CONTROL_NONE, 115200);

    /* Enable Receiver and Transmitter lines */
    Driver_USART1.Control (ARM_USART_CONTROL_TX, 1);
    Driver_USART1.Control (ARM_USART_CONTROL_RX, 1);
	
    Driver_USART1.Send("i'm usart1 for Android\r\n", 25);
} 

void dev95HF_Init(void){
	
/* -- Configures Main system clocks & power */
	RCC_APB2PeriphClockCmd(USB_DISCONNECT_GPIO_CLOCK, ENABLE);
	  
/*------------------- Resources Initialization -----------------------------*/
	
	/* configure the interuptions  */
	Interrupts_Config();
	
/*------------------- Drivers Initialization -------------------------------*/
	USART1_dev95HF_Init();

  /* Led Configuration */
	LED_Config(LED1);	
	delay_ms(10);
		
	/* ST95HF HW Init */
	ConfigManager_HWInit();
  
	LED_On(LED1);
	delay_ms(400);
	LED_Off(LED1);
}
  
void prtApp_95HF(void)
{
	uint8_t TagType = TRACK_NOTHING, tagfounds=TRACK_ALL;
	
	u8 i;
	char UID[20] = {' '};
	char InvertedUID[20] = {' '};
	char LastUIDFound[20] = {' '};
	
	while (1){
		
      TagType = ConfigManager_TagHunting(tagfounds);
      // Turn on the LED if a tag was founded
		/* Check the tag type found */
 		if(TagType&TRACK_NFCTYPE1)
		{
			LED_On(LED1);
			Hex2Char( TagUID, 6, UID);
	
			for(i=0; i<12; i=i+2)
			{
				InvertedUID[11-i] = UID[i+1];
				InvertedUID[11-(i+1)] = UID[i];
			}
			memcpy(&InvertedUID[17],"TT1",3);
			/* Set the Text Color */
			if(memcmp (LastUIDFound, TagUID, 6))
			{	
				memcpy(LastUIDFound,TagUID,6);
				
				Driver_USART1.Send((uint8_t*)InvertedUID,strlen((const char*)InvertedUID));			
			}
			else
			{
				
			}
		}
		else if (TagType&TRACK_NFCTYPE2)
		{	
			LED_On(LED1);
			Hex2Char( ISO14443A_Card.UID, ISO14443A_Card.UIDsize, UID);
			
			memcpy(&UID[17],"TT2",3);
			if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
			{	
				memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
				
				Driver_USART1.Send((uint8_t*)UID,strlen((const char*)UID));
			}
			else
			{
				
			}
		}
		else if (TagType&TRACK_NFCTYPE3)
		{
			LED_On(LED1);
			Hex2Char( FELICA_Card.UID, 8 , UID);
			
			memcpy(&UID[17],"TT3",3);
			
			if(memcmp (LastUIDFound, FELICA_Card.UID, 8))
			{	
				memcpy(LastUIDFound,FELICA_Card.UID,8);
				
				Driver_USART1.Send((uint8_t*)UID,strlen((const char*)UID));			
			}
			else
			{
				
			}
		}
		else if (TagType&TRACK_NFCTYPE4A)
		{	
			LED_On(LED1);
			Hex2Char( ISO14443A_Card.UID, ISO14443A_Card.UIDsize, UID);
			
			memcpy(&UID[16],"TT4A",4);
			
			if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
			{	
				memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
				
				Driver_USART1.Send((uint8_t*)UID,strlen((const char*)UID));
			}
			else
			{
				
			}
		}
		else if (TagType&TRACK_NFCTYPE4B)
		{	
			LED_On(LED1);
			Hex2Char( ISO14443B_Card.PUPI, 4 , UID);
		
			memcpy(&UID[16],"TT4B",4);
	
			if(memcmp (LastUIDFound, ISO14443B_Card.PUPI, 4))
			{	
				memcpy(LastUIDFound,ISO14443B_Card.PUPI,4);		
				
				Driver_USART1.Send((uint8_t*)UID,strlen((const char*)UID));
			}
			else
			{
				
			}
		}
		else if (TagType&TRACK_NFCTYPE5)
		{
			LED_On(LED1);
			Hex2Char( TagUID, 8, UID);
			
			for(i=0; i<16; i=i+2)
			{
				InvertedUID[15-i] = UID[i+1];
				InvertedUID[15-(i+1)] = UID[i];
			}
			memcpy(&InvertedUID[17],"TT5",3);

			if(memcmp (LastUIDFound, TagUID, 8))
			{	
				memcpy(LastUIDFound,TagUID,8);		
				
				Driver_USART1.Send((uint8_t*)InvertedUID,strlen((const char*)InvertedUID));
			}
			else
			{
				
			}
		}
		else /* No supported tags found */
			LED_Off(LED1);
	
	delay_ms(20); 
  }
}

void devApp95HF_Thread(const void *argument){

	prtApp_95HF();
}

void devApp95HF_ThreadActive(void){

	dev95HF_Init();
	tid_devApp95HF_Thread = osThreadCreate(osThread(devApp95HF_Thread),NULL);
}

//void usr_fwdUARTInit(void){

//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA,ENABLE);    

//	/* Configure USART Tx as ouput alternate function push pull*/
//	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	/* Configure USART Rx as input floating */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure); 
//	
//	USART_InitStructure.USART_BaudRate 			= 115200;
//	USART_InitStructure.USART_WordLength 		= USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits 			= USART_StopBits_1;
//	USART_InitStructure.USART_Parity 			= USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;

//	/* Configure USART */
//	USART_Init(USART1, &USART_InitStructure);

//	/* Enable the USART */
//	USART_Cmd(USART1, ENABLE);
//}

//void forwardingPutChar(uint8_t Data){

//    USART_SendData(USART1, Data);  
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}  
//}

//void forwardingSend(uint8_t *pData ,uint8_t length){

//	uint8_t loop;
//	
//	for(loop = 0;loop < length;loop ++){
//	
//		forwardingPutChar(*pData ++);
//	}
//}

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
