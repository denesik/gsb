#pragma once
#ifndef ThreadWorker_h__
#define ThreadWorker_h__

#include <list>
#include <memory>
#include "DataBase.h"

// Добавляем однотипные задачи на исполнение.
// Хранит список задач на выполнение.
// Задача начнет выполняться когда будет свободен воркер.
// Когда воркер закончет работу над указанной задачей, вызовется метод в задаче о завершении.
// В метод передается ссылка на воркер. 
// Внутри каллбека можно использовать методы воркера.
// После этого задача удаляется из списка.
// Перед тем как воркер начнет работу, у задачи вызовется метод, в который передается ссылка на воркер.
// Задача сможет настроить воркер для работы.

// Задача должна иметь методы:
// Настроить воркер перед началом работы.
// Возвращается результат настройки.
// bool Task::Begin(Worker &);
// Завершить работу с воркером. Запросить данные у воркера.
// void Task::End(const Worker &);

// Воркер должен иметь следующие методы:
// Запустить работу воркера.
// void Worker::Run();
// Завершил ли воркер работу?
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

  // Добавить задачу на исполнение.
  void Add(Task &&task)
  {
    mData.push_back({std::forward<Task>(task), nullptr});
  }

  void Update()
  {
    // Пробегаем по всем задачам.
    for (auto it = mData.begin(); it != mData.end();)
    {
      auto &data = *it;
      bool need_remove = false;

      // Если воркер присутствует, значит задача уже выполняется.
      // Проверяем завершена ли задача.
      // Если завершена, удаляем задачу.
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
        // Если воркер отсутствует, ищем свободный воркер и назначаем его задаче.
        // Если смогли настроить воркер, запускаем воркер.
        // Если не смогли, удаляем задачу.
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
