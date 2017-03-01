#include "Timer.h"



Timer::Timer()
{
  Start();
}


Timer::~Timer()
{
}

void Timer::Start()
{
  mStartTime = std::chrono::high_resolution_clock::now();
}

double Timer::Elapsed() const
{
  auto endTime = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>(endTime - mStartTime).count() / 1000.0;
}
