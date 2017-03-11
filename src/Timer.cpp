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

float Timer::Elapsed() const
{
  auto endTime = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<float, std::milli>(endTime - mStartTime).count() / 1000.0f;
}


