/*
 * gpio.c - GPIO driver for STM32F446RE (CMSIS register access)
 *
 * Register map per port (RM0390 section 7.4):
 *   MODER   2 bits/pin  input / output / AF / analog
 *   OTYPER  1 bit/pin   push-pull / open-drain
 *   OSPEEDR 2 bits/pin  output speed
 *   PUPDR   2 bits/pin  pull-up / pull-down
 *   IDR     input data (read)
 *   BSRR    bits 0..15 set pin, bits 16..31 reset pin (atomic)
 *   AFR[0]  4 bits/pin  alternate function for pins 0..7
 *   AFR[1]  4 bits/pin  alternate function for pins 8..15
 */
#include "gpio.h"

#define GPIO_PIN_COUNT 16U

void gpio_clock_enable(GPIO_TypeDef *port)
{
    if (port == GPIOA)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    else if (port == GPIOB)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    else if (port == GPIOC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    else if (port == GPIOD)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
    else if (port == GPIOE)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    }
    else if (port == GPIOF)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    }
    else if (port == GPIOG)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    }
    else if (port == GPIOH)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    }
    else
    {
        return;
    }

    /* Dummy read: makes sure the clock is running before the port is accessed */
    (void)RCC->AHB1ENR;
}

void gpio_init(const gpio_config_t *cfg)
{
    if ((cfg == 0) || (cfg->pin >= GPIO_PIN_COUNT))
    {
        return;
    }

    GPIO_TypeDef *port = cfg->port;
    uint32_t pin = cfg->pin;
    uint32_t pos2 = pin * 2U; /* field position for 2-bit registers */

    gpio_clock_enable(port);

    /* Alternate function number first, so the pin never shows a wrong AF */
    if (cfg->mode == GPIO_MODE_AF)
    {
        uint32_t reg = pin / 8U; /* AFR[0] or AFR[1] */
        uint32_t pos4 = (pin % 8U) * 4U;
        port->AFR[reg] &= ~(0xFUL << pos4);
        port->AFR[reg] |= ((uint32_t)(cfg->af & 0xFU) << pos4);
    }

    port->OTYPER &= ~(1UL << pin);
    port->OTYPER |= ((uint32_t)cfg->otype << pin);

    port->OSPEEDR &= ~(3UL << pos2);
    port->OSPEEDR |= ((uint32_t)cfg->speed << pos2);

    port->PUPDR &= ~(3UL << pos2);
    port->PUPDR |= ((uint32_t)cfg->pull << pos2);

    /* Mode last: pin starts driving only after everything else is set */
    port->MODER &= ~(3UL << pos2);
    port->MODER |= ((uint32_t)cfg->mode << pos2);
}

void gpio_init_output(GPIO_TypeDef *port, uint8_t pin)
{
    gpio_config_t cfg = {
        .port = port,
        .pin = pin,
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PUSH_PULL,
        .speed = GPIO_SPEED_LOW,
        .pull = GPIO_PULL_NONE,
        .af = 0U,
    };
    gpio_init(&cfg);
}

void gpio_init_input(GPIO_TypeDef *port, uint8_t pin, gpio_pull_t pull)
{
    gpio_config_t cfg = {
        .port = port,
        .pin = pin,
        .mode = GPIO_MODE_INPUT,
        .otype = GPIO_OTYPE_PUSH_PULL,
        .speed = GPIO_SPEED_LOW,
        .pull = pull,
        .af = 0U,
    };
    gpio_init(&cfg);
}

void gpio_init_af(GPIO_TypeDef *port, uint8_t pin, uint8_t af,
                  gpio_otype_t otype, gpio_pull_t pull)
{
    gpio_config_t cfg = {
        .port = port,
        .pin = pin,
        .mode = GPIO_MODE_AF,
        .otype = otype,
        .speed = GPIO_SPEED_VERY_HIGH,
        .pull = pull,
        .af = af,
    };
    gpio_init(&cfg);
}

void gpio_write(GPIO_TypeDef *port, uint8_t pin, gpio_state_t state)
{
    if (pin >= GPIO_PIN_COUNT)
    {
        return;
    }
    /* BSRR is atomic: no read-modify-write, safe to use from interrupts */
    if (state == GPIO_STATE_HIGH)
    {
        port->BSRR = (1UL << pin);
    }
    else
    {
        port->BSRR = (1UL << (pin + 16U));
    }
}

void gpio_toggle(GPIO_TypeDef *port, uint8_t pin)
{
    if (pin >= GPIO_PIN_COUNT)
    {
        return;
    }
    if ((port->ODR & (1UL << pin)) != 0U)
    {
        port->BSRR = (1UL << (pin + 16U)); /* currently high -> reset */
    }
    else
    {
        port->BSRR = (1UL << pin); /* currently low  -> set   */
    }
}

gpio_state_t gpio_read(GPIO_TypeDef *port, uint8_t pin)
{
    if (pin >= GPIO_PIN_COUNT)
    {
        return GPIO_STATE_LOW;
    }
    return ((port->IDR & (1UL << pin)) != 0U) ? GPIO_STATE_HIGH : GPIO_STATE_LOW;
}
