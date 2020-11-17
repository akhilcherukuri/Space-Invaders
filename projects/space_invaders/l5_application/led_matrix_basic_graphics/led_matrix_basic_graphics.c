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

void led_matrix_basic_graphics__display_letter(uint8_t row, uint8_t column, char letter, led_color_e color) {
  switch (letter) {
  case 'a':
  case 'A':
    for (uint8_t i = 2; i < 4; i++) {
      led_matrix__set_pixel(row + i, column, color);
      led_matrix__set_pixel(row + i, column + 4, color);
    }
    for (uint8_t i = 1; i < 5; i++) {
      led_matrix__set_pixel(row + i, column + 1, color);
    }
    for (uint8_t i = 2; i < 7; i++) {
      led_matrix__set_pixel(row + i, column, color);
      led_matrix__set_pixel(row + i, column + 1, color);
      led_matrix__set_pixel(row + i, column + 4, color);
      led_matrix__set_pixel(row + i, column + 5, color);
    }
    break;
  case 'b':
  case 'B':
    for (uint8_t i = 0; i < 6; i++) {
      led_matrix__set_pixel(row, column + i, color);
      led_matrix__set_pixel(row + 6, column + i, color);
    }
    for (uint8_t i = 1; i < 3; i++) {
      led_matrix__set_pixel(row + i, column + 1, color);
      led_matrix__set_pixel(row + i, column + 2, color);
      led_matrix__set_pixel(row + i, column + 5, color);
      led_matrix__set_pixel(row + i, column + 6, color);
    }
    for (uint8_t i = 4; i < 6; i++) {
      led_matrix__set_pixel(row + i, column + 1, color);
      led_matrix__set_pixel(row + i, column + 2, color);
      led_matrix__set_pixel(row + i, column + 5, color);
      led_matrix__set_pixel(row + i, column + 6, color);
    }
    for (uint8_t i = 1; i < 6; i++) {
      led_matrix__set_pixel(row + 3, column + i, color);
    }
    break;
  case 'c':
  case 'C':
    /* code */
    break;
  case 'd':
  case 'D':
    /* code */
    break;
  case 'e':
  case 'E':
    /* code */
    break;
  case 'f':
  case 'F':
    /* code */
    break;
  case 'g':
  case 'G':
    /* code */
    break;
  case 'h':
  case 'H':
    /* code */
    break;
  case 'i':
  case 'I':
    /* code */
    break;
  case 'j':
  case 'J':
    /* code */
    break;
  case 'k':
  case 'K':
    /* code */
    break;
  case 'l':
  case 'L':
    /* code */
    break;
  case 'm':
  case 'M':
    /* code */
    break;
  case 'n':
  case 'N':
    /* code */
    break;
  case 'o':
  case 'O':
    /* code */
    break;
  case 'p':
  case 'P':
    /* code */
    break;
  case 'q':
  case 'Q':
    /* code */
    break;
  case 'r':
  case 'R':
    /* code */
    break;
  case 's':
  case 'S':
    /* code */
    break;
  case 't':
  case 'T':
    /* code */
    break;
  case 'u':
  case 'U':
    /* code */
    break;
  case 'v':
  case 'V':
    /* code */
    break;
  case 'w':
  case 'W':
    /* code */
    break;
  case 'x':
  case 'X':
    /* code */
    break;
  case 'y':
  case 'Y':
    /* code */
    break;
  case 'z':
  case 'Z':
    /* code */
    break;
  default:
    break;
  }
}