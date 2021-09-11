#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "printf.h"

#include "uart.h"
#include "pins.h"

void uart_init(void)
{
    rcc_periph_clock_enable(RCC_USART1);

    nvic_enable_irq(NVIC_USART1_IRQ);

    AFIO_MAPR |= AFIO_MAPR_USART1_REMAP;

    gpio_set_mode(UART_TX_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, UART_TX_PIN);
    gpio_set_mode(UART_RX_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, UART_RX_PIN);

    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    usart_enable(USART1);
}

void usart1_isr(void)
{

}

void _putchar(char character)
{
    usart_send_blocking(USART1, character);
}