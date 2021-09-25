#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "solenoid.h"

void setup_timer_channel(enum tim_oc_id channel);

void solenoid_init(void)
{
    rcc_periph_clock_enable(RCC_TIM2);

    AFIO_MAPR |= AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP2;

    gpio_set_mode(DIRECTION1_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, DIRECTION1_PIN);
    gpio_set_mode(DIRECTION2_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, DIRECTION2_PIN);
    gpio_set_mode(ENABLE_LEFT_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, ENABLE_LEFT_PIN);
    gpio_set_mode(ENABLE_RIGHT_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, ENABLE_RIGHT_PIN);

    rcc_periph_reset_pulse(RST_TIM2);

    /* Reset prescaler value. */
    timer_set_prescaler(TIM2, 0);

    /* Enable preload. */
    timer_enable_preload(TIM2);

    /* Continuous mode. */
    timer_continuous_mode(TIM2);

    /* Period (32kHz). */
    timer_set_period(TIM2, CPU_FREQ / 32000);

    /* -- OC1-4 configuration -- */
    setup_timer_channel(TIM_OC1);
    setup_timer_channel(TIM_OC2);
    setup_timer_channel(TIM_OC3);
    setup_timer_channel(TIM_OC4);

    /* ---- */

    /* ARR reload enable. */
    timer_enable_preload(TIM2);

    /* Counter enable. */
    timer_enable_counter(TIM2);
}

void setup_timer_channel(enum tim_oc_id channel)
{
    /* Disable outputs. */
    timer_disable_oc_output(TIM2, channel);

    /* Configure global mode of line 1. */
    timer_disable_oc_clear(TIM2, channel);
    timer_enable_oc_preload(TIM2, channel);
    timer_set_oc_slow_mode(TIM2, channel);
    timer_set_oc_mode(TIM2, channel, TIM_OCM_PWM1);

    /* Configure OC1. */
    timer_set_oc_polarity_high(TIM2, channel);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(TIM2, channel, 0);

    /* Reenable outputs. */
    timer_enable_oc_output(TIM2, channel);
}

void solenoid_task(void *args __attribute__((unused)))
{
    while (1)
    {

    }
}

void solenoid_dir1_enable(bool enable)
{

}

void solenoid_dir2_enable(bool enable)
{

}

void solenoid_en1_enable(bool enable)
{

}

void solenoid_en2_enable(bool enable)
{

}