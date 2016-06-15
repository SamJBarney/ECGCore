#pragma once

#ifdef __cplusplus
  #include <cstdint>
  #define API_VISIBLE extern "C"
#else
  #include <stdint.h>
  #include <stdbool.h>
  #define API_VISIBLE
#endif

/***
* Creates the ticking threads
*/
void ecg_init();

/***
* Preps the ticking threads for ticking
*/
void ecg_start();

/***
* Ticks the threads.
* Returns false if the tick threads are busy
*/
bool ecg_tick();

/***
* Checks whether there is a current tick going on
*/
bool ecg_ticking();

/***
* Stops the execution of the ticking threads
*/
void ecg_cleanup();


/***
* Returns the number of threads used for ticking
*/
uint32_t ecg_thread_count();
