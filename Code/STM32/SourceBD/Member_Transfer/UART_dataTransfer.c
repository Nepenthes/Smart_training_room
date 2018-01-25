#include "UART_dataTransfer.h"

osThreadId tid_com1ToZigbee_Thread;
osThreadId tid_com3ToDev485_Thread;

osThreadDef(com1ToZigbee_Thread,osPriorityNormal,1,1024);
osThreadDef(com3ToDev485_Thread,osPriorityNormal,1,1024);

static void USART1_callback(uint32_t event){

	;
}

static void USART3_callback(uint32_t event){

	;
}

static void USART1_toZigbee_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

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
	
	con485 = 1;
	
    Driver_USART1.Send("i'm usart1 for Zigebee\r\n", 25);
}

static void USART3_toDev485_Init(void){

    /*Initialize the USART driver */
    Driver_USART3.Initialize(USART3_callback);
    /*Power up the USART peripheral */
    Driver_USART3.PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 115200 Bits/sec */
    Driver_USART3.Control(ARM_USART_MODE_ASYNCHRONOUS |
                          ARM_USART_DATA_BITS_8 |
                          ARM_USART_PARITY_NONE |
                          ARM_USART_STOP_BITS_1 |
                          ARM_USART_FLOW_CONTROL_NONE, 115200);

    /* Enable Receiver and Transmitter lines */
    Driver_USART3.Control (ARM_USART_CONTROL_TX, 1);
    Driver_USART3.Control (ARM_USART_CONTROL_RX, 1);
	
    Driver_USART3.Send("i'm usart3 for Dev485\r\n", 22);
}

static void com1ToZigbee_Thread(const void *argument){
	
	for(;;){
	
		datsZigbeeTX("abc",3);
		osDelay(1000);
	}
}

static void com3ToDev485_Thread(const void *argument){

	for(;;){
	
		datsDev485TX("abc",3);
		osDelay(1000);
	}
}

void communicationActive(uint8_t comObj){

	switch(comObj){
		
		case comObj_Dev485:		USART1_toZigbee_Init();
								tid_com1ToZigbee_Thread = osThreadCreate(osThread(com1ToZigbee_Thread),NULL);
								break;
				
		case comObj_Zigbee:		USART3_toDev485_Init();
								tid_com3ToDev485_Thread = osThreadCreate(osThread(com3ToDev485_Thread),NULL);
								break;
		
				   default:		break;
	}	
}



