#pragma once
#include "Accessor.h"
#include "../tools/Crc32.h"
#include <vector>
#include <tuple>
#include "../tools/Common.h"
#include <boost/optional/optional.hpp>

class AccessorItem : public NumeredAgent<AccessorItem, gsb::crc32<std::string>()("Chest")>
{
public:
  using ItemList = std::vector<std::tuple<ItemId, size_t>>;

  AccessorItem();
  AccessorItem(const AccessorItem & other);

  void JsonLoad(const DataBase & db, const rapidjson::Value &val) override;
  void DrawGui(const Magnum::Timeline &dt) override;

  // Возвращает сколько итемов добавлено
  size_t AddItem(ItemId id, size_t count);

  // Возвращает сколько итемов удалено
  size_t RemoveItem(ItemId id, size_t count);

  /// Количество итемов с указанным ид.
  size_t ItemCount(ItemId id) const;

  const ItemList &Items() const;

private:
  ItemList mItems;

  boost::optional<size_t> find_item(ItemId id) const;

};

REGISTER_AGENT_CLASS(AccessorItem);