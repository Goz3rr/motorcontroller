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

    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, ((CPU_FREQ/65536) / PWM_FREQ));
    timer_continuous_mode(TIM1);
    timer_set_period(TIM1, 65535);

    timer_disable_oc_output(TIM1, TIM_OC1);
    timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
	timer_disable_oc_clear(TIM1, TIM_OC1);
	timer_set_oc_value(TIM1, TIM_OC1, 0);
	timer_enable_oc_preload(TIM1, TIM_OC1);
	timer_set_oc_polarity_high(TIM1, TIM_OC1);
	timer_enable_oc_output(TIM1, TIM_OC1);

    timer_disable_oc_output(TIM1, TIM_OC3);
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
	timer_disable_oc_clear(TIM1, TIM_OC3);
	timer_set_oc_value(TIM1, TIM_OC3, 0);
	timer_enable_oc_preload(TIM1, TIM_OC3);
	timer_set_oc_polarity_high(TIM1, TIM_OC3);
	timer_enable_oc_output(TIM1, TIM_OC3);

    timer_enable_counter(TIM1);
}

void motor_left_speed(uint8_t speed)
{
    if(speed == 0)
    {
        timer_set_oc_value(TIM1, TIM_OC3, 65535/100*speed);
    }
    else if(speed < 100)
    {
        timer_set_oc_value(TIM1, TIM_OC3, 65535/100*speed);
    }
    else
    {
        timer_set_oc_value(TIM1, TIM_OC3, 65535);
    }
}

void motor_right_speed(uint8_t speed)
{
    if(speed == 0)
    {
        timer_set_oc_value(TIM1, TIM_OC1, 65535/100*speed);
    }
    else if(speed < 100)
    {
        timer_set_oc_value(TIM1, TIM_OC1, 65535/100*speed);
    }
    else
    {
        timer_set_oc_value(TIM1, TIM_OC1, 65535);
    }
}