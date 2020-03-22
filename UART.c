#include "UART.h"

void UART1_Init(void) {
	//enable USART2 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 
	//select system clock as clock source
	RCC->CCIPR &= ~(RCC_CCIPR_USART1SEL);
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART2_Init(void){
	//enable USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; 
	//select system clock as clock source
	RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART1_GPIO_Init(void) {
	//enable clock for port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	//set to AF mode
	GPIOB->MODER &= ~(GPIO_MODER_MODE7);
	GPIOB->MODER &= ~(GPIO_MODER_MODE6);
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	//enable AFR
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL7);
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL6);
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_2);
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_2);
	//high speed GPIO
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;
	//GPIO push pull
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT7);
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT6);
	//pull up
	GPIOB->PUPDR &= GPIO_PUPDR_PUPD7;
	GPIOB->PUPDR &= GPIO_PUPDR_PUPD6;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;
}

void UART2_GPIO_Init(void){
	//enable clock for port D
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	//set to AF mode
	GPIOD->MODER &= ~(GPIO_MODER_MODE5);
	GPIOD->MODER &= ~(GPIO_MODER_MODE6);
	GPIOD->MODER |= GPIO_MODER_MODE5_1;
	GPIOD->MODER |= GPIO_MODER_MODE6_1;
	//enable AFR
	GPIOD->AFR[0] &= ~(GPIO_AFRL_AFRL5);
	GPIOD->AFR[0] &= ~(GPIO_AFRL_AFRL6);
	GPIOD->AFR[0] |= (GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_1 | GPIO_AFRL_AFSEL5_2);
	GPIOD->AFR[0] |= (GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_2);
	//high speed GPIO
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;
	//GPIO push pull
	GPIOD->OTYPER &= ~(GPIO_OTYPER_OT5);
	GPIOD->OTYPER &= ~(GPIO_OTYPER_OT6);
	//pull up
	GPIOD->PUPDR &= GPIO_PUPDR_PUPD5;
	GPIOD->PUPDR &= GPIO_PUPDR_PUPD6;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD5_0;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPD6_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	//disable USART
	USARTx->CR1 &= ~(USART_CR1_UE);
	//set word length to 8 bits, oversampling to over by 16, stop bits 1
	USARTx->CR1 &= ~(USART_CR1_M1);
	USARTx->CR1 &= ~(USART_CR1_OVER8);
	USARTx->CR2 &= ~(USART_CR2_STOP);
	//Baud rate 9600
	USARTx->BRR = 8333U;
	//enable tx rx
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	//enable interrupts
	USARTx->CR1 |= USART_CR1_RXNEIE;
	//enable USART
	USARTx->CR1 |= USART_CR1_UE;
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART2_IRQn, 0);
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}

void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter) {
	if(USARTx->ISR & USART_ISR_RXNE) {						// Received data                         
		buffer[*pRx_counter] = USARTx->RDR;         // Reading USART_DR automatically clears the RXNE flag 
		(*pRx_counter)++;  
		if((*pRx_counter) >= BufferSize )  {
			(*pRx_counter) = 0;
		}
	} else if(USARTx->ISR & USART_ISR_TXE) {
 		USARTx->ISR &= ~USART_ISR_TXE;            // clear interrupt 
		//Tx1_Counter++;
	} else if(USARTx->ISR & USART_ISR_ORE) {			// Overrun Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// Parity Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// USART_ISR_FE	
		while(1);
	} else if (USARTx->ISR & USART_ISR_NE){ 			// Noise Error Flag
		while(1);     
	}	
}
