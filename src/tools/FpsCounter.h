﻿// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#ifndef FPSCounter_h__
#define FPSCounter_h__

#include <chrono>
#include <list>

class FpsCounter
{
private:
  std::list<double> fpsStack;
  std::chrono::high_resolution_clock::time_point currentTime;
  std::chrono::high_resolution_clock::time_point lastTime;

  double fpsTime;

  double max = 0;
  double min = 0;
  double mean = 0;

  double timer = 0;

public:
  FpsCounter(void);
  ~FpsCounter(void);

  void Update();

  size_t GetCount();

  size_t GetMaxFrameTime();
  size_t GetMinFrameTime();
  size_t GetMeanFrameTime();

};


#endif // FPSCounter_h__