#include "DragNDrop.h"

void DragNDrop::Drag(std::tuple<ItemId, size_t> item)
{
  Drag({item});
}

void DragNDrop::Drag(std::vector<std::tuple<ItemId, size_t>> items)
{
  mContent = std::move(items);
}

std::vector<std::tuple<ItemId, size_t>> DragNDrop::Drop()
{
  return std::move(mContent);
}

bool DragNDrop::IsBusy()
{
  return !mContent.empty();
}

std::vector<std::tuple<ItemId, size_t>> DragNDrop::mContent;
