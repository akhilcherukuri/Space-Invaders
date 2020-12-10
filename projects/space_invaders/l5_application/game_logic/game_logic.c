/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Main Module Header */
#include "game_logic.h"

/* Standard Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* External Includes */
#include "game_graphics.h"
#include "gpio.h"
#include "led_matrix_basic_graphics.h"

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
#define MAX_NUM_OF_ENEMY_BULLETS 2
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

static bool is_game_won = false;
static bool is_game_over = false;
static bool is_game_paused = false;

static int number_of_lives = 3;

static int number_of_enemies_left;
static int outer_most_enemy_index = 0;

static const int score_board_number_column_offset = 31;
static const int score_board_number_row_offset = 5; // 3-11 rows are used
static uint8_t overall_game_score = 0;
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
static const int game_over_row_boundary = laser_cannon_start_row_position - laser_cannon_row_boundary;

static const TickType_t enemies_speed_delay_ms = 20;

static const uint8_t led_matrix_left_boundary = 0;
static const uint8_t led_matrix_right_boundary = 64;
static const uint8_t led_matrix_top_boundary = 10;
static const uint8_t led_matrix_bottom_boundary = 64;

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
    .points = 0,
};

static game_object_s enemies_array[MAX_ROW_OF_ENEMIES][MAX_NUM_OF_ENEMIES];

static game_object_s enemy_bullets_array[MAX_NUM_OF_ENEMY_BULLETS];

static game_object_s cannon_bullets_array[MAX_NUM_OF_CANNON_BULLETS];

/***********************************************************************************************************************
 *
 *                                         P R I V A T E   F U N C T I O N S
 *
 **********************************************************************************************************************/

void spawn_logic__private_spawn_enemy_bullets() {
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    enemy_bullets_array[i].column_position = 0;
    enemy_bullets_array[i].row_position = 0;
    enemy_bullets_array[i].moving_direction = DOWN;
    enemy_bullets_array[i].width = 1;
    enemy_bullets_array[i].height = 5;
    enemy_bullets_array[i].subtype = true;
    enemy_bullets_array[i].entity = ENEMY_BULLET;
    enemy_bullets_array[i].color = YELLOW;
    enemy_bullets_array[i].is_valid = false;
    enemy_bullets_array[i].points = 0;
  }
}

void game_logic__private_spawn_octupus(int starting_row) {
  uint8_t column_offset = 5;
  uint8_t row_offset = 12;
  uint8_t offset_between_rows = 1;
  uint8_t offset_between_columns = 2;
  uint8_t row_position = row_offset + ((offset_between_rows + octopus_row_boundary) * starting_row);
  for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
    enemies_array[starting_row][i].column_position =
        ((octopus_column_boundary + offset_between_columns) * i) + column_offset;
    enemies_array[starting_row][i].row_position = row_position;
    enemies_array[starting_row][i].moving_direction = LEFT;
    enemies_array[starting_row][i].width = octopus_column_boundary;
    enemies_array[starting_row][i].height = octopus_row_boundary;
    enemies_array[starting_row][i].subtype = true;
    enemies_array[starting_row][i].entity = OCTOPUS;
    enemies_array[starting_row][i].color = PURPLE;
    enemies_array[starting_row][i].is_valid = true;
    enemies_array[starting_row][i].points = 10;
  }
}

void game_logic__private_spawn_crab(int starting_row) {
  uint8_t column_offset = 5;
  uint8_t row_offset = 12;
  uint8_t offset_between_rows = 1;
  uint8_t offset_between_columns = 3;
  uint8_t row_position = row_offset + ((offset_between_rows + crab_row_boundary) * starting_row);
  for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
    enemies_array[starting_row][i].column_position =
        ((crab_column_boundary + offset_between_columns) * i) + column_offset;
    enemies_array[starting_row][i].row_position = row_position;
    enemies_array[starting_row][i].moving_direction = LEFT;
    enemies_array[starting_row][i].width = crab_column_boundary;
    enemies_array[starting_row][i].height = crab_row_boundary;
    enemies_array[starting_row][i].subtype = true;
    enemies_array[starting_row][i].entity = CRAB;
    enemies_array[starting_row][i].color = ELECTRIC_BLUE;
    enemies_array[starting_row][i].is_valid = true;
    enemies_array[starting_row][i].points = 20;
  }
}

