#include "IGui.h"

GuiCtx::GuiCtx(DataBase & db)
  : IDBHolder(db)
{
}

void GuiCtx::Unregister(GuiLinkage & linkage)
{
  mStorage.remove_if([&linkage](const decltype(*mStorage.begin()) & compare) {
    return compare.get() == &linkage.Context();
  });
}

GuiCtx::GuiLinkage::GuiLinkage(GuiCtx & ctx, IGui & gui, IContext & context)
  : mCtx(ctx)
  , mGui(gui)
  , mContext(context)
{
}

void GuiCtx::GuiLinkage::DrawGui(const Magnum::Timeline & dt)
{
  mGui.DrawGui(dt, mCtx, mContext);
}

GuiCtx::GuiLinkage::~GuiLinkage()
{
  mCtx.Unregister(*this);
}

IContext & GuiCtx::GuiLinkage::Context()
{
  return mContext;
}

IGui & GuiCtx::GuiLinkage::Gui()
{
  return mGui;
}
