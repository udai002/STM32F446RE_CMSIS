/*
 * main.c - STM32F446RE (NUCLEO-F446RE) bare-metal CMSIS
 *
 * After reset the core runs from the 16 MHz HSI (SystemCoreClock = 16000000).
 */
#include "stm32f4xx.h"

typedef struct {
    uint32_t ID;        // 11-bit or 29-bit identifier
    uint8_t DLC;       // Data Length Code (0-8)
    uint8_t data[8];   // Data bytes
    uint32_t ide ;
    uint32_t rtr ;
    uint32_t data[8];
} CAN_RXMessage;

void initialise_GPIOA(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set PA5 as output */
    GPIOA->MODER &= ~(0x3 << (5 * 2)); // Clear mode bits for PA5
    GPIOA->MODER |= (0x1 << (5 * 2));  // Set mode to output (01)
}

void initialise_CAN(void){

    // Enable CAN1 clock and GPIOB clock
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN; // Enable CAN1 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock

    GPIOB->MODER &= ~(0x3 << (8 * 2)); // Clear mode bits for PB8
    GPIOB->MODER |= (0x2 << (8 * 2));

    GPIOB->AFR[1] &= ~(0xF << ((8 - 8) * 4)); // Clear alternate function bits for PB8
    GPIOB->AFR[1] |= (0x9 << ((8 - 8) * 4)); // Set alternate function to AF9 (CAN1_RX)

    GPIOB->PUPDR &= ~(0x3 << (8 * 2)); // Clear pull-up/pull-down bits for PB8
    GPIOB->PUPDR |= (0x1 << (8 * 2)); // Set pull-up resistor for PB8

    GPIOB->MODER &= ~(0x3 << (9 * 2)); // Clear mode bits for PB9
    GPIOB->MODER |= (0x2 << (9 * 2)); // Set mode to alternate function (10)
    GPIOB->AFR[1] &= ~(0xF << ((9 - 8) * 4)); // Clear alternate function bits for PB9
    GPIOB->AFR[1] |= (0x9 << ((9 - 8) * 4)); // Set alternate function to AF9 (CAN1_TX)

    GPIOB->PUPDR &= ~(0x3 << (9 * 2)); // Clear pull-up/pull-down bits for PB9
    GPIOB->PUPDR |= (0x1 << (9 * 2)); // Set pull-up resistor for PB9

    CAN1->MCR |= (1U << 0); // Set INRQ bit to enter initialization mode
    while (!(CAN1->MSR & (1U << 0))); // Wait until the initialization mode is entered  

    // filter configuration
    CAN1->FMR |= CAN_FMR_FINIT; // Enter filter initialization mode
    CAN1->FM1R |= (1U << 0); // Set filter 0 to identifier list mode
    CAN1->FS1R |= (1U << 0); // Set filter 0 to single 32-bit scale configuration
    CAN1->FFA1R &= ~(1U << 0); // Assign filter 0 to FIFO 0
    CAN1->sFilterRegister[0].FR1 = 0x00000000; // Set filter 0 to accept all IDs  
    CAN1->sFilterRegister[0].FR2 = 0x00000000; // Set filter 0 to accept all IDs
    CAN1->FA1R |= (1U << 0); // Activate filter 0
    CAN1->FMR &= ~CAN_FMR_FINIT; // Exit filter initialization mode


    

    
}

int main(void)
{
    initialise_GPIOA();
    initialise_CAN();
    for (;;) {
    }
}
