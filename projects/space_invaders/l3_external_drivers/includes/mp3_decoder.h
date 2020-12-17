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
#include "gpio.h"
#include "ssp1.h"

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
typedef enum {
  write = 0x02,
  read = 0x03,
} sci_opcodes_e;

typedef enum {
  SCI_MODE = 0x00,
  SCI_STATUS = 0x01,
  SCI_BASS = 0x02,
  SCI_CLKF = 0x03,
  SCI_DECODETIME = 0x04,
  SCI_AUDATA = 0x05,
  SCI_WRAM = 0x06,
  SCI_WRAMADDR = 0x07,
  SCI_HDAT0 = 0x08,
  SCI_HDAT1 = 0x09,
  SCI_VOLUME = 0x0B,
} sci_registers_e;

/***********************************************************************************************************************
 *
 *                                     F U N C T I O N   D E C L A R A T I O N S
 *
 **********************************************************************************************************************/

void mp3_decoder__init(gpio_s xcs, gpio_s xdcs, gpio_s dreq, gpio_s rst);
void mp3_decoder__sci(sci_opcodes_e opcode, sci_registers_e register_address, uint16_t data);
void mp3_decoder__sci_read(sci_opcodes_e opcode, sci_registers_e register_address);
void mp3_decoder__sdi(uint8_t data);
bool mp3_decoder__dreq(void);