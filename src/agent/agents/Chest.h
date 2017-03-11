#pragma once
#include "../Agent.h"
#include "../../tools/Crc32.h"
#include <vector>
#include <tuple>
#include "../../tools/Common.h"
#include <boost/optional/optional.hpp>

class Chest : public NumeredAgent<Chest, gsb::crc32<std::string>()("Chest")>
{
public:
  using ItemList = std::vector<std::tuple<ItemId, size_t>>;

  Chest();
  Chest(const Chest & other);

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value &val) override;
  void DrawGui(const Magnum::Timeline &dt) override;

  // ���������� ������� ������ ���������
  size_t AddItem(ItemId id, size_t count);

  // ���������� ������� ������ �������
  size_t RemoveItem(ItemId id, size_t count);

  /// ���������� ������ � ��������� ��.
  size_t ItemCount(ItemId id) const;

  const ItemList &Items() const;

private:
  ItemList mItems;

  boost::optional<size_t> find_item(ItemId id) const;

};

REGISTER_AGENT_CLASS(Chest);