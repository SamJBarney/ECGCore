#pragma once

/***
* Creates the ticking threads
*/
void ecg_init();

/***
* Preps the ticking threads for ticking
*/
void ecg_start();

/***
* Ticks the threads
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
