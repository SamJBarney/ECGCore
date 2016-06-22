#pragma once

#include "global.h"

typedef struct {
  const char * dataPath;
  uint32_t minThreadCount;
  uint32_t entityTypeMask;
  uint32_t sleepTime;
} ecg_config_t;

ecg_config_t config_default();

ecg_config_t config_current();

bool config_setCurrent(ecg_config_t*);

bool config_setDataPath(void*, const char *);