void game_logic__private_spawn_squid(int starting_row) {
  uint8_t column_offset = 5;
  uint8_t row_offset = 12;
  uint8_t offset_between_rows = 1;
  uint8_t offset_between_columns = 6;
  uint8_t row_position = row_offset + ((offset_between_rows + squid_row_boundary) * starting_row);
  for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
    enemies_array[starting_row][i].column_position =
        ((squid_column_boundary + offset_between_columns) * i) + column_offset;
    enemies_array[starting_row][i].row_position = row_position;
    enemies_array[starting_row][i].moving_direction = LEFT;
    enemies_array[starting_row][i].width = squid_column_boundary;
    enemies_array[starting_row][i].height = squid_row_boundary;
    enemies_array[starting_row][i].subtype = true;
    enemies_array[starting_row][i].entity = SQUID;
    enemies_array[starting_row][i].color = GREEN;
    enemies_array[starting_row][i].is_valid = true;
    enemies_array[starting_row][i].points = 30;
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

static int game_logic__private_determine_outer_most_valid_enemy(game_object_s *row) {
  int index = 0;
  if (row[0].moving_direction == LEFT) {
    for (size_t i = 0; i < MAX_NUM_OF_ENEMIES; i++) {
      if (row[i].is_valid) {
        index = i;
        break;
      }
    }
  } else {
    for (size_t i = MAX_NUM_OF_ENEMIES; i > 0; i--) {
      if (row[i - 1].is_valid) {
        index = i - 1;
        break;
      }
    }
  }
  return index;
}

static bool game_logic__private_horizontal_collision_imminent(void) {
  bool enemy_will_hit_wall = false;

  for (size_t row = 0; row < MAX_ROW_OF_ENEMIES && !enemy_will_hit_wall; row++) {
    outer_most_enemy_index = game_logic__private_determine_outer_most_valid_enemy(&enemies_array[row]);
    if (enemies_array[row][outer_most_enemy_index].moving_direction == LEFT) {
      if (enemies_array[row][outer_most_enemy_index].column_position - enemy_column_delta > led_matrix_left_boundary) {
        // No collision. Do nothing
      } else {
        enemy_will_hit_wall = true;
        break;
      }
    } else if (enemies_array[row][outer_most_enemy_index].moving_direction == RIGHT) {
      if (enemies_array[row][outer_most_enemy_index].column_position + enemy_column_delta +
              enemies_array[row][outer_most_enemy_index].width <
          led_matrix_right_boundary) {
        // No collision. Do nothing
      } else {
        enemy_will_hit_wall = true;
        break;
      }
    } else {
      // Do nothing
    }
  }
  return enemy_will_hit_wall;
}

static void game_logic__private_game_over(void) {
  number_of_enemies_left = 12;
  number_of_lives = 3;
  is_game_over = true;
  vTaskSuspend(NULL);
}

static bool game_logic__private_determine_enemy_movement(void) {
  bool enemy_will_hit_wall = game_logic__private_horizontal_collision_imminent();

  if (enemy_will_hit_wall) {
    // puts("Imminent collision"); // TODO Remove after testing
    for (size_t row = 0; row < MAX_ROW_OF_ENEMIES; row++) {
      if (enemies_array[row][0].moving_direction == LEFT) {
        for (size_t col = 0; col < MAX_NUM_OF_ENEMIES; col++) {
          // printf("Direction left: Enemy in row %d col %d Valid: %s Collision Index: %d Row Pos: %d Col Pos: %d\n",
          // row,
          //        col, enemies_array[row][col].is_valid ? "true" : "false", outer_most_enemy_index,
          //        enemies_array[row][col].row_position, enemies_array[row][col].column_position); // TODO Remove after
          //        testing
          game_logic__private_clear_enemy(&enemies_array[row][col]);
          enemies_array[row][col].moving_direction = RIGHT;
          enemies_array[row][col].row_position += enemy_row_delta;
          if ((enemies_array[row][col].row_position >= game_over_row_boundary) && enemies_array[row][col].is_valid) {
            return false;
          }
        }
      } else if (enemies_array[row][0].moving_direction == RIGHT) {
        for (size_t col = 0; col < MAX_NUM_OF_ENEMIES; col++) {
          // printf("Direction right: Enemy in row %d col %d Valid: %s Collision Index: %d Row Pos: %d Col Pos: %d\n",
          // row,
          //        col, enemies_array[row][col].is_valid ? "true" : "false", outer_most_enemy_index,
          //        enemies_array[row][col].row_position, enemies_array[row][col].column_position); // TODO Remove after
          //        testing
          game_logic__private_clear_enemy(&enemies_array[row][col]);
          enemies_array[row][col].row_position += enemy_row_delta;
          enemies_array[row][col].moving_direction = LEFT;
          if ((enemies_array[row][col].row_position >= game_over_row_boundary) && enemies_array[row][col].is_valid) {
            return false;
          }
        }
      }
    }
  } else {
    for (size_t row = 0; row < MAX_ROW_OF_ENEMIES; row++) {
      if (enemies_array[row][0].moving_direction == LEFT) {
        for (size_t col = 0; col < MAX_NUM_OF_ENEMIES; col++) {
          game_logic__private_clear_enemy(&enemies_array[row][col]);
          enemies_array[row][col].column_position -= enemy_column_delta;
        }
      } else if (enemies_array[row][0].moving_direction == RIGHT) {
        for (size_t col = 0; col < MAX_NUM_OF_ENEMIES; col++) {
          game_logic__private_clear_enemy(&enemies_array[row][col]);
          enemies_array[row][col].column_position += enemy_column_delta;
        }
      }
    }
  }
  return true;
}

bool game_logic__decrease_laser_cannon_lives(void) {
  static uint8_t available_lives = 3;
  bool is_still_alive = false;
  --available_lives;
  is_still_alive = (available_lives > 0) ? true : false;
  return is_still_alive;
}

void game_logic__private_detect_bullet_collision_from_enemy(void) {
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    if (enemy_bullets_array[i].is_valid) {
      puts("I AM HERE\n");
      if ((enemy_bullets_array[i].column_position <= laser_cannon.column_position) &&
          (enemy_bullets_array[i].column_position <= laser_cannon.column_position + laser_cannon.width)) {
        enemy_bullets_array[i].is_valid = false;
        game_graphics__display_enemy_bullet(enemy_bullets_array[i].row_position, enemy_bullets_array[i].column_position,
                                            BLACK);
        game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, BLACK);
        game_graphics__display_explosion(laser_cannon.row_position, laser_cannon.column_position, YELLOW);
        // TODO: temporarely suspend the move enemies task, enemy shooting task, laser cannon task
        // done with line below
        is_game_paused = true;
        // TODO: Add delay to show laser cannon explosion
        // done delay below
        vTaskDelay(15);
        game_graphics__display_explosion(laser_cannon.row_position, laser_cannon.column_position, BLACK);
        is_game_paused = false;
        // TODO: respawn laser cannon to default position and resume task
        // i believe this is done based off the commands below
        led_matrix_basic_graphics__display_number(5, 56, number_of_lives, BLACK);
        number_of_lives--;
        led_matrix_basic_graphics__display_number(5, 56, number_of_lives, ELECTRIC_BLUE);
        break;
        // if (number_of_lives > 0) { // calls function which decrements available lives and returns
        //                            // it
        //   game_logic__private_game_over();
        // }
      }
    }
  }
}

