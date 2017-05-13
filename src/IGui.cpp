#include "IGui.h"

GuiCtx::GuiLinkage::GuiLinkage(GuiLinkage && other)
  : mCtx(other.mCtx)
  , mGui(other.mGui)
  , mContext(other.mContext)
{
}

GuiCtx::GuiLinkage::~GuiLinkage()
{
  mCtx->Unregister(*this);
}

IGui & GuiCtx::GuiLinkage::Gui()
{
  return mGui;
}

void GuiCtx::GuiLinkage::DrawGui(const Magnum::Timeline & dt)
{
  mGui.DrawGui(dt, *mCtx, mContext);
}

GuiCtx::GuiLinkage::GuiLinkage(std::shared_ptr<GuiCtx> ctx, IGui & gui, IContext & context)
  : mCtx(ctx)
  , mGui(gui)
  , mContext(context)
{
}

GuiCtx::GuiCtx(DataBase & db)
  : DBHolder(db)
{

}

GuiCtx::GuiLinkage GuiCtx::Register(IGui & gui)
{
  mStorage.push_front(std::move(std::make_unique<IContext>(gui.CreateContext())));
  auto &con = **mStorage.begin();
  return GuiCtx::GuiLinkage(shared_from_this(), gui, con);
}

size_t GuiCtx::RegisteredCount()
{
  return mStorage.size();
}

void GuiCtx::Unregister(GuiLinkage & linkage)
{
  mStorage.remove_if([&linkage](const decltype(*mStorage.begin()) & compare) {
    return compare.get() == &linkage.Context<IContext::NoContext>();
  });
}
