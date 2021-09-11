#pragma once

float ntc_to_c(uint16_t temp);
int throttle_pct(uint16_t value);

void adc_init(void);
void adc_task(void* args);