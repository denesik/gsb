#pragma once
#ifndef Observer_h__
#define Observer_h__

#include <vector>
#include <functional>
#include <boost/optional.hpp>
#include <algorithm>

namespace ptl // pattern template library
{
#ifdef _DEBUG
#define USE_LOCK_CHECKER
#endif

  template<class T>
  class observer
  {
  public:
    using value_type = T;

    bool attach(T &data)
    {
#ifdef USE_LOCK_CHECKER
      lock lock_access(m_data_access);
#endif

      auto index = find(data);
      if (index)
      {
        return false;
      }
      m_data.push_back(data);

      return true;
    }

    bool detach(T &data)
    {
#ifdef USE_LOCK_CHECKER
      lock lock_access(m_data_access);
#endif

      auto index = find(data);
      if (index)
      {
        std::swap(m_data[*index], m_data.back());
        m_data.pop_back();

        return true;
      }

      return false;
    }

    // TODO: разрешить подписку/отписку во время выполнения функции.
    template<class ...Args, class FPTR = void (T::*)(Args...)>
    void notify(FPTR *fptr, Args &...args)
    {
#ifdef USE_LOCK_CHECKER
      lock lock_access(m_data_access);
#endif

      for (auto i : m_data)
      {
        (i.get().*fptr)(args...);
      }
    }

    template <typename T, class ...Args>
    void notify(void(T::*mf)(Args...), Args &&... args)
    {
      for (auto i : m_data)
      {
        (i.get().*mf)(args...);
      }
    }

  private:
    std::vector<std::reference_wrapper<T>> m_data;

#ifdef USE_LOCK_CHECKER
    bool m_data_access = false;
#endif

  private:
    boost::optional<size_t> find(T &data)
    {
      auto data_ptr = &data;
      auto it = std::find_if(m_data.begin(), m_data.end(), [data_ptr](const decltype(m_data)::value_type &val)
      {
        return &static_cast<T &>(val) == data_ptr;
      });
      if (it != m_data.end())
        return static_cast<size_t>(std::distance(m_data.begin(), it));

      return{};
    }

#ifdef USE_LOCK_CHECKER
    struct lock
    {
      lock(bool &val)
        : m_val(val)
      {
        if (m_val) assert("recursive lock");
        if (!m_val) m_val = true;
      }

      ~lock()
      {
        m_val = false;
      }

      bool &m_val;
    };
#endif

  };

#ifdef _DEBUG
#ifdef USE_LOCK_CHECKER
#undef USE_LOCK_CHECKER
#endif
#endif
}

#endif // Observer_h__
