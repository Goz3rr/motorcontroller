#pragma once

#include <libopencm3/stm32/gpio.h>

#define CPU_FREQ    72000000

// General IO
#define LED1_PORT   GPIOB
#define LED1_PIN    GPIO4
#define LED2_PORT   GPIOB
#define LED2_PIN    GPIO5

#define IO1_PORT    GPIOB
#define IO1_PIN     GPIO0
#define IO2_PORT    GPIOA
#define IO2_PIN     GPIO7
#define IO3_PORT    GPIOA
#define IO3_PIN     GPIO6


// Control
#define STEER_LEFT_PORT     GPIOC
#define STEER_LEFT_PIN      GPIO14
#define STEER_RIGHT_PORT    GPIOC
#define STEER_RIGHT_PIN     GPIO13

#define DIRECTION_PORT  GPIOC
#define DIRECTION_PIN   GPIO15

#define DIRECTION1_PORT GPIOA
#define DIRECTION1_PIN  GPIO0
#define DIRECTION2_PORT GPIOA
#define DIRECTION2_PIN  GPIO1

#define SUPPLY_MONITOR_PORT GPIOA
#define SUPPLY_MONITOR_PIN  GPIO2

#define THROTTLE_PORT   GPIOA
#define THROTTLE_PIN    GPIO3

#define TEMP_LEFT_PORT  GPIOA
#define TEMP_LEFT_PIN   GPIO5
#define TEMP_RIGHT_PORT GPIOA
#define TEMP_RIGHT_PIN  GPIO4

#define BEEP_PORT       GPIO_BANK_TIM3_CH4
#define BEEP_PIN        GPIO_TIM3_CH4

#define IGNITION_PORT   GPIOB
#define IGNITION_PIN    GPIO2

#define ENABLE_LEFT_PORT    GPIOB
#define ENABLE_LEFT_PIN     GPIO10
#define ENABLE_RIGHT_PORT   GPIOB
#define ENABLE_RIGHT_PIN    GPIO11

#define PWM_LEFT_PORT   GPIOA
#define PWM_LEFT_PIN    GPIO10
#define PWM_RIGHT_PORT  GPIOA
#define PWM_RIGHT_PIN   GPIO8

#define USB_VBUS_DET_PORT   GPIOA
#define USB_VBUS_DET_PIN    GPIO9
#define USB_DM_PORT         GPIOA
#define USB_DM_PIN          GPIO11
#define USB_DP_PORT         GPIOA
#define USB_DP_PIN          GPIO12
#define USB_PULL_PORT       GPIOA
#define USB_PULL_PIN        GPIO15


// Interfaces
#define UART_TX_PORT    GPIOB
#define UART_TX_PIN     GPIO6
#define UART_RX_PORT    GPIOB
#define UART_RX_PIN     GPIO7

#define CAN_TX_PORT GPIOB
#define CAN_TX_PIN  GPIO9
#define CAN_RX_PORT GPIOB
#define CAN_RX_PIN  GPIO8

#define SPI_CS_PORT     GPIOB
#define SPI_CS_PIN      GPIO12
#define SPI_CLK_PORT    GPIOB
#define SPI_CLK_PIN     GPIO13
#define SPI_MISO_PORT   GPIOB
#define SPI_MISO_PIN    GPIO14
#define SPI_MOSI_PORT   GPIOB
#define SPI_MOSI_PIN    GPIO15

#define SWDIO_PORT      GPIOA
#define SWDIO_PIN       GPIO13
#define SWCLK_PORT      GPIOA
#define SWCLK_PIN       GPIO14
#define TRACESWO_PORT   GPIOB
#define TRACESWO_PIN    GPIO3