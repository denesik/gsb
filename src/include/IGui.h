#pragma once
#include <tools/Common.h>
#include <functional>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <unordered_map>
#include <boost/optional.hpp>
#include <IDbHolder.h>
#include <memory>
#include <boost\noncopyable.hpp>

namespace Magnum {
  class Timeline;
}

using GuiFunction = std::function<void(const Magnum::Timeline &dt)>;

class GuiCtx;

class IContext : boost::noncopyable
{
public:
  IContext() = default;
  virtual ~IContext() = default;
  class NoContext;
};

class IContext::NoContext : public IContext
{
public:
};

class IGui
{
public:
  virtual void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) = 0;
  virtual std::unique_ptr<IContext> CreateContext() const = 0;
  virtual ~IGui() = default;
};

template <typename Context>
class ContextGui : public IGui
{
public:
  std::unique_ptr<IContext> CreateContext() const override
  {
    return std::make_unique<Context>();
  }
};

class NoContextGui : public ContextGui<IContext::NoContext>
{
public:
  virtual ~NoContextGui() = default;
};

class GuiCtx : public DBHolder, boost::noncopyable, public std::enable_shared_from_this<GuiCtx>
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

    GuiLinkage(std::shared_ptr<GuiCtx> ctx, IGui & gui, IContext & context);

  private:
    std::shared_ptr<GuiCtx> mCtx;
    IGui & mGui;
    IContext & mContext;
  };


  GuiCtx(DataBase & db);

  std::unique_ptr<GuiLinkage> Register(IGui & gui);
  size_t RegisteredCount();

private:
  void Unregister(GuiLinkage & linkage);

private:
  std::list<std::unique_ptr<IContext>> mStorage;
};

