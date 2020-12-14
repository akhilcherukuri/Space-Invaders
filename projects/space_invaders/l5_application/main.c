#include <stdbool.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "delay.h"
#include "ff.h"
#include "game_graphics.h"
#include "game_logic.h"
#include "gpio.h"
#include "gpio_isr.h"
#include "led_matrix.h"
#include "led_matrix_basic_graphics.h"
#include "mp3_decoder.h"
#include "sj2_cli.h"
#include "sys_time.h"
#include "uart.h"

#include "lpc40xx.h"
#include "lpc_peripherals.h"

#define GAME_BOARD 0

#if GAME_BOARD
static SemaphoreHandle_t shooting_button_pressed;
static SemaphoreHandle_t start_button_pressed;
#else
static SemaphoreHandle_t volume_up_button_pressed;
static SemaphoreHandle_t volume_down_button_pressed;
static SemaphoreHandle_t mp3_mutex = NULL;

static QueueHandle_t mp3_file_data;
static QueueHandle_t what_song_to_play;

FIL file;
#endif

static QueueHandle_t send_uart;
static QueueHandle_t receive_uart;

#if GAME_BOARD
static TaskHandle_t start_screen_task_handle;
static TaskHandle_t move_laser_cannon_task_handle;
static TaskHandle_t move_enemies_task_handle;
static TaskHandle_t laser_cannon_shooting_task_handle;
static TaskHandle_t enemy_shooting_task_handle;

static bool is_game_started = false;
#endif

static uint64_t button_pressed_time = 0;
static uint64_t button_last_time_pressed = 0;

#if GAME_BOARD
static void refresh_display_task(void *p);
static void led_decorative_sign_task(void *p);
static void display_scoreboard_task(void *p);
static void start_screen_task(void *p);
static void victory_screen_task(void *p);
static void game_over_screen_task(void *p);
static void move_laser_cannon_task(void *p);
static void move_enemies_task(void *p);
static void laser_cannon_shooting_task(void *p);
static void enemy_shooting_task(void *p);
static void kill_animation_task(void *p);
#else
static void volume_control_task(void *p);
static void which_song_to_play_task(void *p);
static void play_game_sound_task(void *p);
static void audio_decoder_task(void *p);
#endif

#if GAME_BOARD
static void shooting_button_isr(void);
static void start_button_isr(void);
static void configure_gpio_interrupts(void);
#else
static void volume_up_isr(void);
static void volume_down_isr(void);
static void configure_gpio_interrupts(void);
#endif

static void initialize_uart_for_boards(void);

#if GAME_BOARD
static void initialize_game_logic_board(void);
#else
static void initialize_game_sound_board(void);
#endif

int main(void) {

#if GAME_BOARD
  initialize_game_logic_board();
#else
  initialize_game_sound_board();
#endif

  sj2_cli__init();
  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

/***********************************************************************************************************************
 *
 *                                                    T A S K S
 *
 **********************************************************************************************************************/

#if GAME_BOARD

static void refresh_display_task(void *p) {
  while (1) {
    led_matrix__display_pixels();
    vTaskDelay(3);
  }
}

static void led_decorative_sign_task(void *p) {
  while (1) {
    game_graphics__turn_on_decorative_led_bar(WHITE);
    vTaskDelay(3);
  }
}

static void display_scoreboard_task(void *p) {
  uint8_t start_row = 5;
  uint8_t start_column = 1;
  uint8_t score_row_position = 5;
  uint8_t score_column_position = 31;
  while (1) {
    if (is_game_started) {
      led_matrix_basic_graphics__display_word_score(start_row, start_column, PURPLE);
      game_graphics__display_heart_symbol(5, 50, RED);
      game_graphics__display_score_board(score_row_position, score_column_position, ELECTRIC_BLUE,
                                         game_logic__get_game_overall_score());
      led_matrix_basic_graphics__display_number(5, 56, game_logic__get_laser_cannon_lives(), ELECTRIC_BLUE);
      for (size_t i = 0; i < MATRIX_WIDTH; i++) {
        led_matrix__set_pixel(10, i, WHITE);
      }
    }
    vTaskDelay(3);
  }
}

static void start_screen_task(void *p) {
  while (1) {
    if (!is_game_started) {
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

static void victory_screen_task(void *p) {
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

static void game_over_screen_task(void *p) {
  while (1) {
    if (game_logic__get_game_over_status()) {
      led_matrix__clear_display();
      game_graphics__display_game_over_screen();
      is_game_started = false;
      if (xSemaphoreTake(start_button_pressed, portMAX_DELAY)) {
        vTaskResume(start_screen_task_handle);
        game_logic__set_game_over_status(false);
      }
    }
    vTaskDelay(3);
  }
}

static void move_laser_cannon_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__move_laser_cannon();
    }
    vTaskDelay(3);
  }
}

static void move_enemies_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__move_enemies();
    }
    vTaskDelay(3);
  }
}

