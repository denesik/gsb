#pragma once
#include <tools\Common.h>
#include <tuple>
#include <vector>

class DragNDrop
{
public:
  static void Drag(std::tuple<ItemId, size_t> item);
  static void Drag(std::vector<std::tuple<ItemId, size_t>> items);

  static std::vector<std::tuple<ItemId, size_t>> Drop();

  static bool IsBusy();

private:
  static std::vector<std::tuple<ItemId, size_t>> mContent;
};

