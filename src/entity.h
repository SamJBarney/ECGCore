#pragma once

#ifdef __cplusplus
  #include <cstdint>
  #define API_VISIBLE extern "C"
#else
  #include <stdint.h>
  #include <stdbool.h>
  #define API_VISIBLE
#endif

typedef uint32_t entity_t;

void entity_init(uint32_t a_Mask);
API_VISIBLE entity_t entity_new(uint32_t);
API_VISIBLE bool entity_valid(entity_t);
API_VISIBLE bool entity_exists(entity_t);
API_VISIBLE void entity_delete(entity_t);
