// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "FpsCounter.h"

//#include <GLFW/glfw3.h>


FpsCounter::FpsCounter(void)
{
  //currentTime = glfwGetTime();
  lastTime = currentTime;
  fpsTime = 0;
}


FpsCounter::~FpsCounter(void)
{
}

void FpsCounter::Update()
{
  //currentTime = glfwGetTime();
  double frameTime = currentTime - lastTime;

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
    min = 1000;
    for (const auto &time : fpsStack)
    {
      if (time > max) max = time;
      if (time < min) min = time;
      mean += time;
    }
    mean /= static_cast<double>(fpsStack.size());
  }
}

size_t FpsCounter::GetCount()
{
  return fpsStack.size();
}

size_t FpsCounter::GetMaxFrameTime()
{
  return static_cast<size_t>(max * 1000);
}

size_t FpsCounter::GetMinFrameTime()
{
  return static_cast<size_t>(min * 1000);
}

size_t FpsCounter::GetMeanFrameTime()
{
  return static_cast<size_t>(mean * 1000);
}
