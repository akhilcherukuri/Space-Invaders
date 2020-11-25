#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"
#include "game_graphics.h"
#include "led_matrix.h"
#include "led_matrix_basic_graphics.h"
#include "sj2_cli.h"

void start_screen_task(void *p) {
  while (1) {
    // led_matrix_basic_graphics__display_letter(20, 30, 'M', RED);
    // led_matrix_basic_graphics__display_word_score(20, 6);
    // led_matrix_basic_graphics__display_word_pressStart(35, 5);
    // game_graphics__display_splash_screen();
    game_graphics__display_game_over_screen();
    // game_graphics__display_laser_cannon(50, 20, BLUE);
    // game_graphics__display_octopus(10, 5, WHITE);
    // game_graphics__display_octopus(10, 20, WHITE);
    // game_graphics__display_octopus(10, 35, WHITE);
    // game_graphics__display_octopus(10, 50, WHITE);
    // game_graphics__display_crab(20, 5, PURPLE);
    // game_graphics__display_crab(20, 20, PURPLE);
    // game_graphics__display_crab(20, 35, PURPLE);
    // game_graphics__display_crab(20, 50, PURPLE);
    // game_graphics__display_squid(30, 5, WHITE);
    // game_graphics__display_squid(30, 20, WHITE);
    // game_graphics__display_squid(30, 35, WHITE);
    // game_graphics__display_squid(30, 50, WHITE);
    // for (size_t i = 30; i < 64; i++) {
    //   for (size_t j = 0; j < 64; j++) {
    //     led_matrix__set_pixel(i, j, PURPLE);
    //     // delay__ms(1);
    //     // led_matrix__clear_pixel(i, j, PURPLE);
    //   }
    // }
    // // Added for testing displays numbers 0-9
    // for (uint8_t i = 0; i < 10; i++) {
    //   led_matrix_basic_graphics__display_number(20, 30, i, GREEN);
    //   vTaskDelay(500);
    //   led_matrix__clear_display(GREEN);
    vTaskDelay(3);
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
