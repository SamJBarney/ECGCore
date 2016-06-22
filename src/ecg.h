#pragma once

#include "global.h"
#include "config.h"

typedef bool(*continue_func_t)();

/***
* Creates the ticking threads
*/
void ecg_init(ecg_config_t*);

void ecg_reload();

void ecg_run(continue_func_t);

bool ecg_isRunning();

uint32_t ecg_threadCount();
