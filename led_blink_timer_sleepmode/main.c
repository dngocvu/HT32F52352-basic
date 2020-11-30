#include "ht32f5xxxx_01.h"              // Device header


uint8_t signal = 0,i;


// BFTM INTERRUPT
void BFTM0_IRQHandler(void)
{
	// set default 									
	HT_BFTM0->CNTR = 0;	  				// Reset counter
	HT_GPIOC -> DOUTR =~ HT_GPIOC -> DOUTR;
	HT_BFTM0 -> SR = 0;					// clear the BFTM interrupt flag
}
//// EXTERNAL INTERRUPT
void EXTI0_1_IRQHandler(void) // Handle for EXTI_1
{	
	HT_GPIOC->RR |= (1<<1);
	HT_GPIOC->RR |= (1<<14);
	HT_GPIOC->RR |= (1<<15);
	HT_BFTM0->CR |= (1<<2);	// enable timer
	HT_EXTI->EDGEFLGR |=(1<<1); // Clear interrupt flag of EXTI1
}

int main(void)
{
	// Enable clock for Port C,B,D
	HT_CKCU -> AHBCCR |= (7 << 17);
	// Enable clock for BFTM0
	HT_CKCU->APBCCR1 |= (1<<16);
	// Enable clock for EXTI
	HT_CKCU -> APBCCR0 |= (1 << 15);
	// Enable clock for AFIO
	HT_CKCU -> APBCCR0 |= (1 << 14);
	// Config as output
	HT_GPIOC -> DIRCR |= (3 << 14); // PC14/LED0;PC15/LED1
	HT_GPIOC -> DIRCR |= (1 << 1);
	HT_GPIOC -> DOUTR = 0xFFFF;  // initial value
	// Set up AF1 for PB12
	HT_AFIO -> GPBCFGR[1] |= (1 << 16);
	// Config as input
	HT_GPIOD -> DIRCR &=~ (3 << 1);
	HT_GPIOB -> DIRCR &=~ (1 << 12);
	HT_GPIOD->INER = 0xFFFF;	// initial value
	HT_GPIOB->INER = 0xFFFF;	// initial value
	//Assign EXTI function to GPIO pins
	HT_AFIO->ESSR[0] |= (3 << 4);  // PD1-Button1-EXTI1
	HT_AFIO->ESSR[0] |= (3 << 8);	 // PD2-Button2-EXTI2
	HT_AFIO->ESSR[1] |= (1 << 16); // PB12-WakeUp-EXTI12
	//Configure rising-edge detection
	HT_EXTI->CFGR1 |= (4 << 28);	// Positive edge Trigger for EXT1
	HT_EXTI->CFGR2 |= (3 << 28);   // Positive edge Trigger for EXT2
	HT_EXTI->CFGR12 |= (3 << 28); // Positive edge Trigger for EXT12
	// Enable EXTI interrupt pins
	HT_EXTI->CR |= (3 << 1);  // Enable for EXTI1, EXTI2
	HT_EXTI->CR |= (1 << 12);  // Enable for EXTI12
	
	// Select BFTM APB prescaler
	HT_CKCU->APBPCSR0 |= (3<<12); // PCLK = CK_AHB/8 = 6MHz
	
	// Configure BFTM timer: counter, reload value, enable timer, interrupt
	HT_BFTM0->CNTR = 0;						// reset counter
	HT_BFTM0->CMP = 24000000;     // 2s timer
	HT_BFTM0->CR = 3;						// disable timer, interrupt, repetitive mode
	

	
	// Enable EXTI interrupts
	NVIC_EnableIRQ(EXTI0_1_IRQn);
//	NVIC_EnableIRQ(EXTI2_3_IRQn);
//	NVIC_EnableIRQ(EXTI4_15_IRQn);
	NVIC_EnableIRQ(BFTM0_IRQn);
	////////
	NVIC_SetPriority(EXTI0_1_IRQn, 1);
	NVIC_SetPriority(BFTM0_IRQn, 0);
	//
	SCB -> SCR |= (1<<1);
	SCB -> SCR |= (1<<4);
	//
	__WFI();
	//
	HT_GPIOC->RR |= (1<<1);
	HT_GPIOC->RR |= (1<<14);
	HT_GPIOC->RR |= (1<<15);
	while(1)
	{
//			if(signal==1)
//			{
//				signal=0;
//				HT_GPIOC->SRR |= (1<<1);
//			}
//			if(signal==2)
//			{
//				signal=0;
//				HT_GPIOC->SRR |= (1<<14);
//			}
//			if(signal==12) 
//			{
//				signal=0;
//				HT_GPIOC->SRR |= (1<<15);
//			}
//			if(signal==22) 
//			{
//				HT_GPIOC->RR |= (1<<1);
//				HT_GPIOC->RR |= (1<<14);
//				HT_GPIOC->RR |= (1<<15);
//				signal=0;
//			}
//			
		// empty
	}
}