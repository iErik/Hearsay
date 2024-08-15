#include <stdlib.h>

#include "utils/mem.h"

void safeFree (void** ptr) {
  if (ptr == NULL || *ptr == NULL)
    return;

  free(*ptr);
  *ptr = NULL;
}