static void enemy_shooting_task(void *p) {
  while (1) {
    if (is_game_started) {
      game_logic__check_valid_enemy_to_shoot_bullet();
    }
    vTaskDelay(3);
  }
}

static void laser_cannon_shooting_task(void *p) {
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

static void kill_animation_task(void *p) {
  while (1) {
    if (game_logic__get_game_status_to_display_enemy_killed_animation()) {
      vTaskSuspend(move_enemies_task_handle);
    } else {
      vTaskResume(move_enemies_task_handle);
    }
    vTaskDelay(3);
  }
}

#else

static void volume_control_task(void *p) {
  while (1) {
    static uint16_t volume = 0x0101;
    if (xSemaphoreTake(volume_down_button_pressed, 0)) {
      --volume;
      mp3_decoder__sci(write, SCI_VOLUME, volume);
    } else if (xSemaphoreTake(volume_up_button_pressed, 0)) {
      ++volume;
      mp3_decoder__sci(write, SCI_VOLUME, volume);
    } else {
      // do nothing
    }
  }
  vTaskDelay(3);
}

static void which_song_to_play_task(void *p) {
  char *song_file = NULL;
  char song_number;
  // uart__enable_queues(UART__3, send_uart, receive_uart);
  while (1) {
    uart__get(UART__3, &song_number, 5000);
    if (song_number == '1') {
      song_file = "invaderkilled.wav";
      xQueueSend(what_song_to_play, &song_file, 0);
    } else if (song_number == '2') {
      song_file = "shoot.wav";
      xQueueSend(what_song_to_play, &song_file, 0);
    } else if (song_number == '3') {
      song_file = "explosion.wav";
      xQueueSend(what_song_to_play, &song_file, 0);
    } else if (song_number == '4') {
      song_file = "fastinvader1.wav";
      xQueueSend(what_song_to_play, &song_file, 0);
    } else {
      // do nothing
    }
    song_number = 0;
    vTaskDelay(3);
  }
}

void play_game_sound_task(void *p) {
  char *song_file = "shoot.wav";
  unsigned char bytes_512[512];
  UINT bytes_read = 0;
  f_open(&file, song_file, FA_READ);
  while (1) {
    while (!f_eof(&file)) {
      xSemaphoreTake(mp3_mutex, portMAX_DELAY);
      f_read(&file, bytes_512, 512, &bytes_read);
      xSemaphoreGive(mp3_mutex);
      xQueueSend(mp3_file_data, &bytes_512[0], portMAX_DELAY);
    }
    f_close(&file);
    // xQueueReceive(what_song_to_play, &song_file, portMAX_DELAY);
    f_open(&file, song_file, FA_READ);
    vTaskDelay(3);
  }
}

static void audio_decoder_task(void *p) {
  unsigned char data[512];
  while (1) {
    if (xQueueReceive(mp3_file_data, &data, portMAX_DELAY)) {
      size_t byte_counter = 0;
      if (xSemaphoreTake(mp3_mutex, portMAX_DELAY)) {
        while (byte_counter < sizeof(data)) {
          if (mp3_decoder__dreq()) {
            for (size_t byte = byte_counter; byte < (byte_counter + 32); byte++) {
              mp3_decoder__sdi(data[byte]);
            }
            byte_counter += 32;
          }
          // } else {
          //   vTaskDelay(1);
          // }
        }
        xSemaphoreGive(mp3_mutex);
      }
    }
    vTaskDelay(3);
  }
}
#endif

/***********************************************************************************************************************
 *
 *                                                 I N T E R R U P T S
 *
 **********************************************************************************************************************/

#if GAME_BOARD

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
#else
static void volume_down_isr(void) {
  button_pressed_time = sys_time__get_uptime_ms();
  if (button_pressed_time - button_last_time_pressed > 200) {
    button_last_time_pressed = button_pressed_time;
    xSemaphoreGiveFromISR(volume_down_button_pressed, NULL);
  }
}

static void volume_up_isr(void) {
  button_pressed_time = sys_time__get_uptime_ms();
  if (button_pressed_time - button_last_time_pressed > 200) {
    button_last_time_pressed = button_pressed_time;
    xSemaphoreGiveFromISR(volume_up_button_pressed, NULL);
  }
}

static void configure_gpio_interrupts(void) {
  gpio_isr__attach_interrupt(0, 25, GPIO_INTR__RISING_EDGE, volume_down_isr);
  gpio_isr__attach_interrupt(0, 26, GPIO_INTR__RISING_EDGE, volume_up_isr);
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpio_isr__interrupt_dispatcher, "gpio_buttons_interrupt");
  NVIC_EnableIRQ(GPIO_IRQn);
}
#endif

