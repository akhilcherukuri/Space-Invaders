#pragma once
/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Standard Includes */
#include <stdint.h>

/* External Includes */

/* Module Includes */

/***********************************************************************************************************************
 *
 *                                                   D E F I N E S
 *
 **********************************************************************************************************************/
#define MATRIX_HEIGHT 64
#define MATRIX_WIDTH 64
#define MATRIX_ROWS 32

uint8_t matrix_buffer[MATRIX_ROWS][MATRIX_HEIGHT];
/***********************************************************************************************************************
 *
 *                                                  T Y P E D E F S
 *
 **********************************************************************************************************************/
typedef enum {
  BLACK = 0,
  BLUE = 1,
  GREEN = 2,
  ELECTRIC_BLUE = 3,
  RED = 4,
  PURPLE = 5,
  YELLOW = 6,
  WHITE = 7
} led_color_e;
/***********************************************************************************************************************
 *
 *                                     F U N C T I O N   D E C L A R A T I O N S
 *
 **********************************************************************************************************************/

void led_matrix__initialize(void);
void led_matrix__display_pixels(void);
void led_matrix__clear_display(led_color_e color);
void led_matrix__set_pixel(uint8_t row, uint8_t column, led_color_e color);
void led_matrix__clear_pixel(uint8_t row, uint8_t column, led_color_e color);