#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "motor.h"

#define PWM_FREQ 4000

void motor_init(void)
{
    rcc_periph_clock_enable(RCC_TIM1);

    gpio_set_mode(PWM_LEFT_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, PWM_LEFT_PIN);
    gpio_set_mode(PWM_RIGHT_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, PWM_RIGHT_PIN);

    rcc_periph_reset_pulse(RST_TIM1);

    /* Reset prescaler value. */
    timer_set_prescaler(TIM1, 0);

    /* Reset repetition counter value. */
    timer_set_repetition_counter(TIM1, 0);

    /* Enable preload. */
    timer_enable_preload(TIM1);

    /* Continuous mode. */
    timer_continuous_mode(TIM1);

    /* Period (32kHz). */
    timer_set_period(TIM1, CPU_FREQ / 32000);

    /* Configure break and deadtime. */
    timer_set_deadtime(TIM1, 0);
    timer_set_enabled_off_state_in_idle_mode(TIM1);
    timer_set_enabled_off_state_in_run_mode(TIM1);
    timer_disable_break(TIM1);
    timer_set_break_polarity_high(TIM1);
    timer_disable_break_automatic_output(TIM1);
    timer_set_break_lock(TIM1, TIM_BDTR_LOCK_OFF);

    /* -- OC1 and OC1N configuration -- */

    /* Disable outputs. */
    timer_disable_oc_output(TIM1, TIM_OC1);

    /* Configure global mode of line 1. */
    timer_disable_oc_clear(TIM1, TIM_OC1);
    timer_enable_oc_preload(TIM1, TIM_OC1);
    timer_set_oc_slow_mode(TIM1, TIM_OC1);
    timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);

    /* Configure OC1. */
    timer_set_oc_polarity_high(TIM1, TIM_OC1);
    timer_set_oc_idle_state_set(TIM1, TIM_OC1);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(TIM1, TIM_OC1, 0);

    /* Reenable outputs. */
    timer_enable_oc_output(TIM1, TIM_OC1);

    /* -- OC3 and OC3N configuration -- */

    /* Disable outputs. */
    timer_disable_oc_output(TIM1, TIM_OC3);

    /* Configure global mode of line 3. */
    timer_disable_oc_clear(TIM1, TIM_OC3);
    timer_enable_oc_preload(TIM1, TIM_OC3);
    timer_set_oc_slow_mode(TIM1, TIM_OC3);
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

    /* Configure OC3. */
    timer_set_oc_polarity_high(TIM1, TIM_OC3);
    timer_set_oc_idle_state_set(TIM1, TIM_OC3);

    /* Set the capture compare value for OC3. */
    timer_set_oc_value(TIM1, TIM_OC3, 0);

    /* Reenable outputs. */
    timer_enable_oc_output(TIM1, TIM_OC3);

    /* ---- */

    /* ARR reload enable. */
    timer_enable_preload(TIM1);

    /* Enable outputs in the break subsystem. */
    timer_enable_break_main_output(TIM1);

    /* Counter enable. */
    timer_enable_counter(TIM1);
}

void motor_left_speed(uint8_t speed)
{
    if (speed > 100)
        speed = 100;

    timer_set_oc_value(TIM1, TIM_OC3, speed * 23);
}

void motor_right_speed(uint8_t speed)
{
    if (speed > 100)
        speed = 100;

    timer_set_oc_value(TIM1, TIM_OC1, speed * 23);
}