/***********************************************************************************************************************
 *
 *                                                I N I T I A L I Z E R S
 *
 **********************************************************************************************************************/

void initialize_uart_for_boards(void) {
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // tx pin
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // rx pin
  send_uart = xQueueCreate(1, sizeof(char));
  receive_uart = xQueueCreate(1, sizeof(char));
  uart__init(UART__3, clock__get_peripheral_clock_hz(), 9600);
  uart__enable_queues(UART__3, receive_uart, send_uart);
}

#if GAME_BOARD
void initialize_game_logic_board(void) {
  (void)led_matrix__initialize();
  (void)game_logic__initialize();
  (void)initialize_uart_for_boards();
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
  xTaskCreate(enemy_shooting_task, "enemy shooting", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &enemy_shooting_task_handle);
  xTaskCreate(kill_animation_task, "kill animation", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
}
#else
void initialize_game_sound_board(void) {
  (void)initialize_uart_for_boards();
  (void)configure_gpio_interrupts();

  volume_up_button_pressed = xSemaphoreCreateBinary();
  volume_down_button_pressed = xSemaphoreCreateBinary();

  gpio_s dreq = {GPIO__PORT_2, 0};
  gpio_s xcs = {GPIO__PORT_2, 2};
  gpio_s xdcs = {GPIO__PORT_2, 5};
  gpio_s rst = {GPIO__PORT_2, 7};
  gpio_s volume_up = {GPIO__PORT_0, 26};
  gpio_s volume_down = {GPIO__PORT_0, 25};

  gpio__construct_with_function(GPIO__PORT_0, 7, GPIO__FUNCTION_2); // SCK
  gpio__construct_with_function(GPIO__PORT_0, 9, GPIO__FUNCTION_2); // MOSI
  gpio__construct_with_function(GPIO__PORT_0, 8, GPIO__FUNCTION_2); // MISO

  gpio__set_as_output(xdcs);
  gpio__set_as_output(xcs);
  gpio__set_as_output(rst);
  gpio__set_as_input(dreq);
  gpio__set_as_input(volume_up);
  gpio__set_as_input(volume_down);

  mp3_decoder__init(xcs, xdcs, dreq, rst);

  xTaskCreate(play_game_sound_task, "Play song", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(audio_decoder_task, "MP3 decoding", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(which_song_to_play_task, "Song selection", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(volume_control_task, "Change volume", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
}
#endif
