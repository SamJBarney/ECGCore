#include "entity.h"
#include "config.h"

#include <mutex>
#include <vector>

static entity_t next_entity = 0;
static uint32_t TYPE_MASK = 0;
static uint32_t ENTITY_MASK = UINT32_MAX;

static std::vector<entity_t> claimed;
static std::vector<entity_t> unused;

static std::mutex ne_lock;
static std::mutex ce_lock;
static std::mutex ue_lock;

void entity_init()
{
  TYPE_MASK = config_current().entityTypeMask;
  ENTITY_MASK = UINT32_MAX ^ TYPE_MASK;
}




void entity_reload()
{
  
}




bool entity_retrieve_by_type(uint32_t a_Mask, entity_t* a_Entity)
{
  a_Mask = TYPE_MASK & a_Mask;
  size_t count = unused.size();
  for (size_t i = 0; i < count; ++i)
  {
    if (unused[i] & a_Mask)
    {
      *a_Entity = unused[i];
      unused[i] = unused[count - 1];
      unused.pop_back();
      return true;
    }
  }

  return false;
}




entity_t entity_new(uint32_t type)
{
  type = type & TYPE_MASK;
  entity_t entity;
  ue_lock.lock();
  if (unused.size() > 0 && entity_retrieve_by_type(type, &entity))
  {
    ue_lock.unlock();
  }
  else
  {
    ue_lock.unlock();
    ne_lock.lock();
    entity = next_entity;
    entity_t start = next_entity - 1;
    while(entity_exists(entity) && start != next_entity)
    {
      next_entity = (next_entity + 1) & ENTITY_MASK;
      entity = next_entity | type;
    }
    ne_lock.unlock();

    // Invalid entity
    if (start == next_entity)
    {
      entity = entity | ENTITY_MASK;
    }
  }

  // If it was a valid entity, add it to the claimed entities vector
  if ((entity & ENTITY_MASK) != ENTITY_MASK)
  {
    ce_lock.lock();
    claimed.push_back(entity);
    ce_lock.unlock();
  }

  return entity;
}

bool entity_valid(entity_t a_Entity)
{
  return (a_Entity & ENTITY_MASK) != ENTITY_MASK;
}




bool entity_exists(entity_t a_Entity)
{
  bool exists = false;

  ce_lock.lock();
  size_t count = claimed.size();
  for (size_t i = 0; i < count && !exists; ++i)
  {
    exists = claimed[i] == a_Entity;
  }
  ce_lock.unlock();

  return exists;
}




void entity_delete(entity_t a_Entity)
{
  ce_lock.lock();
  size_t count = claimed.size();
  for (size_t i = 0; i < count; ++ i)
  {
    if (claimed[i] == a_Entity)
    {
      claimed[i] = claimed[count - 1];
      claimed.pop_back();
      break;
    }
  }
  ce_lock.unlock();

  ue_lock.lock();
  unused.push_back(a_Entity);
  ue_lock.unlock();
}
