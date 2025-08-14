#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"

int main () {
  char str[20];
  i2c_init();
  i2c_start();
  
  while (1) {}
}

  