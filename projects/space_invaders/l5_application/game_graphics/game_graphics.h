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

void game_graphics__display_splash_screen(void);
void game_graphics__display_game_over_screen(void);
void game_graphics__display_laser_cannon(uint8_t row, uint8_t column, led_color_e color);
void game_graphics__display_octopus(uint8_t row, uint8_t column, led_color_e color);         // Large Invader
void game_graphics__display_octopus_walking(uint8_t row, uint8_t column, led_color_e color); // Lager Invader Walking
void game_graphics__display_crab(uint8_t row, uint8_t column, led_color_e color);            // Medium Invader
void game_graphics__display_crab_walking(uint8_t row, uint8_t column, led_color_e color);    // Medium Invader Walking
void game_graphics__display_squid(uint8_t row, uint8_t column, led_color_e color);           // Small Invader
void game_graphics__display_squid_walking(uint8_t row, uint8_t column, led_color_e color);   // Small Invader Walking
void game_graphics__display_ufo(uint8_t row, uint8_t column, led_color_e color);
void game_graphics__display_enemy_bullet(uint8_t row, uint8_t column, led_color_e color);
void game_graphics__display_laser_cannon_bullet(uint8_t row, uint8_t column, led_color_e color);
void game_graphics__display_explotion(uint8_t row, uint8_t column, led_color_e color);