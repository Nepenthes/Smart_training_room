#include "RelayCon.h"

extern ARM_DRIVER_USART Driver_USART1;		//设备驱动库串口一设备声明

osThreadId tid_RelayCon_Thread;
osThreadDef(RelayCon_Thread,osPriorityNormal,1,1024);

static void RelayCon_spiInit(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	relayMR 	= 1;
	relaySTCP	= 0;
	relaySHCP	= 0;
	relayDIN	= 0;
}

static void RelayCon_keyInit(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void RelayCon_Init(void){

	RelayCon_keyInit();
	RelayCon_spiInit();
}

void RelayDatsIn595(uint16_t Dats){

	uint8_t loop;
	
	relayMR	  = 0;
	osDelay(10);
	relayMR	  = 1;
	
	relaySTCP = 0;
	
	for(loop = 0;loop < 16;loop ++){
	
		relaySHCP	= 0;
		relayDIN	= Dats & 0x0001;
		relaySHCP	= 1;
		Dats	  	= Dats>>1;
	}
	
	relaySTCP = 1;
	osDelay(10);
	relaySTCP = 0;
}

void RelayCon_Thread(const void *argument){
	
	for(;;){
		
		RelayDatsIn595(0xffc0);
		osDelay(100);
		while(1);
	}
}

void RelayConThread_Active(void){

	RelayCon_Init();
	tid_RelayCon_Thread = osThreadCreate(osThread(RelayCon_Thread),NULL);
}
