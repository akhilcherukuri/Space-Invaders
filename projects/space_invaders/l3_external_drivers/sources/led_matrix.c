/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Main Module Header */
#include "led_matrix.h"

/* Standard Includes */
#include <string.h>

/* External Includes */
#include "gpio.h"

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

static gpio_s R1, G1, B1, R2, G2, B2, A, B, C, D, E, CLK, OE, LAT;

/***********************************************************************************************************************
 *
 *                                         P R I V A T E   F U N C T I O N S
 *
 **********************************************************************************************************************/

static void led_matrix__private_enable_display(void) { gpio__set(OE); }

static void led_matrix__private_disable_display(void) { gpio__reset(OE); }

static void led_matrix__private_enable_latch(void) { gpio__set(LAT); }

static void led_matrix__private_disable_latch(void) { gpio__reset(LAT); }

static void led_matrix__private_select_row(uint8_t row) {
  (row & 0x01) ? gpio__set(A) : gpio__reset(A);
  (row & 0x02) ? gpio__set(B) : gpio__reset(B);
  (row & 0x04) ? gpio__set(C) : gpio__reset(C);
  (row & 0x08) ? gpio__set(D) : gpio__reset(D);
  (row & 0x10) ? gpio__set(E) : gpio__reset(E);
}
/***********************************************************************************************************************
 *
 *                                          P U B L I C   F U N C T I O N S
 *
 **********************************************************************************************************************/

void led_matrix__initialize() {
  R1 = gpio__construct_as_output(GPIO__PORT_2, 0);
  G1 = gpio__construct_as_output(GPIO__PORT_2, 1);
  B1 = gpio__construct_as_output(GPIO__PORT_2, 2);

  R2 = gpio__construct_as_output(GPIO__PORT_2, 4);
  G2 = gpio__construct_as_output(GPIO__PORT_2, 5);
  B2 = gpio__construct_as_output(GPIO__PORT_2, 6);

  A = gpio__construct_as_output(GPIO__PORT_2, 7);
  B = gpio__construct_as_output(GPIO__PORT_2, 8);
  C = gpio__construct_as_output(GPIO__PORT_2, 9);
  D = gpio__construct_as_output(GPIO__PORT_0, 16);
  E = gpio__construct_as_output(GPIO__PORT_0, 15);

  CLK = gpio__construct_as_output(GPIO__PORT_1, 20);
  OE = gpio__construct_as_output(GPIO__PORT_1, 23);
  LAT = gpio__construct_as_output(GPIO__PORT_1, 30);

  gpio__reset(R1);
  gpio__reset(G1);
  gpio__reset(B1);

  gpio__reset(R2);
  gpio__reset(G2);
  gpio__reset(B2);

  gpio__reset(A);
  gpio__reset(B);
  gpio__reset(C);
  gpio__reset(D);
  gpio__reset(E);

  gpio__reset(CLK);
  led_matrix__private_disable_display();
  led_matrix__private_disable_latch();

  memset(matrix_buffer, 0, MATRIX_HEIGHT * MATRIX_ROWS);
}

void led_matrix__display_pixels(void) {
  for (uint8_t row = 0; row < 32; row++) {
    led_matrix__private_disable_latch();
    led_matrix__private_disable_display();
    for (uint8_t column = 0; column < 64; column++) {
      (matrix_buffer[row][column] & 0x1) ? gpio__set(B1) : gpio__reset(B1);
      (matrix_buffer[row][column] & 0x2) ? gpio__set(G1) : gpio__reset(G1);
      (matrix_buffer[row][column] & 0x4) ? gpio__set(R1) : gpio__reset(R1);
      (matrix_buffer[row][column] & 0x8) ? gpio__set(B2) : gpio__reset(B2);
      (matrix_buffer[row][column] & 0x10) ? gpio__set(G2) : gpio__reset(G2);
      (matrix_buffer[row][column] & 0x20) ? gpio__set(R2) : gpio__reset(R2);

      gpio__set(CLK);
      gpio__reset(CLK);
    }
    led_matrix__private_enable_display();
    led_matrix__private_enable_latch();
    led_matrix__private_select_row(row);
  }
}

void led_matrix__clear_display(led_color_e color) {
  for (uint8_t row = 0; row < 32; row++) {
    for (uint8_t column = 0; column < 64; column++) {
      // matrix_buffer[row][column] = 0;
      led_matrix__clear_pixel(row, column, color);
    }
  }
}

void led_matrix__set_pixel(uint8_t row, uint8_t column, led_color_e color) {

  if ((row < 0) || (row > 63))
    return;
  if ((column < 0) || (column > 63))
    return;

  if (row > 31) {
    row = row - 32;
    color = color << 3;
    matrix_buffer[row][column] = (matrix_buffer[row][column] & 0x07) | color;
  } else {
    matrix_buffer[row][column] = (matrix_buffer[row][column] & 0x38) | color;
  }
}

void led_matrix__clear_pixel(uint8_t row, uint8_t column, led_color_e color) {

  if ((row < 0) || (row > 63))
    return;
  if ((column < 0) || (column > 63))
    return;

  if (row > 31) {
    row = row - 32;
    color = color << 3;
    matrix_buffer[row][column] &= ~((matrix_buffer[row][column] & 0x07) | color);
  } else {
    matrix_buffer[row][column] &= ~((matrix_buffer[row][column] & 0x38) | color);
  }
}