void game_logic__private_detect_bullet_collision_from_laser_cannon_to_enemy(void) {
  for (size_t i = 0; i < MAX_NUM_OF_CANNON_BULLETS; i++) {
    if (cannon_bullets_array[i].is_valid == 1) {
      for (size_t j = MAX_ROW_OF_ENEMIES; j > 0; j--) {
        for (size_t k = 0; k < MAX_NUM_OF_ENEMIES; k++) {
          if (enemies_array[j - 1][k].is_valid) {
            if ((enemies_array[j - 1][k].column_position <= cannon_bullets_array[i].column_position) &&
                (cannon_bullets_array[i].column_position <=
                 (enemies_array[j - 1][k].column_position + enemies_array[j - 1][k].width))) {
              cannon_bullets_array[i].is_valid = false;
              game_graphics__display_laser_cannon_bullet(cannon_bullets_array[i].row_position,
                                                         cannon_bullets_array[i].column_position, BLACK);
              enemies_array[j - 1][k].is_valid = false;
              number_of_enemies_left--;
              game_graphics__display_score_board(score_board_number_row_offset, score_board_number_column_offset, BLACK,
                                                 overall_game_score);
              overall_game_score += enemies_array[j - 1][k].points;
              game_graphics__display_score_board(score_board_number_row_offset, score_board_number_column_offset,
                                                 ELECTRIC_BLUE, overall_game_score);
              game_graphics__display_explosion(enemies_array[j - 1][k].row_position,
                                               enemies_array[j - 1][k].column_position, RED);
              // TODO: Suspend move enemies and shooting task during explotion animation
              // done
              is_game_paused = true;
              vTaskDelay(15);
              game_graphics__display_explosion(enemies_array[j - 1][k].row_position,
                                               enemies_array[j - 1][k].column_position, BLACK);
              // TODO: Resume move enemies and shooting task after explotion animation
              // done with command above
              is_game_paused = false;
              return;
            }
          }
        }
      }
    }
  }
}

