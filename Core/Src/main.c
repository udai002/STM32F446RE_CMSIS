/*
 * main.c - STM32F446RE (NUCLEO-F446RE) bare-metal CMSIS
 *
 * After reset the core runs from the 16 MHz HSI (SystemCoreClock = 16000000).
 */
#include "stm32f4xx.h"
#include "gpio.h"

// typedef struct {
//     uint32_t ID;        // 11-bit or 29-bit identifier
//     uint8_t DLC;       // Data Length Code (0-8)
//     uint8_t data[8];   // Data bytes
//     uint32_t ide ;
//     uint32_t rtr ;
//     uint32_t data[8];
// } CAN_RXMessage;

// void initialise_GPIOA(void)
// {
//     /* Enable GPIOA clock */
//     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//     /* Set PA5 as output */
//     GPIOA->MODER &= ~(0x3 << (5 * 2)); // Clear mode bits for PA5
//     GPIOA->MODER |= (0x1 << (5 * 2));  // Set mode to output (01)
// }

// void initialise_CAN(void){

//     // Enable CAN1 clock and GPIOB clock
//     RCC->APB1ENR |= RCC_APB1ENR_CAN1EN; // Enable CAN1 clock
//     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock

//     GPIOB->MODER &= ~(0x3 << (8 * 2)); // Clear mode bits for PB8
//     GPIOB->MODER |= (0x2 << (8 * 2));

//     GPIOB->AFR[1] &= ~(0xF << ((8 - 8) * 4)); // Clear alternate function bits for PB8
//     GPIOB->AFR[1] |= (0x9 << ((8 - 8) * 4)); // Set alternate function to AF9 (CAN1_RX)

//     GPIOB->PUPDR &= ~(0x3 << (8 * 2)); // Clear pull-up/pull-down bits for PB8
//     GPIOB->PUPDR |= (0x1 << (8 * 2)); // Set pull-up resistor for PB8

//     GPIOB->MODER &= ~(0x3 << (9 * 2)); // Clear mode bits for PB9
//     GPIOB->MODER |= (0x2 << (9 * 2)); // Set mode to alternate function (10)
//     GPIOB->AFR[1] &= ~(0xF << ((9 - 8) * 4)); // Clear alternate function bits for PB9
//     GPIOB->AFR[1] |= (0x9 << ((9 - 8) * 4)); // Set alternate function to AF9 (CAN1_TX)

//     GPIOB->PUPDR &= ~(0x3 << (9 * 2)); // Clear pull-up/pull-down bits for PB9
//     GPIOB->PUPDR |= (0x1 << (9 * 2)); // Set pull-up resistor for PB9

//     CAN1->MCR |= (1U << 0); // Set INRQ bit to enter initialization mode
//     while (!(CAN1->MSR & (1U << 0))); // Wait until the initialization mode is entered  

//     // filter configuration
//     CAN1->FMR |= CAN_FMR_FINIT; // Enter filter initialization mode
//     CAN1->FM1R |= (1U << 0); // Set filter 0 to identifier list mode
//     CAN1->FS1R |= (1U << 0); // Set filter 0 to single 32-bit scale configuration
//     CAN1->FFA1R &= ~(1U << 0); // Assign filter 0 to FIFO 0
//     CAN1->sFilterRegister[0].FR1 = 0x00000000; // Set filter 0 to accept all IDs  
//     CAN1->sFilterRegister[0].FR2 = 0x00000000; // Set filter 0 to accept all IDs
//     CAN1->FA1R |= (1U << 0); // Activate filter 0
//     CAN1->FMR &= ~CAN_FMR_FINIT; // Exit filter initialization mode

//     //receiving the message from can 
//     if(((CAN1->RF0R) & (1U << 0)) == 0 ){
//         return; // No message pending in FIFO 0
//     }

//     CAN_RXMessage rxMessage;
//     rxMessage.ID = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF; // Extract the 11-bit identifier
//     rxMessage.DLC = (CAN1->sFIFOMailBox[0].RDTR) & 0x0F; // Extract the Data Length Code
//     rxMessage.ide = (CAN1->sFIFOMailBox[0].RIR >> 2) & 0x01; // Extract the IDE bit
//     rxMessage.rtr = (CAN1->sFIFOMailBox[0].RIR >> 1) & 0x01; // Extract the RTR bit
//     rxMessage.data[0] = (CAN1->sFIFOMailBox[0].RDLR) & 0xFF; // Extract the first data byte
//     rxMessage.data[1] = (CAN1->sFIFOMailBox[0].RDLR >> 8) & 0xFF; // Extract the second data byte
//     rxMessage.data[2] = (CAN1->sFIFOMailBox[0].RDLR >> 16) & 0xFF; // Extract the third data byte
//     rxMessage.data[3] = (CAN1->sFIFOMailBox[0].RDLR >> 24) & 0xFF; // Extract the fourth data byte
//     rxMessage.data[4] = (CAN1->sFIFOMailBox[0].RDHR) & 0xFF; // Extract the fifth data byte
//     rxMessage.data[5] = (CAN1->sFIFOMailBox[0].RDHR >> 8) & 0xFF; // Extract the sixth data byte
//     rxMessage.data[6] = (CAN1->sFIFOMailBox[0].RDHR >> 16) & 0xFF; // Extract the seventh data byte
//     rxMessage.data[7] = (CAN1->sFIFOMailBox[0].RDHR >> 24) & 0xFF; // Extract the eighth data byte      
    
// }

int main(void)
{

    gpio_init_output(GPIOA, 5); // Initialize PA5 as output
    gpio_init_output(GPIOB , 0); // Initialize PB0 as output
    gpio_write(GPIOA, 5 , GPIO_STATE_HIGH); // Set PA5 high
    gpio_write(GPIOB, 0 , GPIO_STATE_LOW); // Set PB0 low

    for (;;) {
    }
}
  