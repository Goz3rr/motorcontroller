#include <math.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>

#include "printf.h"
#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"
#include "pins.h"
#include "motor.h"

#define VREF 3.3
#define ADC_MAX 4095

#define SUPPLY_DIVIDER_R1 62000
#define SUPPLY_DIVIDER_R2 3600

#define NTC_SERIES_R 33000
#define NTC_NOMINAL_R 100000
#define NTC_NOMINAL_TEMP 25
#define NTC_B_CONST 4050

#define THROTTLE_MIN 1024
#define THROTTLE_MAX 3072

void adc_init(void)
{
    rcc_periph_clock_enable(RCC_ADC1);

    gpio_set_mode(SUPPLY_MONITOR_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, SUPPLY_MONITOR_PIN);
    gpio_set_mode(THROTTLE_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, THROTTLE_PIN);
    gpio_set_mode(TEMP_LEFT_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, TEMP_LEFT_PIN);
    gpio_set_mode(TEMP_RIGHT_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, TEMP_RIGHT_PIN);

    adc_power_off(ADC1);

    adc_disable_scan_mode(ADC1);
    adc_set_single_conversion_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_41DOT5CYC);

    adc_enable_temperature_sensor();

    adc_power_on(ADC1);

    for (int i = 0; i < 800000; i++)
        __asm__("nop");

    adc_reset_calibration(ADC1);
    adc_calibrate(ADC1);
}

static uint16_t read_adc_channel(uint8_t channel)
{
    uint8_t channel_array[16];
    channel_array[0] = channel;
    adc_set_regular_sequence(ADC1, 1, channel_array);
    adc_start_conversion_direct(ADC1);

    while (!adc_eoc(ADC1))
        taskYIELD();

    return adc_read_regular(ADC1);
}

float ntc_to_c(uint16_t temp)
{
    float temp_R = (NTC_SERIES_R * ADC_MAX / temp) - NTC_SERIES_R;

    float steinhart = log(temp_R / NTC_NOMINAL_R) / NTC_B_CONST;
    steinhart += 1.0 / (NTC_NOMINAL_TEMP + 273.15);
    steinhart = 1.0 / steinhart;
    steinhart -= 273.15;

    return steinhart;
}

int throttle_pct(uint16_t value)
{
    if (value <= THROTTLE_MIN)
        return 0;

    if (value >= THROTTLE_MAX)
        return 100;

    int16_t adjusted = value - THROTTLE_MIN;
    return (int)((float)adjusted / (THROTTLE_MAX - THROTTLE_MIN) * 100);
}

void adc_task(void *args __attribute__((unused)))
{
    uint16_t supply, throttle, tempL, tempR, tempI;

    while (1)
    {
        supply = read_adc_channel(2);
        float supplyV = (supply / ADC_MAX) * VREF * (SUPPLY_DIVIDER_R1 + SUPPLY_DIVIDER_R2) / SUPPLY_DIVIDER_R2;
        throttle = read_adc_channel(3);
        int throttlePct = throttle_pct(throttle);
        tempL = read_adc_channel(4);
        tempR = read_adc_channel(5);
        tempI = read_adc_channel(16);

        float tempL_C = ntc_to_c(tempL);
        float tempR_C = ntc_to_c(tempR);

        float calcTemp = ((1.43f - (tempI / 4095.0f * 3.3f)) / 4.3f) + 25;
        printf("%d (%.2fV), %d (%d%%), %d (%.2fC), %d (%.2fC), %d (%.2fC)\r\n", supply, supplyV, throttle, throttlePct, tempL, tempL_C, tempR, tempR_C, tempI, calcTemp);

        //vTaskDelay(pdMS_TO_TICKS(250));

        motor_left_speed(throttlePct);
        motor_right_speed(throttlePct);
    }
}