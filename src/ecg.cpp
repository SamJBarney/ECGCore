#include "ecg.h"
#include "component.h"

#include <atomic>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>

#ifndef MIN_TICK_THREAD_COUNT
#define MIN_TICK_THREAD_COUNT 4
#endif

#ifndef MIN_GC_THREAD_COUNT
#define MIN_GC_THREAD_COUNT 2
#endif

void tick_thread_func(uint32_t id);
void gc_thread_func();


std::vector<std::thread> tick_threads;
static uint32_t thread_count = 0;
std::thread gc_thread;

std::atomic_bool running;
std::mutex tick_mutex;
std::mutex completed_mutex;
std::condition_variable cv;
static std::atomic<uint32_t> completed_threads_flag(UINT32_MAX);

extern std::vector<component_t> registered_components;




void create_threads()
{
  // Lookup the amount of threads we can use, minimum of MIN_TICK_THREAD_COUNT
  thread_count = std::thread::hardware_concurrency();
  if (thread_count < MIN_TICK_THREAD_COUNT)
  {
    thread_count = MIN_TICK_THREAD_COUNT;
  }

  // Create the tick threads
  for(unsigned i = 0; i < thread_count; ++i)
  {
    tick_threads.push_back(std::thread(tick_thread_func, i+1));
  }
}




void ecg_init()
{
  // Create the tick threads
  create_threads();

  // Set the flag to the finished state
  completed_threads_flag = UINT32_MAX;

  // Create the garbage collection thread
  gc_thread = std::thread(gc_thread_func);
}




void ecg_start()
{
  running = true;
}




bool ecg_tick()
{
  if (!ecg_ticking())
  {
    completed_mutex.lock();
    completed_threads_flag.store(0, std::memory_order_seq_cst);
    completed_mutex.unlock();
    cv.notify_all();
    return true;
  }
  // Handle either the gc not being done, or the ticking not being done
  return false;
}




bool ecg_ticking()
{
  completed_mutex.lock();
  auto value = completed_threads_flag.load(std::memory_order_seq_cst);
  bool ticking = value != UINT32_MAX;
  completed_mutex.unlock();
  return ticking;
}




void ecg_cleanup()
{
  // Wait for any tick threads to finish
  while(ecg_ticking());
  // Notify the threads that cleanup is happening
  running = false;
  ecg_tick();

  // Join the threads
  for(auto& thread : tick_threads)
  {
    if (thread.joinable())
      thread.join();
  }
  if (gc_thread.joinable())
    gc_thread.join();
}




uint32_t ecg_thread_count()
{
  return thread_count;
}




#include <iostream>
void tick_thread_func(uint32_t id)
{
  // Hold until the engine is ready to run
  while(!running);

  // While the engine is running
  while(running)
  {
    std::unique_lock<std::mutex> lck (tick_mutex);

    // Wait until it is time to tick
    cv.wait(lck, []{
      completed_mutex.lock();
      // Need to figure out a better way to handle waiting to tick
      bool ready = completed_threads_flag.load(std::memory_order_seq_cst) < thread_count;
      completed_mutex.unlock();
      return ready;
    });

    // If the engine hasn't been stopped
    if (running)
    {
      // Iterate through all components and tick them
      for (auto& component : registered_components)
      {
        if (component.tick_func != NULL)
        {
          component.tick_func(id);
        }
      }
    }

    // Update the flag to show this thread has completed
    completed_mutex.lock();
    completed_threads_flag.fetch_add(1, std::memory_order_seq_cst);
    completed_mutex.unlock();

    // Notify any waiting threads that a thread has finished
    cv.notify_all();
  }
}




void gc_thread_func()
{
  while(running)
  {
    // Wait until all tick threads have completed before doing garbase collection
    std::unique_lock<std::mutex> lck (tick_mutex);
    cv.wait(lck, []{
      completed_mutex.lock();
      bool ready = completed_threads_flag.load(std::memory_order_seq_cst) == thread_count;
      completed_mutex.unlock();
      return ready;
    });

    // Don't gc if we are stopping the engine
    if (running)
    {
      for (auto& component : registered_components)
      {
        component.gc_func();
      }
    }

    // Update the flag to COMPLETED
    completed_mutex.lock();
    completed_threads_flag.store(UINT32_MAX, std::memory_order_seq_cst);
    completed_mutex.unlock();

    // Notify waiting threads
    // Is this even necessary? What threads are waiting?
    cv.notify_all();
  }
}
