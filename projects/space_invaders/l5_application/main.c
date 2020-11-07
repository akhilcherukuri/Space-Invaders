#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "sj2_cli.h"


int main(void) {

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
