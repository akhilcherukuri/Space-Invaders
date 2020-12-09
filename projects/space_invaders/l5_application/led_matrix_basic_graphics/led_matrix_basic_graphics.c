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
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 'b':
  case 'B':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    break;
  case 'c':
  case 'C':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 'd':
  case 'D':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    break;
  case 'e':
  case 'E':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 'f':
  case 'F':
    /* code */
    break;
  case 'g':
  case 'G':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 'h':
  case 'H':
    /* code */
    break;
  case 'i':
  case 'I':
    led_matrix__set_pixel(row + 0, column, color);
    led_matrix__set_pixel(row + 1, column, color);
    led_matrix__set_pixel(row + 2, column, color);
    led_matrix__set_pixel(row + 3, column, color);
    led_matrix__set_pixel(row + 4, column, color);
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
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 4, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 1, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 4, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 4, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 4, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 4, color);
    break;
  case 'n':
  case 'N':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 4, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 1, color);
    led_matrix__set_pixel(row + 1, column + 4, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 4, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 4, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 4, color);
    break;
  case 'o':
  case 'O':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 'p':
  case 'P':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    break;
  case 'q':
  case 'Q':
    /* code */
    break;
  case 'r':
  case 'R':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 's':
  case 'S':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 3, color);
    break;
  case 't':
  case 'T':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 1, color);
    led_matrix__set_pixel(row + 0, column + 2, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 0, column + 4, color);
    led_matrix__set_pixel(row + 1, column + 2, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 3, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    break;
  case 'u':
  case 'U':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 3, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 1, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    break;
  case 'v':
  case 'V':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 4, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 4, color);
    led_matrix__set_pixel(row + 2, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 3, color);
    led_matrix__set_pixel(row + 3, column + 1, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    break;
  case 'w':
  case 'W':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 1, column + 0, color);
    led_matrix__set_pixel(row + 2, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 0, color);
    led_matrix__set_pixel(row + 4, column + 0, color);
    led_matrix__set_pixel(row + 3, column + 1, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 3, column + 3, color);
    led_matrix__set_pixel(row + 0, column + 4, color);
    led_matrix__set_pixel(row + 1, column + 4, color);
    led_matrix__set_pixel(row + 2, column + 4, color);
    led_matrix__set_pixel(row + 3, column + 4, color);
    led_matrix__set_pixel(row + 4, column + 4, color);
    break;
  case 'x':
  case 'X':
    /* code */
    break;
  case 'y':
  case 'Y':
    led_matrix__set_pixel(row + 0, column + 0, color);
    led_matrix__set_pixel(row + 0, column + 4, color);
    led_matrix__set_pixel(row + 1, column + 1, color);
    led_matrix__set_pixel(row + 1, column + 3, color);
    led_matrix__set_pixel(row + 2, column + 2, color);
    led_matrix__set_pixel(row + 3, column + 2, color);
    led_matrix__set_pixel(row + 4, column + 2, color);
    break;
  case 'z':
  case 'Z':
    /* code */
    break;
  default:
    break;
  }
}

void led_matrix_basic_graphics__display_word_score(uint8_t row, uint8_t column, led_color_e color) {
  led_matrix_basic_graphics__display_letter(row, column, 'S', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'C', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'O', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'R', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'E', color);
  // SCORE : <score> below displays the colon
  column += 6;
  led_matrix__set_pixel(row + 0, column, color);
  led_matrix__set_pixel(row + 1, column, color);
  led_matrix__set_pixel(row + 3, column, color);
  led_matrix__set_pixel(row + 4, column, color);
  column += 1;
  led_matrix__set_pixel(row + 0, column, color);
  led_matrix__set_pixel(row + 1, column, color);
  led_matrix__set_pixel(row + 3, column, color);
  led_matrix__set_pixel(row + 4, column, color);
}

void led_matrix_basic_graphics__display_word_press_red_button(uint8_t row, uint8_t column, led_color_e color) {
  led_matrix_basic_graphics__display_letter(row, column, 'P', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'R', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'E', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'S', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'S', color);
  column += 8;
  led_matrix_basic_graphics__display_letter(row, column, 'R', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'E', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'D', color);
  row += 7;
  column = 0;
  column += 16;
  led_matrix_basic_graphics__display_letter(row, column, 'B', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'U', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'T', color);
  column += 6;
  led_matrix_basic_graphics__display_letter(row, column, 'T', color);
  column += 6;
  led_matrix_basic_graphics__display_letter(row, column, 'O', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'N', color);
}

void led_matrix_basic_graphics__display_word_game_over(uint8_t row, uint8_t column, led_color_e color) {
  led_matrix_basic_graphics__display_letter(row, column, 'G', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'A', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'M', color);
  column += 6;
  led_matrix_basic_graphics__display_letter(row, column, 'E', color);
  column += 8;
  led_matrix_basic_graphics__display_letter(row, column, 'O', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'V', color);
  column += 6;
  led_matrix_basic_graphics__display_letter(row, column, 'E', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'R', color);
}

void led_matrix_basic_graphics__display_word_you_win(uint8_t row, uint8_t column, led_color_e color) {
  led_matrix_basic_graphics__display_letter(row, column, 'Y', color);
  column += 6;
  led_matrix_basic_graphics__display_letter(row, column, 'O', color);
  column += 5;
  led_matrix_basic_graphics__display_letter(row, column, 'U', color);
  column += 8;
  led_matrix_basic_graphics__display_letter(row, column, 'W', color);
  column += 6;
  led_matrix_basic_graphics__display_letter(row, column, 'I', color);
  column += 2;
  led_matrix_basic_graphics__display_letter(row, column, 'N', color);

  // Display exclamation point
  column += 8;
  led_matrix__set_pixel(row + 0, column, color);
  led_matrix__set_pixel(row + 1, column, color);
  led_matrix__set_pixel(row + 2, column, color);
  led_matrix__set_pixel(row + 4, column, color);
}