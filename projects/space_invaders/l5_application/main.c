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

static void shooting_button_isr(void);
static void start_button_isr(void);
static void configure_gpio_interrupts(void);

static SemaphoreHandle_t shooting_button_pressed;
static SemaphoreHandle_t start_button_pressed;

static TaskHandle_t start_screen_task_handle;
static TaskHandle_t move_laser_cannon_task_handle;
static TaskHandle_t move_enemies_task_handle;
static TaskHandle_t laser_cannon_shooting_task_handle;
static TaskHandle_t enemy_shooting_task_handle;

void refresh_display_task(void *p);
void led_decorative_sign_task(void *p);
void display_scoreboard_task(void *p);
void start_screen_task(void *p);
void victory_screen_task(void *p);
void game_over_screen_task(void *p);
void move_laser_cannon_task(void *p);
void move_enemies_task(void *p);
void laser_cannon_shooting_task(void *p);
void enemy_shooting_task(void *p);

uint64_t button_pressed_time = 0;
uint64_t button_last_time_pressed = 0;

bool is_game_started = false;

int main(void) {

  (void)led_matrix__initialize();
  (void)game_logic__initialize();
  (void)configure_gpio_interrupts();

  start_button_pressed = xSemaphoreCreateBinary();
  shooting_button_pressed = xSemaphoreCreateBinary();

  xTaskCreate(refresh_display_task, "refresh display", 4096 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(led_decorative_sign_task, "led decorative sign", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(display_scoreboard_task, "display scoreboard", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(start_screen_task, "start screen", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &start_screen_task_handle);
  xTaskCreate(victory_screen_task, "victory screen", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(game_over_screen_task, "game over screen", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(move_laser_cannon_task, "move laser cannon", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &move_laser_cannon_task_handle);
  xTaskCreate(move_enemies_task, "move enemies", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &move_enemies_task_handle);
  xTaskCreate(laser_cannon_shooting_task, "laser cannon shooting", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &laser_cannon_shooting_task_handle);
  xTaskCreate(enemy_shooting_task, "enemy task for shooting", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &enemy_shooting_task_handle);

  sj2_cli__init();
  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void refresh_display_task(void *p) {
  while (1) {
    led_matrix__display_pixels();
    vTaskDelay(3);
  }
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

void display_scoreboard_task(void *p) {
  uint8_t start_row = 5;
  uint8_t start_column = 1;
  while (1) {
    if (is_game_started) {
      led_matrix_basic_graphics__display_word_score(start_row, start_column, PURPLE);
      game_graphics__display_heart_symbol(5, 50, RED);
      led_matrix_basic_graphics__display_number(5, 56, game_logic__get_laser_cannon_lives(), ELECTRIC_BLUE);
      for (size_t i = 0; i < MATRIX_WIDTH; i++) {
        led_matrix__set_pixel(10, i, WHITE);
      }
    }
    vTaskDelay(3);
  }
}

void start_screen_task(void *p) {
  while (1) {
    if (!is_game_started) {
      led_matrix__clear_display();
      game_graphics__display_splash_screen();
      if (xSemaphoreTake(start_button_pressed, portMAX_DELAY)) {
        is_game_started = true;
        game_logic__reset_game();
        led_matrix__clear_display();
        vTaskSuspend(start_screen_task_handle);
      }
    }
    vTaskDelay(3);
  }
}

void victory_screen_task(void *p) {
  while (1) {
    if (game_logic__get_game_won_status()) {
      led_matrix__clear_display();
      game_graphics__display_victory_screen();
      is_game_started = false;
      if (xSemaphoreTake(start_button_pressed, portMAX_DELAY)) {
        game_logic__set_game_won_status(false);
        vTaskResume(start_screen_task_handle);
      }
    }
    vTaskDelay(3);
  }
}

void game_over_screen_task(void *p) {
  while (1) {
    if (game_logic__get_game_over_status()) {
      led_matrix__clear_display();
      game_graphics__display_game_over_screen();
      is_game_started = false;
      if (xSemaphoreTake(start_button_pressed, portMAX_DELAY)) {
        game_logic__set_game_over_status(false);
        vTaskResume(start_screen_task_handle);
      }
    }
    vTaskDelay(3);
  }
}

void move_laser_cannon_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__move_laser_cannon();
    }
    vTaskDelay(3);
  }
}

void move_enemies_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__move_enemies();
    }
    vTaskDelay(3);
  }
}

void enemy_shooting_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__check_valid_enemy_to_shoot_bullet();
    }
    vTaskDelay(3);
  }
}

void laser_cannon_shooting_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__update_bullet_location();
      if (xSemaphoreTake(shooting_button_pressed, 0)) {
        game_logic__shoot_bullet();
      }
    }
    vTaskDelay(3);
  }
}

static void shooting_button_isr(void) {
  if (is_game_started) {
    button_pressed_time = sys_time__get_uptime_ms();
    if (button_pressed_time - button_last_time_pressed > 200) {
      button_last_time_pressed = button_pressed_time;
      xSemaphoreGiveFromISR(shooting_button_pressed, NULL);
    }
  }
}

static void start_button_isr(void) {
  if (!is_game_started) {
    button_pressed_time = sys_time__get_uptime_ms();
    if (button_pressed_time - button_last_time_pressed > 200) {
      button_last_time_pressed = button_pressed_time;
      xSemaphoreGiveFromISR(start_button_pressed, NULL);
    }
  }
}

static void configure_gpio_interrupts(void) {
  gpio_isr__attach_interrupt(0, 25, GPIO_INTR__RISING_EDGE, shooting_button_isr);
  gpio_isr__attach_interrupt(0, 26, GPIO_INTR__RISING_EDGE, start_button_isr);
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpio_isr__interrupt_dispatcher, "gpio_buttons_interrupt");
  NVIC_EnableIRQ(GPIO_IRQn);
}
