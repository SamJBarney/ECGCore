#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

typedef void(*component_init_t)();
typedef void(*component_tick_t)(uint32_t);
typedef void(*component_gc_t)();
typedef void(*component_cleanup_t)();


typedef struct
{
  component_init_t init_func;
  component_tick_t tick_func;
  component_gc_t gc_func;
  component_cleanup_t cleanup_func;
} component_t;

/***
* Returns a new component in its default state
*/
component_t component_new();

/***
* Adds a component to the component registry.
* Returns true if it was added.
* Returns false if it already exists.
*/
bool component_register(const char*, component_t);

/***
* Removes a component from the component registry
* Returns true if it was removed.
* Returns false if it didn't exist.
*/
bool component_deregister(const char*);

/***
* Checks if a component with the specified name exists in the registry
*/
bool component_exists(const char*);

/***
* Grab a component from the registry.
* Returns false if it wasn't able to grab the component.
*/
bool component_get(const char*, component_t*);

/***
* Validates whether the component
*/
bool component_valid(component_t);
