#include "ItemTools.h"

SortItemList::SortItemList(const Params & params)
  : mParams(params)
{
}

ItemList & SortItemList::operator()(ItemList & list)
{
  using VT = ItemList::value_type;
  if (mParams.IsIncreasing())
  {
    std::qsort(&list[0], list.size(), sizeof(VT), [](const void *r, const void *l) -> int {
      const auto &arg1 = std::get<0>(*static_cast<const VT*>(r));
      const auto &arg2 = std::get<0>(*static_cast<const VT*>(l));

      return (arg1 > arg2) - (arg1 < arg2);
    });
  }
  else
  {
    std::qsort(&list[0], list.size(), sizeof(VT), [](const void *r, const void *l) -> int {
      const auto &arg1 = std::get<0>(*static_cast<const VT*>(r));
      const auto &arg2 = std::get<0>(*static_cast<const VT*>(l));

      return (arg1 < arg2) - (arg1 > arg2);
    });
  }
  return list;
}

SortItemList::Params & SortItemList::Params::SetDecreaseSort()
{
  mIncrease = false;
  return *this;
}

SortItemList::Params & SortItemList::Params::SetIncreaseSort()
{
  mIncrease = true;
  return *this;
}

bool SortItemList::Params::IsIncreasing() const
{
  return mIncrease;
}
