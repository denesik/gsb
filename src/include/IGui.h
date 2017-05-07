#pragma once
#include <tools/Common.h>
#include <functional>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <unordered_map>
#include <boost/optional.hpp>
#include "Log.h"
#include <IDbHolder.h>

namespace Magnum {
  class Timeline;
}

using GuiFunction = std::function<void(const Magnum::Timeline &dt)>;

class GuiCtx;

class IContext
{
public:
  IContext() = default;
  virtual ~IContext() = default;
  class NoContext;
};

class IContext::NoContext : public IContext
{
public:
  IContext::NoContext() = default;
};

class GSB_NOVTABLE IGui
{
public:
  virtual void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) = 0;
  virtual IContext CreateContext() = 0;
  virtual ~IGui() = default;
};

template <typename Context>
class GSB_NOVTABLE IContextGui : public IGui
{
public:
  IContextGui(Context & context)
    : mContext(context)
  {
  }

  IContext CreateContext()
  {
    return Context();
  }

private:
  IContext &mContext;
};


class GSB_NOVTABLE INoContextGui : public IContextGui<IContext::NoContext>
{
public:
  INoContextGui() 
    : IContextGui<IContext::NoContext>(mFakeContext)
  {

  }
  virtual ~INoContextGui() = default;

  boost::signals2::signal<void()> onGuiClose;

private:
  IContext::NoContext mFakeContext;
};

class GuiCtx : public IDBHolder
{
public:

  class GuiLinkage
  {
  public:

    ~GuiLinkage();

    IContext & Context();
    IGui & Gui();

    void DrawGui(const Magnum::Timeline & dt);

  private:
    friend class GuiCtx;
    GuiLinkage(GuiCtx & ctx, IGui & gui, IContext & context);

  private:
    GuiCtx & mCtx;
    IGui & mGui;
    IContext & mContext;
  };

  GuiCtx(DataBase &db);

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

  GuiLinkage Register(IGui & gui)
  {
    mStorage.push_front(std::move(std::make_unique<IContext>(gui.CreateContext())));
    auto &con = **mStorage.begin();
    return GuiLinkage(*this, gui, con);
  }

private:
  void Unregister(GuiLinkage & linkage);

private:
  std::list<std::unique_ptr<IContext>> mStorage;
};

