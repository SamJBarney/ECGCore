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

/***
* Sets up the entity system
*/
void entity_init(uint32_t a_Mask);

/***
* Creates a new entity
*/
API_VISIBLE entity_t entity_new(uint32_t);

/***
* Checks whether an entity is valid
*/
API_VISIBLE bool entity_valid(entity_t);

/***
* Checks if the entity exists within the system
*/
API_VISIBLE bool entity_exists(entity_t);

/***
* Removes an entity from the system
*/
API_VISIBLE void entity_delete(entity_t);
