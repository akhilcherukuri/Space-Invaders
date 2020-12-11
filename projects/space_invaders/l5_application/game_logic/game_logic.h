#pragma once
/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Standard Includes */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* External Includes */

/* Module Includes */
#include "led_matrix.h"

/***********************************************************************************************************************
 *
 *                                                   D E F I N E S
 *
 **********************************************************************************************************************/
#define ENTITY_SUBTYPE_1 true
#define ENTITY_SUBTYPE_2 false
/***********************************************************************************************************************
 *
 *                                                  T Y P E D E F S
 *
 **********************************************************************************************************************/
typedef enum { LASER_CANNON, OCTOPUS, CRAB, SQUID, LASER_CANNON_BULLET, ENEMY_BULLET } enemy_entity_e;
typedef enum { LEFT, RIGHT, UP, DOWN } enemy_direction_e;
typedef bool entity_subtype_t;

typedef struct {
  int column_position;
  int row_position;
  enemy_direction_e moving_direction;
  int width;
  int height;
  enemy_entity_e entity;
  entity_subtype_t subtype;
  led_color_e color;
  bool is_valid;
  int points;
} game_object_s;

/***********************************************************************************************************************
 *
 *                                     F U N C T I O N   D E C L A R A T I O N S
 *
 **********************************************************************************************************************/
void game_logic__initialize(void);
void game_logic__reset_game(void);
void game_logic__respawn_enemies(void);
void game_logic__respawn_enemies_bullets(void);
void game_logic__respawn_laser_cannon_bullets(void);
void game_logic__move_laser_cannon(void);
void game_logic__move_enemies(void);
void game_logic__shoot_bullet(void);
void game_logic__update_bullet_location(void);
void game_logic__check_valid_enemy_to_shoot_bullet(void);
bool game_logic__get_game_won_status(void);
void game_logic__set_game_won_status(bool status);
bool game_logic__get_game_over_status(void);
void game_logic__set_game_over_status(bool status);
int game_logic__get_game_overall_score(void);
void game_logic__set_game_overall_score(int score);
int game_logic__get_laser_cannon_lives(void);