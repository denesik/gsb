#pragma once
#ifndef Timer_h__
#define Timer_h__

#include <chrono>

class Timer
{
public:
  Timer();
  ~Timer();

  void Start();

  float Elapsed() const;

private:
  std::chrono::high_resolution_clock::time_point mStartTime;
};



#endif // Timer_h__
