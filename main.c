#include "stm32l476xx.h"

#include "SysClock.h"
#include "UART.h"
#include "PWM.h"

#include <string.h>
#include <stdio.h>


void USART1_IRQHandler(){
	unsigned char buffer[32];
	uint32_t pc = 0;
	USART_IRQHandler(USART1, buffer, &pc);
	if(!pc)
		return;
	unsigned char rxByte = buffer[0];
	switch(rxByte){
			case 's':
				PWM_stop();
				printf("Stopping!");
				break;
			case 'f':
				PWM_forward();
				printf("Going forward!");
				break;
			case 'b':
				PWM_reverse();
				printf("Going backward!");
				break;
			case 'r':
				PWM_right();
				printf("Turning right!");
				break;
			case 'l':
				PWM_left();
				printf("Turning left!");
				break;
		}
	
}

void Init_USARTx(int part) {
	
	if(part == 1) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else if(part == 2) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	Init_USARTx(2);
	PWM_Init();
	while(1) {
	}
	
}
