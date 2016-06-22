#pragma once

#ifdef __cplusplus
  #include <cstdint>
  #define API_VISIBLE extern "C"
#else
  #include <stdint.h>
  #include <stdbool.h>
  #define API_VISIBLE
#endif
