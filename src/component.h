#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

typedef void(*component_tick_t)(uint32_t);
typedef void(*component_gc_t)();
typedef void(*component_cleanup_t)();


typedef struct
{
  component_tick_t tick_func;
  component_gc_t gc_func;
  component_cleanup_t cleanup_func;
} component_t;

bool component_register(const char*, component_t);
bool component_deregister(const char*);
bool component_exists(const char*);
bool component_get(const char*, component_t*);
