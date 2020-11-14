/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Main Module Header */
#include "led_matrix_basic_graphics.h"

/* Standard Includes */

/* External Includes */

/***********************************************************************************************************************
 *
 *                                                   D E F I N E S
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *                                                  T Y P E D E F S
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *                             P R I V A T E   F U N C T I O N   D E C L A R A T I O N S
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *                                  P R I V A T E   D A T A   D E F I N I T I O N S
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *                                         P R I V A T E   F U N C T I O N S
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *                                          P U B L I C   F U N C T I O N S
 *
 **********************************************************************************************************************/

void led_matrix_basic_graphics__display_number(uint8_t row, uint8_t column, uint8_t number, led_color_e color) {
  switch (number) {
  case 0:
    // Draw left and right sides
    for (uint8_t i = 1; i < 7; i++) {
      led_matrix__set_pixel(row + i, column, color);
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    // Draw top and bottom sides
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row, column + i, color);
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    break;
  case 1:
    // Draw bottom side
    for (uint8_t i = 0; i < 3; i++) {
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    // Draw vertical line
    for (uint8_t i = 0; i < 7; i++) {
      led_matrix__set_pixel(row + i, column + 1, color);
    }
    // Draw dot
    led_matrix__set_pixel(row + 1, column, color);
    break;
  case 2:
    // Draw top side
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row, column + i, color);
    }
    // Draw bottom side
    for (uint8_t i = 0; i < 5; i++) {
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    led_matrix__set_pixel(row + 1, column, color);
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    led_matrix__set_pixel(row + 4, column + 3, color);
    led_matrix__set_pixel(row + 5, column + 2, color);
    led_matrix__set_pixel(row + 6, column + 1, color);
    break;
  case 3:
    // Draw top and bottom sides
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row, column + i, color);
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    led_matrix__set_pixel(row + 1, column, color);
    led_matrix__set_pixel(row + 1, column + 4, color);
    led_matrix__set_pixel(row + 2, column + 4, color);
    led_matrix__set_pixel(row + 3, column + 2, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    for (uint8_t i = 4; i < 7; i++) {
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    led_matrix__set_pixel(row + 6, column, color);
    break;
  case 4:
    for (uint8_t i = 0; i < 8; i++) {
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 3, column + 1, color);
    led_matrix__set_pixel(row + 4, column, color);
    for (uint8_t i = 0; i < 4; i++) {
      led_matrix__set_pixel(row + 5, column + i, color);
    }
    break;
  case 5:
    for (uint8_t i = 0; i < 5; i++) {
      led_matrix__set_pixel(row, column + i, color);
    }
    led_matrix__set_pixel(row + 1, column, color);
    led_matrix__set_pixel(row + 2, column, color);
    for (uint8_t i = 0; i < 4; i++) {
      led_matrix__set_pixel(row + 3, column + i, color);
    }
    for (uint8_t i = 4; i < 7; i++) {
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    led_matrix__set_pixel(row + 6, column, color);
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    break;
  case 6:
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row, column + i, color);
    }
    led_matrix__set_pixel(row + 1, column + 4, color);
    for (uint8_t i = 1; i < 7; i++) {
      led_matrix__set_pixel(row + i, column, color);
    }
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row + 3, column + i, color);
    }
    for (uint8_t i = 4; i < 7; i++) {
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    break;
  case 7:
    for (uint8_t i = 0; i < 5; i++) {
      led_matrix__set_pixel(row, column + i, color);
    }
    led_matrix__set_pixel(row + 1, column + 4, color);
    led_matrix__set_pixel(row + 2, column + 4, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    for (uint8_t i = 5; i < 8; i++) {
      led_matrix__set_pixel(row + i, column + 1, color);
    }
    break;
  case 8:
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row, column + i, color);
      led_matrix__set_pixel(row + 3, column + i, color);
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    for (uint8_t i = 1; i < 3; i++) {
      led_matrix__set_pixel(row + i, column, color);
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    for (uint8_t i = 4; i < 7; i++) {
      led_matrix__set_pixel(row + i, column, color);
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    break;
  case 9:
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row, column + i, color);
      led_matrix__set_pixel(row + 4, column + i, color);
      led_matrix__set_pixel(row + 7, column + i, color);
    }
    for (uint8_t i = 1; i < 4; i++) {
      led_matrix__set_pixel(row + i, column, color);
    }
    for (uint8_t i = 1; i < 7; i++) {
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    led_matrix__set_pixel(row + 6, column, color);
    break;
  default:
    break;
  }
}

void led_matrxi_basic_graphics__display_letter(uint8_t x, uint8_t y, char letter, led_color_e color) {}