bool game_logic__private_detect_enemy_bullet_is_valid(game_object_s *enemy, uint8_t row) {
  bool valid = false;
  int enemy_possible_routes = 0;
  int enemy_checks_done = 0;
  uint8_t bullet_shooting_location = ((enemy->column_position + (enemy->width / 2)));
  if (row == 2) {
    valid = true;
  } else if (row == 1) {
    for (int i = 0; i < MAX_ROW_OF_ENEMIES; i++) {
      if (enemies_array[2][i].is_valid == 1) {
        if (((enemies_array[2][i].column_position) == bullet_shooting_location) &&
            (bullet_shooting_location == (enemies_array[2][i].column_position + enemies_array[2][i].width))) {
          enemy_possible_routes++;
        }
        enemy_checks_done++;
      }
    }
    if (enemy_possible_routes == enemy_checks_done) {
      valid = true;
    }
  } else {
    for (int i = 0; i < MAX_ROW_OF_ENEMIES - 1; i++) {
      for (int j = 0; j < (MAX_NUM_OF_ENEMIES); j++) {
        if (enemies_array[i + 1][j].is_valid == 1) {
          if (((enemies_array[i + 1][j].column_position) == bullet_shooting_location) &&
              (bullet_shooting_location ==
               ((enemies_array[i + 1][j].column_position + enemies_array[i + 1][j].width)))) {
            enemy_possible_routes++;
          }
          enemy_checks_done++;
        }
      }
    }
    if (enemy_possible_routes == enemy_checks_done) {
      valid = true;
    }
  }
  return valid;
}

void game_logic__private_enemy_shoot_bullet(game_object_s *enemy) {
  uint8_t row_offset_below_enemy = 6;
  uint8_t column_offset_to_center_bullet = 1;
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    if (enemy_bullets_array[i].is_valid == false) {
      enemy_bullets_array[i].is_valid = true;
      enemy_bullets_array[i].row_position = enemy->row_position + row_offset_below_enemy;
      enemy_bullets_array[i].column_position =
          (enemy->column_position + (enemy->width / 2)) - column_offset_to_center_bullet;
      break;
    }
  }
}

void game_logic__private_update_enemy_bullet_location(void) {
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    if (enemy_bullets_array[i].is_valid == true) {
      game_graphics__display_enemy_bullet(enemy_bullets_array[i].row_position, enemy_bullets_array[i].column_position,
                                          BLACK);
      if (enemy_bullets_array[i].row_position != led_matrix_bottom_boundary) {
        enemy_bullets_array[i].row_position++;
        game_graphics__display_enemy_bullet(enemy_bullets_array[i].row_position, enemy_bullets_array[i].column_position,
                                            enemy_bullets_array[i].color);
      } else {
        enemy_bullets_array[i].is_valid = false;
      }
    }
  }
  game_logic__private_detect_bullet_collision_from_enemy();
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
  (void)game_logic__respawn_enemies_bullets();
  (void)game_logic__respawn_laser_cannon_bullets();
  number_of_lives = 3;
  overall_game_score = 0;
}

void game_logic__respawn_enemies(void) {
  uint8_t squid_row = 0;
  uint8_t crab_row = 1;
  uint8_t octopus_row = 2;

  number_of_enemies_left = 12;

  memset(enemies_array, 0, sizeof(enemies_array));

  game_logic__private_spawn_squid(squid_row);
  game_logic__private_spawn_crab(crab_row);
  game_logic__private_spawn_octupus(octopus_row);
}
void game_logic__respawn_enemies_bullets() {
  for (size_t i = 0; i < MAX_NUM_OF_ENEMY_BULLETS; i++) {
    enemy_bullets_array[i].column_position = 0;
    enemy_bullets_array[i].row_position = 0;
    enemy_bullets_array[i].moving_direction = DOWN;
    enemy_bullets_array[i].width = 1;
    enemy_bullets_array[i].height = 5;
    enemy_bullets_array[i].subtype = true;
    enemy_bullets_array[i].entity = ENEMY_BULLET;
    enemy_bullets_array[i].color = YELLOW;
    enemy_bullets_array[i].is_valid = false;
    enemy_bullets_array[i].points = 0;
  }
}

void game_logic__respawn_laser_cannon_bullets() {
  for (size_t i = 0; i < MAX_NUM_OF_CANNON_BULLETS; i++) {
    cannon_bullets_array[i].column_position = 53;
    cannon_bullets_array[i].row_position = 26;
    cannon_bullets_array[i].moving_direction = UP;
    cannon_bullets_array[i].width = 1;
    cannon_bullets_array[i].height = 5;
    cannon_bullets_array[i].subtype = true;
    cannon_bullets_array[i].entity = LASER_CANNON_BULLET;
    cannon_bullets_array[i].color = RED;
    cannon_bullets_array[i].is_valid = false;
    cannon_bullets_array[i].points = 0;
  }
}

