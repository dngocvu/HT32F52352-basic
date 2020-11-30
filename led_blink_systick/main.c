#include "ht32f5xxxx_01.h"              // Device header

volatile uint32_t msTicks = 0;
uint8_t signal = 0,i;

void Delay(uint32_t delayTicks)
{
	uint32_t currentTicks;
	currentTicks = msTicks;
	while ((msTicks - currentTicks)< delayTicks);
}
void SysTick_Handler(void)
{
	// Increment counter necessary in Delay()
	msTicks++;
}
void EXTI0_1_IRQHandler(void) // Handle for EXTI_1
{	
	signal=12;
	HT_EXTI->EDGEFLGR |=(1<<1); // Clear interrupt flag of EXTI1
}

void EXTI2_3_IRQHandler(void) // Handle for EXTI_2
{
	signal=1;
	HT_EXTI->EDGEFLGR |=(1<<2); // Clear interrupt flag of EXTI2
}	
	
void EXTI4_15_IRQHandler(void) //  Handle for EXTI12
{
	signal=2;
	HT_EXTI->EDGEFLGR |= (1<<12); // Clear interrupt flag of EXTI12
}
void init(void)
{
	// Enable clock for Port C,B,D
	HT_CKCU -> AHBCCR |= (7 << 17);
	// Enable clock for EXTI
	HT_CKCU -> APBCCR0 |= (1 << 15);
	// Enable clock for AFIO
	HT_CKCU -> APBCCR0 |= (1 << 14);
	// Config as output
	HT_GPIOC -> DIRCR |= (3 << 14); // PC14/LED0;PC15/LED1
	HT_GPIOC -> DIRCR |= (1 << 1);
	//HT_GPIOC -> DOUTR = 0x0;  // initial value
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
	//Configure edge detection, de-bounce
	HT_EXTI->CFGR1 |= (4 << 28);	// Positive edge Trigger for EXT1
	HT_EXTI->CFGR2 |= (3 << 28);   // Positive edge Trigger for EXT2
	HT_EXTI->CFGR12 |= (3 << 28); // Positive edge Trigger for EXT12
	
	// Enable EXTI interrupt pins
	HT_EXTI->CR |= (3 << 1);  // Enable for EXTI1, EXTI2
	HT_EXTI->CR |= (1 << 12);  // Enable for EXTI12
	// Enable EXTI interrupts
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	// Initial Value for PORT C(out)
	HT_GPIOC->DOUTR = 0xFFFF;
	// Reset the Corresponding pins
	//HT_GPIOC->RR |= (3<<14);  //Reset LED0, LED1
	//HT_GPIOC->RR |= (1<<1);		//Reset LED2
}
int main(void)
{
	init();
	SysTick_Config(48000);
	while(1)
	{
			
			//Press Button1
			if(signal==1)
			{
				for(i=0;i<4;i++)
				{
					HT_GPIOC->SRR |= (1<<1);
					Delay(250);
					HT_GPIOC->RR |= (1<<1);
					Delay(250);
				}
				signal=0;
				HT_GPIOC->SRR |= (1<<1);
			}
			if(signal==2)
			{
				for(i=0;i<4;i++)
				{
					HT_GPIOC->SRR |= (1<<14);
					Delay(250);
					HT_GPIOC->RR |= (1<<14);
					Delay(250);
				}
				signal=0;
				HT_GPIOC->SRR |= (1<<14);
			}
			if(signal==12) 
			{
				for(i=0;i<4;i++)
				{
					HT_GPIOC->SRR |= (1<<15);
					Delay(250);
					HT_GPIOC->RR |= (1<<15);
					Delay(250);
				}
				signal=0;
				HT_GPIOC->SRR |= (1<<15);
			}
	}
}