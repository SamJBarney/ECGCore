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
uint32_t thread_count = 0;
std::thread gc_thread;

std::atomic_bool running;
std::mutex tick_mutex;
std::mutex completed_mutex;
std::condition_variable cv;
static std::atomic<uint32_t> completed_threads(UINT32_MAX);

extern std::vector<component_t> registered_components;




void create_threads()
{
  thread_count = std::thread::hardware_concurrency();
  if (thread_count < MIN_TICK_THREAD_COUNT)
  {
    thread_count = MIN_TICK_THREAD_COUNT;
  }

  for(unsigned i = 0; i < thread_count; ++i)
  {
    tick_threads.push_back(std::thread(tick_thread_func, i+1));
  }
}




void ecg_init()
{
  create_threads();
  completed_threads = UINT32_MAX;
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
    completed_threads.store(0, std::memory_order_seq_cst);
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
  auto value = completed_threads.load(std::memory_order_seq_cst);
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



#include <iostream>
void tick_thread_func(uint32_t id)
{
  while(!running);
  while(running)
  {
    std::unique_lock<std::mutex> lck (tick_mutex);
    cv.wait(lck, []{
      completed_mutex.lock();
      bool ready = completed_threads.load(std::memory_order_seq_cst) < thread_count;
      completed_mutex.unlock();
      return ready;
    });

    if (running)
    {
      for (auto& component : registered_components)
      {
        if (component.tick_func != NULL)
        {
          component.tick_func(id);
        }
      }
    }
    completed_mutex.lock();
    auto current = completed_threads.load(std::memory_order_seq_cst);
    // std::cerr << "Thread #" << id << " with completed: " << current << std::endl;
    completed_threads.fetch_add(1, std::memory_order_seq_cst);
    completed_mutex.unlock();
    cv.notify_all();
  }
}




void gc_thread_func()
{
  while(running)
  {
    std::unique_lock<std::mutex> lck (tick_mutex);
    cv.wait(lck, []{
      completed_mutex.lock();
      bool ready = completed_threads.load(std::memory_order_seq_cst) == thread_count;
      completed_mutex.unlock();
      return ready;
    });

    if (running)
    {
      for (auto& component : registered_components)
      {
        component.gc_func();
      }
    }
    completed_mutex.lock();
    completed_threads.store(UINT32_MAX, std::memory_order_seq_cst);
    completed_mutex.unlock();
    cv.notify_all();
  }
}
