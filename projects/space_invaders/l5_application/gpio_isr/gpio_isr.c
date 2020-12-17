/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Main Module Header */
#include "gpio_isr.h"

/* Standard Includes */

/* External Includes */
#include "lpc40xx.h"

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
static function_pointer_t gpio_falling_edge_callbacks[2][32];
static function_pointer_t gpio_rising_edge_callbacks[2][32];

static void gpio_isr__private_clear_pin_interrupt(uint8_t port, uint8_t pin);
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
static void gpio_isr__private_clear_pin_interrupt(uint8_t port, uint8_t pin) {
  if (port == 0) {
    LPC_GPIOINT->IO0IntClr |= (1U << pin);
  } else if (port == 2) {
    LPC_GPIOINT->IO2IntClr |= (1U << pin);
  } else {
    // Interrupt not cleared. Invalid interrupt port
  }
}
/***********************************************************************************************************************
 *
 *                                          P U B L I C   F U N C T I O N S
 *
 **********************************************************************************************************************/

void gpio_isr__attach_interrupt(uint8_t port, uint32_t pin, gpio_interrupt_e interrupt_type,
                                function_pointer_t callback) {

  // check that the interrupt port is valid
  if (port != 0 && port != 2) {
    return;
  }

  switch (interrupt_type) {
  case GPIO_INTR__FALLING_EDGE:
    if (port == 0) {
      gpio_falling_edge_callbacks[0][pin] = callback;
      LPC_GPIOINT->IO0IntEnF |= (1U << pin);
    } else {
      gpio_falling_edge_callbacks[1][pin] = callback;
      LPC_GPIOINT->IO2IntEnF |= (1U << pin);
    }
    break;
  case GPIO_INTR__RISING_EDGE:
    if (port == 0) {
      gpio_rising_edge_callbacks[0][pin] = callback;
      LPC_GPIOINT->IO0IntEnR |= (1U << pin);
    } else {
      gpio_falling_edge_callbacks[1][pin] = callback;
      LPC_GPIOINT->IO2IntEnR |= (1U << pin);
    }
    break;
  default:
    break;
  }
}

void gpio_isr__interrupt_dispatcher(void) {
  uint8_t interrupt_pin = 0;

  /* Check if there is an interrupt on PORT 0 */
  if (LPC_GPIOINT->IntStatus & (1U << 0)) {
    while ((LPC_GPIOINT->IO0IntStatF >> interrupt_pin) && (interrupt_pin < 32)) {
      if (LPC_GPIOINT->IO0IntStatF >> interrupt_pin & 1) {
        if (gpio_falling_edge_callbacks[0][interrupt_pin]) {
          gpio_falling_edge_callbacks[0][interrupt_pin]();
        }
        gpio_isr__private_clear_pin_interrupt(0, interrupt_pin);
      }
      interrupt_pin++;
    }
  }
  if (LPC_GPIOINT->IntStatus & (1U << 0)) {
    interrupt_pin = 0;
    while ((LPC_GPIOINT->IO0IntStatR >> interrupt_pin) && (interrupt_pin < 32)) {
      if (LPC_GPIOINT->IO0IntStatR >> interrupt_pin & 1) {
        if (gpio_rising_edge_callbacks[0][interrupt_pin]) {
          gpio_rising_edge_callbacks[0][interrupt_pin]();
        }
        gpio_isr__private_clear_pin_interrupt(0, interrupt_pin);
      }
      interrupt_pin++;
    }
  }
  /* Check if there is an interrupt on PORT 2 */
  if (LPC_GPIOINT->IntStatus & (1U << 2)) {
    interrupt_pin = 0;
    while ((LPC_GPIOINT->IO2IntStatF >> interrupt_pin) && (interrupt_pin < 32)) {
      if (LPC_GPIOINT->IO2IntStatF >> interrupt_pin & 1) {
        if (gpio_falling_edge_callbacks[1][interrupt_pin]) {
          gpio_falling_edge_callbacks[1][interrupt_pin]();
        }
        gpio_isr__private_clear_pin_interrupt(1, interrupt_pin);
      }
      interrupt_pin++;
    }
  }
  if (LPC_GPIOINT->IntStatus & (1U << 2)) {
    interrupt_pin = 0;
    while ((LPC_GPIOINT->IO2IntStatR >> interrupt_pin) && (interrupt_pin < 32)) {
      if (LPC_GPIOINT->IO2IntStatR >> interrupt_pin & 1) {
        if (gpio_rising_edge_callbacks[1][interrupt_pin]) {
          gpio_rising_edge_callbacks[1][interrupt_pin]();
        }
        gpio_isr__private_clear_pin_interrupt(1, interrupt_pin);
      }
      interrupt_pin++;
    }
  }
}
