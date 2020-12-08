/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Main Module Header */
#include "game_logic.h"

/* Standard Includes */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* External Includes */
#include "game_graphics.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"

/***********************************************************************************************************************
 *
 *                                                   D E F I N E S
 *
 **********************************************************************************************************************/
#define MAX_ROW_OF_ENEMIES 3
#define MAX_NUM_OF_ENEMIES 4
#define MAX_NUM_OF_CANNON_BULLETS 9 //(64 rows - 13 rows from cannon)/5
#define MAX_NUM_OF_ENEMY_BULLETS 4
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

static bool is_game_over = false;

static gpio_s joystick_left, joystick_right, start_button, shooting_button;

static const int laser_cannon_start_column_position = 26;
static const int laser_cannon_start_row_position = 53;

static const int laser_cannon_row_boundary = 7;
static const int laser_cannon_column_boundary = 13;
static const int octopus_row_boundary = 8;
static const int octopus_column_boundary = 12;
static const int crab_row_boundary = 8;
static const int crab_column_boundary = 11;
static const int squid_row_boundary = 8;
static const int squid_column_boundary = 8;

static const int speed_delay_ms = 30;
static const int number_of_enemies_left;

static const uint8_t column_min_boundary = 0;
static const uint8_t column_max_boundary = 64;

static const size_t enemy_column_delta = 1;
static const size_t enemy_row_delta = 1;

static game_object_s laser_cannon = {
    .column_position = laser_cannon_start_column_position,
    .row_position = laser_cannon_start_row_position,
    .moving_direction = 0,
    .width = laser_cannon_column_boundary,
    .height = laser_cannon_row_boundary,
    .subtype = true,
    .entity = LASER_CANNON,
    .color = ELECTRIC_BLUE,
    .is_valid = true,
};

static game_object_s enemies_array[MAX_ROW_OF_ENEMIES][MAX_NUM_OF_ENEMIES];

static game_object_s enemy_bullets_array[MAX_NUM_OF_ENEMY_BULLETS] = {{.column_position = 53,
                                                                       .row_position = 26,
                                                                       .moving_direction = DOWN,
                                                                       .width = 1,
                                                                       .height = 5,
                                                                       .subtype = true,
                                                                       .entity = ENEMY_BULLET,
                                                                       .color = YELLOW,
                                                                       .is_valid = false}};

static const int game_over_row_boundary = laser_cannon_start_row_position - laser_cannon_row_boundary;

static game_object_s cannon_bullets_array[MAX_NUM_OF_CANNON_BULLETS] = {{.column_position = 53,
                                                                         .row_position = 26,
                                                                         .moving_direction = UP,
                                                                         .width = 1,
                                                                         .height = 5,
                                                                         .subtype = true,
                                                                         .entity = LASER_CANNON_BULLET,
                                                                         .color = RED,
                                                                         .is_valid = false}};

/***********************************************************************************************************************
 *
 *                                         P R I V A T E   F U N C T I O N S
 *
 **********************************************************************************************************************/

void game_logic__private_spawn_octupus(int starting_row) {
  uint8_t column_offset = 5;
  uint8_t row_offset = 12;
  uint8_t offset_between_rows = 1;
  uint8_t offset_between_columns = 2;
  uint8_t row_position = row_offset + ((offset_between_rows + octopus_row_boundary) * starting_row);
  for (size_t i = 0; i < 4; i++) {
    enemies_array[starting_row][i].column_position =
        ((octopus_column_boundary + offset_between_columns) * i) + column_offset;
    enemies_array[starting_row][i].row_position = row_position;
    enemies_array[starting_row][i].moving_direction = 0;
    enemies_array[starting_row][i].width = octopus_column_boundary;
    enemies_array[starting_row][i].height = octopus_row_boundary;
    enemies_array[starting_row][i].entity = OCTOPUS;
    enemies_array[starting_row][i].color = PURPLE;
    enemies_array[starting_row][i].is_valid = true;
    enemies_array[starting_row][i].subtype = true;
  }
}

