#pragma once
#ifndef ThreadProcess_h__
#define ThreadProcess_h__

#include <mutex>
#include <atomic>
#include <thread>
#include <functional>
#include <vector>

// Класс заглушка.
template<class Worker, class Task>
class ThreadProcess
{
public:
  using CallbackType = std::function<void(Worker &, Task &)>;

  // Количество потоков, количество воркеров.
  template<class ...Args>
  ThreadProcess(size_t thread_count, size_t worker_count, Args &&...args)
    : mWorker(std::forward<Args>(args)...)
  {
  }

  // Добавить задачу на выполнение.
  void Push(Task &task) 
  {
    mTasks.emplace_back(task);
  };

  void Update() 
  {
    if (!mTasks.empty())
    {
      for (auto &task : mTasks)
      {
        if (mBeginCallback)
          mBeginCallback(mWorker, task);
        mWorker.Process(task);
        if (mEndCallback)
          mEndCallback(mWorker, task);
      }
      mTasks.clear();
    }
  };

  void SetBeginCallback(CallbackType func) 
  {
    mBeginCallback = func;
  };

  void SetEndCallback(CallbackType func) 
  {
    mEndCallback = func;
  };

private:
  CallbackType mBeginCallback;
  CallbackType mEndCallback;

  Worker mWorker;
  std::vector<std::reference_wrapper<Task>> mTasks;
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
