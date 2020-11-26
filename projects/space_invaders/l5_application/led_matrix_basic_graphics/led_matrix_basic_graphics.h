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
#include "led_matrix.h"

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
 *                                     F U N C T I O N   D E C L A R A T I O N S
 *
 **********************************************************************************************************************/

void led_matrix_basic_graphics__display_number(uint8_t row, uint8_t column, uint8_t number, led_color_e color);
void led_matrix_basic_graphics__display_letter(uint8_t row, uint8_t column, char letter, led_color_e color);

void led_matrix_basic_graphics__display_word_score(uint8_t row, uint8_t column);
void led_matrix_basic_graphics__display_word_press_start(uint8_t row, uint8_t column);
void led_matrix_basic_graphics__display_word_game_over(uint8_t row, uint8_t column);