void game_logic__private_spawn_crab(int starting_row) {
  uint8_t column_offset = 5;
  uint8_t row_offset = 12;
  uint8_t offset_between_rows = 1;
  uint8_t offset_between_columns = 3;
  uint8_t row_position = row_offset + ((offset_between_rows + crab_row_boundary) * starting_row);
  for (size_t i = 0; i < 4; i++) {
    enemies_array[starting_row][i].column_position =
        ((crab_column_boundary + offset_between_columns) * i) + column_offset;
    enemies_array[starting_row][i].row_position = row_position;
    enemies_array[starting_row][i].moving_direction = 0;
    enemies_array[starting_row][i].width = crab_column_boundary;
    enemies_array[starting_row][i].height = crab_row_boundary;
    enemies_array[starting_row][i].entity = CRAB;
    enemies_array[starting_row][i].color = ELECTRIC_BLUE;
    enemies_array[starting_row][i].is_valid = true;
    enemies_array[starting_row][i].subtype = true;
  }
}

void game_logic__private_spawn_squid(int starting_row) {
  uint8_t column_offset = 6;
  uint8_t row_offset = 12;
  uint8_t offset_between_rows = 1;
  uint8_t offset_between_columns = 6;
  uint8_t row_position = row_offset + ((offset_between_rows + squid_row_boundary) * starting_row);
  for (size_t i = 0; i < 4; i++) {
    enemies_array[starting_row][i].column_position =
        ((squid_column_boundary + offset_between_columns) * i) + column_offset;
    enemies_array[starting_row][i].row_position = row_position;
    enemies_array[starting_row][i].moving_direction = 0;
    enemies_array[starting_row][i].width = squid_column_boundary;
    enemies_array[starting_row][i].height = squid_row_boundary;
    enemies_array[starting_row][i].entity = SQUID;
    enemies_array[starting_row][i].color = GREEN;
    enemies_array[starting_row][i].is_valid = true;
    enemies_array[starting_row][i].subtype = true;
  }
}

static void game_logic__private_clear_enemy(game_object_s *enemy) {
  switch (enemy->entity) {
  case SQUID:
    game_graphics__display_squid(enemy->row_position, enemy->column_position, BLACK, !enemy->subtype);
    break;
  case CRAB:
    game_graphics__display_crab(enemy->row_position, enemy->column_position, BLACK, !enemy->subtype);
    break;
  case OCTOPUS:
    game_graphics__display_octopus(enemy->row_position, enemy->column_position, BLACK, !enemy->subtype);
    break;
  default:
    break;
  }
}

static void game_logic__private_display_enemy(game_object_s *enemy) {
  switch (enemy->entity) {
  case SQUID:
    game_graphics__display_squid(enemy->row_position, enemy->column_position, enemy->color, enemy->subtype);
    break;
  case CRAB:
    game_graphics__display_crab(enemy->row_position, enemy->column_position, enemy->color, enemy->subtype);
    break;
  case OCTOPUS:
    game_graphics__display_octopus(enemy->row_position, enemy->column_position, enemy->color, enemy->subtype);
    break;
  default:
    break;
  }
  enemy->subtype = !enemy->subtype;
}

