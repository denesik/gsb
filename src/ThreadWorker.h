#pragma once
#ifndef ThreadWorker_h__
#define ThreadWorker_h__

#include <list>
#include <memory>
#include "DataBase.h"

// ��������� ���������� ������ �� ����������.
// ������ ������ ����� �� ����������.
// ������ ������ ����������� ����� ����� �������� ������.
// ����� ������ �������� ������ ��� ��������� �������, ��������� ����� � ������ � ����������.
// � ����� ���������� ������ �� ������. 
// ������ �������� ����� ������������ ������ �������.
// ����� ����� ������ ��������� �� ������.
// ����� ��� ��� ������ ������ ������, � ������ ��������� �����, � ������� ���������� ������ �� ������.
// ������ ������ ��������� ������ ��� ������.

// ������ ������ ����� ������:
// ��������� ������ ����� ������� ������.
// ������������ ��������� ���������.
// bool Task::Begin(Worker &);
// ��������� ������ � ��������. ��������� ������ � �������.
// void Task::End(const Worker &);

// ������ ������ ����� ��������� ������:
// ��������� ������ �������.
// void Worker::Run();
// �������� �� ������ ������?
// bool Worker::IsDone() const;

template<class Task, class Worker>
class ThreadWorker
{
public:
  template<class ...Args>
  ThreadWorker(Args &&...args)
  {
    mWorker = std::make_shared<Worker>(std::forward<Args>(args)...);
  }

  // �������� ������ �� ����������.
  void Add(Task &&task)
  {
    mData.push_back({std::forward<Task>(task), nullptr});
  }

  void Update()
  {
    // ��������� �� ���� �������.
    for (auto it = mData.begin(); it != mData.end();)
    {
      auto &data = *it;
      bool need_remove = false;

      // ���� ������ ������������, ������ ������ ��� �����������.
      // ��������� ��������� �� ������.
      // ���� ���������, ������� ������.
      if (data.worker)
      {
        if (data.worker->IsDone())
        {
          data.task.End(*data.worker);
          need_remove = true;
        }
      }
      else
      {
        // ���� ������ �����������, ���� ��������� ������ � ��������� ��� ������.
        // ���� ������ ��������� ������, ��������� ������.
        // ���� �� ������, ������� ������.
        if (mWorker.use_count() == 1)
        {
          data.worker = mWorker;
          if (data.task.Begin(*data.worker))
          {
            data.worker->Run();
          }
          else
          {
            need_remove = true;
          }
        }
      }

      if (need_remove)
      {
        it = mData.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

private:
  using WorkerType = std::shared_ptr<Worker>;

  struct Data
  {
    Task task;
    WorkerType worker;
  };

  std::list<Data> mData;

  WorkerType mWorker;
};


#endif // ThreadWorker_h__
