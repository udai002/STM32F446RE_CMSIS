/*
 * gpio.h - GPIO driver for STM32F446RE (CMSIS register access)
 *
 * Usage:
 *   gpio_init_output(GPIOA, 5);                   // LD2 on Nucleo
 *   gpio_init_input(GPIOC, 13, GPIO_PULL_NONE);   // B1 on Nucleo (active low)
 *   gpio_write(GPIOA, 5, GPIO_STATE_HIGH);
 *   if (gpio_read(GPIOC, 13) == GPIO_STATE_LOW) { ... }
 */
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f4xx.h"

/* MODER: 2 bits per pin */
typedef enum {
    GPIO_MODE_INPUT  = 0U,
    GPIO_MODE_OUTPUT = 1U,
    GPIO_MODE_AF     = 2U,      /* alternate function (CAN, TIM, UART, ...) */
    GPIO_MODE_ANALOG = 3U
} gpio_mode_t;

/* OTYPER: 1 bit per pin */
typedef enum {
    GPIO_OTYPE_PUSH_PULL  = 0U,
    GPIO_OTYPE_OPEN_DRAIN = 1U
} gpio_otype_t;

/* OSPEEDR: 2 bits per pin */
typedef enum {
    GPIO_SPEED_LOW       = 0U,
    GPIO_SPEED_MEDIUM    = 1U,
    GPIO_SPEED_FAST      = 2U,
    GPIO_SPEED_VERY_HIGH = 3U
} gpio_speed_t;

/* PUPDR: 2 bits per pin */
typedef enum {
    GPIO_PULL_NONE = 0U,
    GPIO_PULL_UP   = 1U,
    GPIO_PULL_DOWN = 2U
} gpio_pull_t;

typedef enum {
    GPIO_STATE_LOW  = 0U,
    GPIO_STATE_HIGH = 1U
} gpio_state_t;

/* Full pin configuration, used by gpio_init() */
typedef struct {
    GPIO_TypeDef *port;     /* GPIOA .. GPIOH */
    uint8_t       pin;      /* 0 .. 15 */
    gpio_mode_t   mode;
    gpio_otype_t  otype;
    gpio_speed_t  speed;
    gpio_pull_t   pull;
    uint8_t       af;       /* 0 .. 15, only used when mode == GPIO_MODE_AF */
} gpio_config_t;

/* Clock */
void gpio_clock_enable(GPIO_TypeDef *port);

/* Configuration (all of these enable the port clock themselves) */
void gpio_init(const gpio_config_t *cfg);
void gpio_init_output(GPIO_TypeDef *port, uint8_t pin);
void gpio_init_input(GPIO_TypeDef *port, uint8_t pin, gpio_pull_t pull);
void gpio_init_af(GPIO_TypeDef *port, uint8_t pin, uint8_t af,
                  gpio_otype_t otype, gpio_pull_t pull);

/* Pin access */
void         gpio_write(GPIO_TypeDef *port, uint8_t pin, gpio_state_t state);
void         gpio_toggle(GPIO_TypeDef *port, uint8_t pin);
gpio_state_t gpio_read(GPIO_TypeDef *port, uint8_t pin);

#endif /* GPIO_H */
