#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "game_graphics.h"
#include "led_matrix_basic_graphics.h"
#include "sj2_cli.h"

void start_screen_task(void *p) {
  while (1) {
    // Added for testing displays numbers 0-9
    for (uint8_t i = 0; i < 10; i++) {
      led_matrix_basic_graphics__display_number(20, 30, i, GREEN);
      vTaskDelay(500);
      led_matrix__clear_display(GREEN);
      vTaskDelay(500);
    }
  }
}

void refresh_display_task(void *p) {
  while (1) {
    led_matrix__display_pixels();
    vTaskDelay(3);
  }
}

int main(void) {

  (void)led_matrix__initialize();

  xTaskCreate(refresh_display_task, "refresh display", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(start_screen_task, "start screen", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
