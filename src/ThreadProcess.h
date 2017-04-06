#pragma once
#ifndef ThreadProcess_h__
#define ThreadProcess_h__

#include <mutex>
#include <atomic>
#include <thread>

// Класс заглушка.
template<class Worker, class Task>
class ThreadProcess
{
public:
  // Количество потоков, количество воркеров.
  ThreadProcess(size_t thread_count, size_t worker_count) {};
  // Добавить задачу на выполнение.
  void Push(Task &&task) {};

  void Update() {};
};



template<class Worker>
class ThreadProcess_old
{
public:
  ThreadProcess_old()
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

  ~ThreadProcess_old()
  {
    // Скорей всего этот метод нужно вызывать в деструкторах потомков, 
    // что бы поток завершился до того, как начнут уничтожаться данные потомка.
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
