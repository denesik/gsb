#pragma once
#ifndef ThreadProcess_h__
#define ThreadProcess_h__

#include <mutex>
#include <atomic>
#include <thread>
#include <vector>


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



// �������� �������� ���������� ����������� �����.
// ������ ������ ����� ����� 3 �������: ��������, ������ � ���������� / �����������, ���������.
// TODO: �������� ����� ����� ����������� � ������ ������� ������������.
template<class Task>
class ThreadTask
{
public:
  template<class ...Args>
  ThreadTask(Args &&...args)
  {
    for (size_t i = 0; i < 10; ++i)
    {
      m_tasks.emplace_back(std::forward<Args>(args)...);
    }

    mThread = std::thread([this]()
    {
      while (!mClose)
      {
        for (auto &data : m_tasks)
        {
          if (data.status == READY)
          {
            data.task.Process();
            data.status = DONE;
          }
        }
        
        std::unique_lock<std::mutex> lock(mMutex);
        if (!mRun)
        {
          mCv.wait(lock);
        }
        mRun = false;
      }
    });
  }

  ~ThreadTask()
  {
    if (!mClose)
    {
      mClose = true;
      mCv.notify_one();
      mThread.join();
    }
  }

  void Run()
  {
    {
      std::unique_lock<std::mutex> lock(mMutex);
      mRun = true;
    }
    mCv.notify_one();
  }

  enum Status : int
  {
    FREE,
    READY,
    DONE,
  };

  struct TaskStatus
  {
    template<class ...Args>
    TaskStatus(Args &&...args)
      : task(std::forward<Args>(args)...)
    {}

    TaskStatus() = delete;
    TaskStatus(const TaskStatus &) = delete;
    TaskStatus(TaskStatus &&other) // ��-�� ����-������� � ������� ����� ���� �����������.
      : task(std::move(other.task))
    {}
    const TaskStatus &operator=(const TaskStatus &) = delete;
    TaskStatus &operator=(TaskStatus &&) = delete;
    ~TaskStatus() = default;

    Task task;
    std::atomic<int> status = FREE;
  };

  inline std::vector<TaskStatus> &tasks()
  {
    return m_tasks;
  }

private:
  std::vector<TaskStatus> m_tasks;

  std::thread mThread;
  std::atomic<bool> mClose = false;
  bool mRun = false;
  std::condition_variable mCv;
  std::mutex mMutex;
};





#endif // ThreadProcess_h__
