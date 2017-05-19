#pragma once
#include <tuple>
#include <vector>

using ItemId = uint32_t;
using ItemList = std::vector<std::tuple<ItemId, size_t>>;

class IItemListTransform
{
public:
  virtual ItemList & operator ()(ItemList &list) = 0;
};

class SortItemList : public IItemListTransform
{
public:
  class Params
  {
  public:
    Params &SetDecreaseSort();
    Params &SetIncreaseSort();

    bool IsIncreasing() const;

  private:
    bool mIncrease = true;
  };

  SortItemList(const Params & params);

  // Унаследовано через IItemListFunction
  ItemList & operator()(ItemList & list) override;

private:
  const Params &mParams;
};