void game_logic__move_laser_cannon(void) {
  if (gpio__get(joystick_left) == 1 && gpio__get(joystick_right) == 0) {
    game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, BLACK);
    if (laser_cannon.column_position > led_matrix_left_boundary) {
      laser_cannon.column_position--;
    }
  } else if (gpio__get(joystick_right) == 1 && gpio__get(joystick_left) == 0) {
    game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, BLACK);
    if (laser_cannon.column_position < led_matrix_right_boundary - laser_cannon.width) {
      laser_cannon.column_position++;
    }
  } else {
    // Do nothing
  }
  game_graphics__display_laser_cannon(laser_cannon.row_position, laser_cannon.column_position, laser_cannon.color);
}

void game_logic__move_enemies(void) {
  if (!is_game_paused) {
    if (number_of_enemies_left > 0) {
      if (game_logic__private_determine_enemy_movement()) {
        for (size_t i = 0; i < MAX_ROW_OF_ENEMIES; i++) {
          for (size_t j = 0; j < MAX_NUM_OF_ENEMIES; j++) {
            if (enemies_array[i][j].is_valid) {
              game_logic__private_display_enemy(&enemies_array[i][j]);
            }
          }
        }
        vTaskDelay(number_of_enemies_left * enemies_speed_delay_ms);
      } else {
        game_logic__private_game_over();
      }
    } else {
      is_game_won = true;
    }
  } else {
    // do nothing
  }
}

void game_logic__shoot_bullet(void) {
  const uint8_t offset_cannon_row_center = 8;
  const uint8_t offset_cannon_column_center = 6;
  for (size_t i = 0; i < MAX_NUM_OF_CANNON_BULLETS; i++) {
    if (cannon_bullets_array[i].is_valid == 0) {
      cannon_bullets_array[i].is_valid = 1;
      cannon_bullets_array[i].row_position = laser_cannon.row_position + offset_cannon_row_center;
      cannon_bullets_array[i].column_position = laser_cannon.column_position + offset_cannon_column_center;
      game_graphics__display_laser_cannon_bullet(
          cannon_bullets_array[i].row_position, cannon_bullets_array[i].column_position, cannon_bullets_array[i].color);
      break;
    }
  }
}

void game_logic__update_bullet_location(void) {
  for (size_t i = 0; i < MAX_NUM_OF_CANNON_BULLETS; i++) {
    if (cannon_bullets_array[i].is_valid) {
      game_graphics__display_laser_cannon_bullet(cannon_bullets_array[i].row_position,
                                                 cannon_bullets_array[i].column_position, BLACK);
      if (cannon_bullets_array[i].row_position != led_matrix_top_boundary) {
        cannon_bullets_array[i].row_position--;
        game_graphics__display_laser_cannon_bullet(cannon_bullets_array[i].row_position,
                                                   cannon_bullets_array[i].column_position,
                                                   cannon_bullets_array[i].color);
      } else {
        cannon_bullets_array[i].is_valid = false;
        game_graphics__display_laser_cannon_bullet(cannon_bullets_array[i].row_position,
                                                   cannon_bullets_array[i].column_position, BLACK);
      }
    }
  }
  game_logic__private_detect_bullet_collision_from_laser_cannon_to_enemy();
}

void game_logic__check_valid_enemy_to_shoot_bullet(void) {
  const TickType_t bullet_speed = 50;
  uint8_t enemy_row = (rand() % MAX_ROW_OF_ENEMIES);
  uint8_t enemy_column = (rand() % MAX_NUM_OF_ENEMIES);
  if (enemies_array[enemy_row][enemy_column].is_valid) {
    if (game_logic__private_detect_enemy_bullet_is_valid(&enemies_array[enemy_row][enemy_column], enemy_row)) {
      game_logic__private_enemy_shoot_bullet(&enemies_array[enemy_row][enemy_column]);
    }
  }
  game_logic__private_update_enemy_bullet_location();
  vTaskDelay(bullet_speed);
}

bool game_logic__get_game_won_status(void) { return is_game_won; }
void game_logic__set_game_won_status(bool status) { is_game_won = status; }
bool game_logic__get_game_over_status(void) { return is_game_over; }
void game_logic__set_game_over_status(bool status) { is_game_over = status; }
int game_logic__get_game_overall_score(void) { return overall_game_score; }
void game_logic__set_game_overall_score(int score) { overall_game_score = score; }
int game_logic__get_laser_cannon_lives(void) { return number_of_lives; }