static void game_logic__private_determine_enemy_movement(game_object_s *enemy) {
  if (enemy[0].moving_direction == LEFT) {
    if (enemy[0].column_position - enemy_column_delta > column_min_boundary) {
      for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
        game_logic__private_clear_enemy(&enemy[i]);
        enemy[i].column_position -= enemy_column_delta;
      }
    } else {
      for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
        game_logic__private_clear_enemy(&enemy[i]);
        if (enemy[i].row_position != game_over_row_boundary) {
          enemy[i].row_position += enemy_row_delta;
          enemy[i].moving_direction = RIGHT;
        } else {
          is_game_over = true;
        }
      }
    }
  } else if (enemy[MAX_NUM_OF_ENEMIES - 1].moving_direction == RIGHT) {
    if (enemy[MAX_NUM_OF_ENEMIES - 1].column_position + enemy_column_delta + enemy[MAX_NUM_OF_ENEMIES - 1].width <
        column_max_boundary) {
      for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
        game_logic__private_clear_enemy(&enemy[i]);
        enemy[i].column_position += enemy_column_delta;
      }
    } else {
      for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
        game_logic__private_clear_enemy(&enemy[i]);
        if (enemy[i].row_position != game_over_row_boundary) {
          enemy[i].row_position += enemy_row_delta;
          enemy[i].moving_direction = LEFT;
        } else {
          is_game_over = true;
        }
      }
    }
  } else {
    // Do nothing
  }
}

void game_logic__private_detect_bullet_collision_from_enemy(game_object_s *enemy) {
  // TODO
}

void game_logic__private_detect_bullet_collision_from_laser_cannon_to_enemy(game_object_s *enemy) {
  // TODO
}

// TODO: Add score counter logic

bool game_logic__private_detect_enemy_bullet_is_valid(game_object_s *enemy) {
  bool valid = false;
  uint8_t offset_enemy_bullet = 2;
  uint8_t bullet_shooting_location = ((enemy->column_position + enemy->width) / 2);
  for (int i = 0; i < MAX_ROW_OF_ENEMIES; i++) {
    for (int j = 0; j < MAX_NUM_OF_ENEMIES; j++) {
      if ((enemies_array[i][j].column_position == enemy->column_position) && (i == 0)) {
        valid = true;
        return valid; // we are in the first column, so shoot away
      } else if (enemies_array[i][j].column_position == enemy->column_position) {
        valid = false;
        break; // if we are in the same column as the enemies, then break. Dont check that column and increment to next
               // column
      } else if (((enemies_array[i][j].column_position) <= bullet_shooting_location) &&
                 (bullet_shooting_location <=
                  enemies_array[i][j].column_position + enemies_array[i][j].width + offset_enemy_bullet)) {
        valid = false; // this states that they are in the same column bounds
      } else {
        valid = true; // none of the cases matched, so the bullet can be shot
      }
    }
  }
  return valid;
}

void game_logic__private_enemy_shoot_bullet(game_object_s *enemy) {
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    if (enemy_bullets_array[i].is_valid == 0) {
      enemy_bullets_array[i].is_valid = 1;
      enemy_bullets_array[i].row_position = enemy[0].row_position;
      printf("enemy bullet Row value: %i\n", enemy[0].row_position);
      enemy_bullets_array[i].column_position = (enemy[0].column_position + enemy->width) / 2;
      printf("enemy bullet Column value: %i\n", enemy[0].column_position);
      break;
    }
  }
}

void game_logic__private_update_enemy_bullet_location(void) {
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    if (enemy_bullets_array[i].is_valid == 1) {
      game_graphics__display_enemy_bullet(enemy_bullets_array[i].row_position, enemy_bullets_array[i].column_position,
                                          BLACK);
      if (enemy_bullets_array[i].row_position != 64) {
        enemy_bullets_array[i].row_position++;
        game_graphics__display_enemy_bullet(enemy_bullets_array[i].row_position, enemy_bullets_array[i].column_position,
                                            enemy_bullets_array[i].color);
      } else {
        enemy_bullets_array[i].is_valid = 0;
      }
    }
  }
}

/***********************************************************************************************************************
 *
 *                                          P U B L I C   F U N C T I O N S
 *
 **********************************************************************************************************************/

void game_logic__initialize(void) {
  joystick_left = gpio__construct_as_input(GPIO__PORT_1, 30);
  joystick_right = gpio__construct_as_input(GPIO__PORT_1, 31);
  start_button = gpio__construct_as_input(GPIO__PORT_0, 26);
  shooting_button = gpio__construct_as_input(GPIO__PORT_0, 25);

  gpio__reset(joystick_right);
  gpio__reset(joystick_left);
  gpio__reset(start_button);
  gpio__reset(shooting_button);

  memset(enemies_array, 0, sizeof(enemies_array));
  (void)game_logic__respawn_enemies();
}

