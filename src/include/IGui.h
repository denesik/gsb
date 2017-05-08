#pragma once
#include <tools/Common.h>
#include <functional>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <unordered_map>
#include <boost/optional.hpp>
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
};

class GSB_NOVTABLE IGui
{
public:
  virtual void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) = 0;
  virtual IContext CreateContext() = 0;
  virtual ~IGui() = default;
};

template <typename Context>
class GSB_NOVTABLE ContextGui : public IGui
{
public:
  IContext CreateContext()
  {
    return Context();
  }
};

class GSB_NOVTABLE NoContextGui : public ContextGui<IContext::NoContext>
{
public:
  virtual ~NoContextGui() = default;

  boost::signals2::signal<void()> onGuiClose;
};

class GuiCtx : public DBHolder, public boost::noncopyable
{
public:

  class GuiLinkage : public boost::noncopyable
  {
  public:

    GuiLinkage(GuiLinkage && other);
    ~GuiLinkage();

    template<typename Type>
    Type & Context()
    {
      return static_cast<Type&>(mContext);
    }

    template<>
    IContext & Context()
    {
      return mContext;
    }

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


  GuiCtx(DataBase & db);

  GuiLinkage Register(IGui & gui);
  size_t RegisteredCount();

private:
  void Unregister(GuiLinkage & linkage);

private:
  std::list<std::unique_ptr<IContext>> mStorage;
};

