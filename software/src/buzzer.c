#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "printf.h"
#include "FreeRTOS.h"
#include "task.h"

#include "buzzer.h"
#include "pins.h"

#define PWM_FREQ 4000

void buzzer_init(void)
{
    rcc_periph_clock_enable(RCC_TIM3);

    gpio_set_mode(BEEP_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, BEEP_PIN);

    rcc_periph_reset_pulse(RST_TIM3);

    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM3, ((CPU_FREQ/65536) / PWM_FREQ));
    timer_continuous_mode(TIM3);
    timer_set_period(TIM3, 65535);

    timer_disable_oc_output(TIM3, TIM_OC4);
    timer_set_oc_mode(TIM3, TIM_OC4, TIM_OCM_PWM1);
	timer_disable_oc_clear(TIM3, TIM_OC4);
	timer_set_oc_value(TIM3, TIM_OC4, 0);
	timer_enable_oc_preload(TIM3, TIM_OC4);
	timer_set_oc_polarity_high(TIM3, TIM_OC4);
	timer_enable_oc_output(TIM3, TIM_OC4);

    timer_enable_counter(TIM3);
}

void buzzer_beep(void)
{
    timer_set_oc_value(TIM3, TIM_OC4, 16000);

    vTaskDelay(pdMS_TO_TICKS(100));

    timer_set_oc_value(TIM3, TIM_OC4, 0);
}