void game_logic__move_laser_cannon(void) {
  if (gpio__get(joystick_left) == 1 && gpio__get(joystick_right) == 0) {
    game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, BLACK);
    if (laser_cannon.column_position > column_min_boundary) {
      laser_cannon.column_position--;
    }
  } else if (gpio__get(joystick_right) == 1 && gpio__get(joystick_left) == 0) {
    game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, BLACK);
    if (laser_cannon.column_position < column_max_boundary - laser_cannon.width) {
      laser_cannon.column_position++;
    }
  } else {
    // Do nothing
  }
  game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, laser_cannon.color);
}

void game_logic__move_enemies(void) {
  if (!is_game_over) {
    for (size_t i = 0; i < MAX_ROW_OF_ENEMIES; i++) {
      game_logic__private_determine_enemy_movement(&enemies_array[i]);
      for (size_t j = 0; j < MAX_NUM_OF_ENEMIES; j++) {
        game_logic__private_display_enemy(&enemies_array[i][j]);
      }
    }
    vTaskDelay(8 * speed_delay_ms); // TODO: Enemies speed = (Num_enemies_left * Const delay in ms)
  }
}

bool game_logic__get_game_over_status(void) { return is_game_over; }
void game_logic__set_game_over_status(bool status) { is_game_over = status; }

void game_logic__shoot_bullet(void) {
  const uint8_t offset_cannon_row_center = 6;
  const uint8_t offset_cannon_column_center = 6;
  for (size_t i = 0; i < MAX_NUM_OF_CANNON_BULLETS; i++) {
    if (cannon_bullets_array[i].is_valid == 0) {
      cannon_bullets_array[i].is_valid = 1;
      cannon_bullets_array[i].row_position = laser_cannon.row_position + offset_cannon_row_center;
      cannon_bullets_array[i].column_position = laser_cannon.column_position + offset_cannon_column_center;
      break;
    }
  }
}

void game_logic__update_bullet_location(void) {
  for (size_t i = 0; i < MAX_NUM_OF_CANNON_BULLETS; i++) {
    if (cannon_bullets_array[i].is_valid == 1) {
      game_graphics__display_laser_cannon_bullet(cannon_bullets_array[i].row_position,
                                                 cannon_bullets_array[i].column_position, BLACK);
      if (cannon_bullets_array[i].row_position != 0) {
        cannon_bullets_array[i].row_position--;
        game_graphics__display_laser_cannon_bullet(cannon_bullets_array[i].row_position,
                                                   cannon_bullets_array[i].column_position,
                                                   cannon_bullets_array[i].color);
      } else {
        cannon_bullets_array[i].is_valid = 0;
      }
    }
  }
}

void game_logic__check_valid_enemy_to_shoot_bullet(void) {
  uint8_t enemy_row = (rand() % MAX_ROW_OF_ENEMIES) + 1; // gives time between 1-3
  uint8_t enemy_column = (rand() % MAX_NUM_OF_ENEMIES) + 1;
  if (enemies_array[enemy_row][enemy_column].is_valid == true) {
    if (game_logic__private_detect_enemy_bullet_is_valid(&enemies_array[enemy_row][enemy_column])) {
      game_logic__private_enemy_shoot_bullet(&enemies_array[enemy_row][enemy_column]);
    }
  }
  game_logic__private_update_enemy_bullet_location();
}

void game_logic__respawn_enemies(void) {
  uint8_t squid_row = 0;
  uint8_t crab_row = 1;
  uint8_t octopus_row = 2;

  memset(enemies_array, 0, sizeof(enemies_array));

  game_logic__private_spawn_squid(squid_row);
  game_logic__private_spawn_crab(crab_row);
  game_logic__private_spawn_octupus(octopus_row);
}
