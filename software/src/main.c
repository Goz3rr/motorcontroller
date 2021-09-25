#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/usart.h>

#include "printf.h"
#include "FreeRTOS.h"
#include "task.h"

#include "pins.h"
#include "uart.h"
#include "adc.h"
#include "buzzer.h"
#include "motor.h"
#include "solenoid.h"

void clock_init(void);
void gpio_init(void);

void vApplicationStackOverflowHook(TaskHandle_t pxTask __attribute__((unused)), char *pcTaskName __attribute__((unused)))
{
    while (1);
}

void vAssertCalled(unsigned long ulLine __attribute__((unused)), const char *const pcFileName __attribute__((unused)))
{
    volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    taskENTER_CRITICAL();
    {
        while (ulSetToNonZeroInDebuggerToContinue == 0)
        {
        }
    }
    taskEXIT_CRITICAL();
}

static void task1(void *args __attribute__((unused)))
{
    while (1)
    {
        gpio_toggle(GPIOB, GPIO4);
        gpio_toggle(GPIOB, GPIO5);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void task2(void *args __attribute__((unused)))
{
    while (1)
    {
        buzzer_beep();
        vTaskDelay(pdMS_TO_TICKS(900));
    }
}

void clock_init(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_AFIO);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
}

void gpio_init(void)
{
    AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON; // Disable JTAG
}

int main(void)
{
    clock_init();
    gpio_init();

    uart_init();
    adc_init();
    
    motor_init();
    buzzer_init();
    solenoid_init();

    printf("Starting up!\r\n");

    gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED1_PIN);
    gpio_set_mode(LED2_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED2_PIN);

    gpio_clear(LED1_PORT, LED1_PIN);
    gpio_clear(LED2_PORT, LED2_PIN);

    xTaskCreate(task1, "LED", 100, NULL, 2, NULL);
    //xTaskCreate(task2, "Beep", 100, NULL, 2, NULL);
    xTaskCreate(adc_task, "ADC", 256, NULL, 2, NULL);
    //xTaskCreate(solenoid_task, "Solenoid", 256, NULL, 2, NULL);

    vTaskStartScheduler();

    while (1);
    return 0;
}