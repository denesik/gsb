// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#ifndef FPSCounter_h__
#define FPSCounter_h__

#include <chrono>
#include <list>

class FpsCounter
{
public:
  FpsCounter(void);
  ~FpsCounter(void);

  void Update();

  size_t GetCount() const;

  size_t GetMaxFrameTime() const;
  size_t GetMinFrameTime() const;
  size_t GetMeanFrameTime() const;

  size_t GetMaxFps() const;
  size_t GetMinFps() const;
  size_t GetMeanFps() const;

  size_t GetPercentLongFrame() const;

  std::chrono::high_resolution_clock::duration LastDuration() const;
  float LastDelta() const;

private:
  std::list<double> fpsStack;
  std::chrono::high_resolution_clock::time_point currentTime;
  std::chrono::high_resolution_clock::time_point lastTime;

  double fpsTime;

  double max = 0;
  double min = 0;
  double mean = 0;

  double timer = 0;

  size_t count_long_frame = 0;
  size_t percent_long_frame = 0;

};


#endif // FPSCounter_h__