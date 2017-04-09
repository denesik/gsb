#pragma once
#include <tools/Common.h>
#include <functional>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <unordered_map>
#include <boost/optional.hpp>
#include "Log.h"

namespace Magnum {
  class Timeline;
}

using GuiFunction = std::function<void(const Magnum::Timeline &dt)>;

class GuiCtx
{
public:
  template<typename T>
  T * Get(intptr_t caller)
  {
    if (mStorage.find(caller) == mStorage.end())
      mStorage[caller] = T();

    auto & i = mStorage[caller];

    if (i.type() != typeid(T))
      LOG(error) << "ctx type mismatch; caller = 0x" << std::hex << caller << "; type = " << typeid(T).name();

    return boost::any_cast<T>(&i);
  }

  void Reset();

private:
  std::unordered_map<intptr_t, boost::any> mStorage;
};

class GSB_NOVTABLE IGui
{
public:
  virtual void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx) = 0;
  virtual ~IGui();

  boost::signals2::signal<void()> onGuiClose;
};
