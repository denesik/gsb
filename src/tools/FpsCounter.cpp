// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "FpsCounter.h"
#include <chrono>

//#include <GLFW/glfw3.h>


FpsCounter::FpsCounter(void)
{
  currentTime = std::chrono::high_resolution_clock::now();
  lastTime = currentTime;
  fpsTime = 0;
}


FpsCounter::~FpsCounter(void)
{
}

void FpsCounter::Update()
{
  currentTime = std::chrono::high_resolution_clock::now();
  auto frameTime = std::chrono::duration<double, std::milli>(currentTime - lastTime).count() / 1000.0;

  fpsTime += frameTime;
  fpsStack.push_back(frameTime);

  while(fpsTime > 1)
  {
    fpsTime -= fpsStack.front();
    fpsStack.pop_front();
  }

  lastTime = currentTime;

  timer += frameTime;
  if (timer >= 0.1)
  {
    timer = 0;
    max = 0;
    min = 10000;
    mean = 0;
    for (const auto &time : fpsStack)
    {
      if (time > max) max = time;
      if (time < min) min = time;
      mean += time;
    }
    if (!fpsStack.empty())
      mean /= static_cast<double>(fpsStack.size());

    count_long_frame = 0;
    for (const auto &time : fpsStack)
    {
      if (time > (mean + max) / 2.0) ++count_long_frame;
    }
    if (!fpsStack.empty())
      percent_long_frame = static_cast<size_t>((static_cast<double>(count_long_frame) * 100.0) / static_cast<double>(fpsStack.size()));
  }
}

size_t FpsCounter::GetCount() const
{
  return fpsStack.size();
}

size_t FpsCounter::GetMaxFrameTime() const
{
  return static_cast<size_t>(max * 1000);
}

size_t FpsCounter::GetMinFrameTime() const
{
  return static_cast<size_t>(min * 1000);
}

size_t FpsCounter::GetMeanFrameTime() const
{
  return static_cast<size_t>(mean * 1000);
}

size_t FpsCounter::GetMaxFps() const
{
  return static_cast<size_t>(1.0 / min);
}

size_t FpsCounter::GetMinFps() const
{
  return static_cast<size_t>(1.0 / max);
}

size_t FpsCounter::GetMeanFps() const
{
  return static_cast<size_t>(1.0 / mean);
}

size_t FpsCounter::GetPercentLongFrame() const
{
  return percent_long_frame;
}

std::chrono::high_resolution_clock::duration FpsCounter::LastDuration() const
{
  return currentTime - lastTime;
}

float FpsCounter::LastDelta() const
{
  return static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(LastDuration()).count());
}
