#pragma once

#include "global.h"
#include "config.h"

typedef uint32_t entity_t;

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
