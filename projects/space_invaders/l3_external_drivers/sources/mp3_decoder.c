/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Main Module Header */
#include "mp3_decoder.h"

/* Standard Includes */

/* External Includes */
#include "delay.h"

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
static gpio_s dreq_pin, xdcs_pin, xcs_pin;

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
void mp3_decoder__init(gpio_s xcs, gpio_s xdcs, gpio_s dreq, gpio_s rst) {

  // set xcs xdcs reset output
  // MCU outputs reset as a low, set chip selects as high
  uint32_t set_12_mhz_clock = 12 * 1000 * 1000;
  // uint8_t enable_SM_EARSPEAKER_LO = 4;
  // uint8_t enable_SM_TESTS = 5;
  // uint8_t enable_SM_SDINEW = 11;
  uint16_t sci_mode_register_defaults = 0x4800;
  uint16_t default_sample_11hz = 0x2B10;
  uint16_t default_clockf_divide = 0xD800;
  dreq_pin = dreq;
  xdcs_pin = xdcs;
  xcs_pin = xcs;
  ssp1__initialize(set_12_mhz_clock);
  gpio__set(xcs);
  gpio__set(xdcs);
  gpio__reset(rst);
  delay__ms(2);
  gpio__set(rst);
  mp3_decoder__sci(write, SCI_MODE, sci_mode_register_defaults);
  mp3_decoder__sci(write, SCI_CLKF, default_clockf_divide);
  mp3_decoder__sci(write, SCI_AUDATA, default_sample_11hz);
  mp3_decoder__sci(write, SCI_VOLUME, 0x0101); // volume

  // setup i2s
  mp3_decoder__sci(write, SCI_WRAMADDR, 0xc017);
  mp3_decoder__sci(write, SCI_WRAM, 0xf0);
  mp3_decoder__sci(write, SCI_WRAMADDR, 0xc040);
  mp3_decoder__sci(write, SCI_WRAM, 0x0c);
}

void mp3_decoder__sci(sci_opcodes_e opcode, sci_registers_e register_address, uint16_t data) {
  uint8_t upper_8_bit_transfer = 8;
  uint8_t clear_upper_bits = 0xFF;
  while (!gpio__get(dreq_pin))
    ;
  gpio__reset(xcs_pin);
  ssp1__exchange_byte(opcode);
  ssp1__exchange_byte(register_address);
  ssp1__exchange_byte(data >> upper_8_bit_transfer);
  ssp1__exchange_byte(data & clear_upper_bits);
  gpio__set(xcs_pin);
}

void mp3_decoder__sdi(uint8_t data) {
  gpio__reset(xdcs_pin);
  ssp1__exchange_byte(data);
  gpio__set(xdcs_pin);
}

bool mp3_decoder__dreq(void) { return gpio__get(dreq_pin); }