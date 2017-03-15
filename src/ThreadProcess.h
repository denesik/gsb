#pragma once
#ifndef ThreadProcess_h__
#define ThreadProcess_h__

#include <mutex>
#include <atomic>
#include <thread>


template<class Worker>
class ThreadProcess
{
public:
  ThreadProcess()
  {
    mThread = std::thread([this]()
    {
      while (!mClose)
      {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mRunned)
        {
          static_cast<Worker &>(*this).Process();
          mRunned = false;
        }
        mCv.wait(lock);
      }
    });
  }

  ~ThreadProcess()
  {
    // ������ ����� ���� ����� ����� �������� � ������������ ��������, 
    // ��� �� ����� ���������� �� ����, ��� ������ ������������ ������ �������.
    Release();
  }

  void Run()
  {
    mRunned = true;
    mCv.notify_one();
  }

  bool IsDone() const
  {
    return mRunned == false;
  }

  void Release()
  {
    if (!mClose)
    {
      mClose = true;
      mCv.notify_one();
      mThread.join();
    }
  }

private:
  std::thread mThread;
  std::atomic<bool> mRunned = false;
  std::atomic<bool> mClose = false;
  std::condition_variable mCv;
  std::mutex mMutex;
};

#endif // ThreadProcess_h__
