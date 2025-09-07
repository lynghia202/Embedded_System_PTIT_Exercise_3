#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"

#define LED_1 GPIO_Pin_0
#define LED_2 GPIO_Pin_1
#define NUT 	GPIO_Pin_2

void delay(uint32_t ms) {
    volatile uint32_t n;
    for(uint32_t i = 0; i < ms; i++) {
        n = 7200; 
        while(n--) {
            __NOP(); 
        }
    }
}

void GPIO_Config(void) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitTypeDef gpio;
		gpio.GPIO_Pin = LED_1 | LED_2;
		gpio.GPIO_Mode = GPIO_Mode_Out_PP;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &gpio);
		
		gpio.GPIO_Pin = NUT;
		gpio.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOA,&gpio);
}

void EXTI_Config(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
		EXTI_InitTypeDef exti;
		exti.EXTI_Line = EXTI_Line2;
		exti.EXTI_Mode = EXTI_Mode_Interrupt;
		exti.EXTI_Trigger = EXTI_Trigger_Rising;
		exti.EXTI_LineCmd = ENABLE;
		EXTI_Init(&exti);
		
		NVIC_InitTypeDef nvic;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority = 0;
		nvic.NVIC_IRQChannel = EXTI2_IRQn;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
}

void EXTI2_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line2) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line2);
        for (volatile int i = 0; i < 50000; i++); 
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) {
            GPIOA->ODR ^= LED_1; 
        }
    }
}

int main (void){
	GPIO_Config();
	EXTI_Config();
	GPIO_ResetBits(GPIOA, LED_1 | LED_2);
	while(1){
		GPIO_WriteBit(GPIOA,LED_2,1);
		delay(500);
		GPIO_WriteBit(GPIOA,LED_2,0);
		delay(500);
	}
}