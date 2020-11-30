#pragma once
/***********************************************************************************************************************
 *
 *                                                  I N C L U D E S
 *
 **********************************************************************************************************************/
/* Standard Includes */
#include <stdint.h>
#include <stdio.h>

/* External Includes */

/* Module Includes */

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
  GPIO_INTR__FALLING_EDGE,
  GPIO_INTR__RISING_EDGE,
} gpio_interrupt_e;

typedef void (*function_pointer_t)(void);
/***********************************************************************************************************************
 *
 *                                     F U N C T I O N   D E C L A R A T I O N S
 *
 **********************************************************************************************************************/

/* Allow the user to attach their callbacks */
void gpio_isr__attach_interrupt(uint8_t port, uint32_t pin, gpio_interrupt_e interrupt_type,
                                function_pointer_t callback);

/* main() should configure interrupts to invoke this dispatcher where we will invoke user attached callbacks */
void gpio_isr__interrupt_dispatcher(void);
