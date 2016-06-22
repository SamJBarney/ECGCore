#include "config.h"

typedef struct {
  const char * dataPath;
  uint32_t minThreadCount;
  uint32_t entityTypeMask;
  uint32_t sleepTime;
} ecg_config_editable_t;

static ecg_config_t default_settings = { "data/", 2, 0, 50 };
static ecg_config_t current_settings = default_settings;

ecg_config_t config_default()
{
  return default_settings;
}




ecg_config_t config_current()
{
  return current_settings;
}




bool config_setCurrent(ecg_config_t* a_Config)
{
  bool exists = a_Config != nullptr;
  if (exists)
  {
    current_settings = *a_Config;
  }

  return exists;
}




bool config_setDataPath(void* a_Config, const char * a_Path)
{
  bool exists = a_Config != nullptr && a_Path != nullptr;
  if (exists)
  {
    ecg_config_editable_t* editable = static_cast<ecg_config_editable_t*>(a_Config);
    editable->dataPath = a_Path;
  }

  return exists;
}
