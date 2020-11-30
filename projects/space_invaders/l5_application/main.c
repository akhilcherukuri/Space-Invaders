#include <stdbool.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "delay.h"
#include "game_graphics.h"
#include "game_logic.h"
#include "gpio.h"
#include "gpio_isr.h"
#include "led_matrix.h"
#include "led_matrix_basic_graphics.h"
#include "sj2_cli.h"
#include "sys_time.h"

#include "lpc40xx.h"
#include "lpc_peripherals.h"

extern bool is_game_over;

static void shooting_button_isr(void);
static void start_button_isr(void);
static void configure_gpio_interrupts(void);
static SemaphoreHandle_t shooting_button_pressed;
static SemaphoreHandle_t start_button_pressed;

uint64_t button_pressed_time = 0;
uint64_t button_last_time_pressed = 0;

void led_decorative_sign_task(void *p);
void refresh_display_task(void *p);
void start_screen_task(void *p);
void game_over_screen_task(void *p);
void move_laser_cannon_task(void *p);
void move_enemies_task(void *p);

int main(void) {

  (void)led_matrix__initialize();
  (void)game_logic__initialize();
  // (void)configure_gpio_interrupts();

  // start_button_pressed = xSemaphoreCreateBinary();

  xTaskCreate(refresh_display_task, "refresh display", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(led_decorative_sign_task, "led decorative sign", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(start_screen_task, "start screen", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(game_over_screen_task, "game over screen", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(move_laser_cannon_task, "move laser cannon", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(move_enemies_task, "move enemies", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void led_decorative_sign_task(void *p) {
  while (1) {
    for (size_t i = 0; i < 64; i++) {
      led_matrix__set_pixel(0, i, WHITE);
      led_matrix__set_pixel(1, i, WHITE);
      led_matrix__set_pixel(2, i, WHITE);
    }
    vTaskDelay(3);
  }
}

void refresh_display_task(void *p) {
  while (1) {
    led_matrix__display_pixels();
    vTaskDelay(3);
  }
}

void start_screen_task(void *p) {
  while (1) {
    game_graphics__display_splash_screen();
    // if (!is_start_button_pressed){
    //  game_graphics__display_splash_screen();
    //} else {
    //  led_matrix__clear_display();
    //
    //}
    vTaskDelay(3);
  }
}

void game_over_screen_task(void *p) {
  while (1) {
    if (is_game_over) {
      game_graphics__display_game_over_screen();
    }
    vTaskDelay(3);
  }
}

void move_laser_cannon_task(void *p) {
  while (1) {
    game_logic__move_laser_cannon();
    vTaskDelay(3);
  }
}

void move_enemies_task(void *p) {
  while (1) {
    game_logic__move_enemies();
    vTaskDelay(3);
  }
}

// static void gpio_joystick_isr(void) {
//   button_pressed_time = sys_time__get_uptime_ms();
//   if (button_pressed_time - button_last_time_pressed > 200) {
//     button_last_time_pressed = button_pressed_time;
//     xSemaphoreGiveFromISR(joystick_button_pressed, NULL);
//   }
// }

// static void configure_gpio_interrupts(void) {
//   (void)game_logic__initialize();
//   gpio_isr__attach_interrupt(0, 0, GPIO_INTR__RISING_EDGE, gpio_joystick_isr);
//   gpio_isr__attach_interrupt(0, 1, GPIO_INTR__RISING_EDGE, gpio_joystick_isr);
//   lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpio_isr__interrupt_dispatcher, "joystick_interrupt");
//   NVIC_EnableIRQ(GPIO_IRQn);
// }
