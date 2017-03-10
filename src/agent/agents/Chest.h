#pragma once
#include "../Agent.h"
#include "../../tools/Crc32.h"
#include <vector>
#include <tuple>
#include "../../tools/Common.h"

class Chest : public NumeredAgent<Chest, gsb::crc32<std::string>()("Chest")>
{
public:
  Chest();
  Chest(const Chest & other);

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value &val) override;
  void DrawGui(const Magnum::Timeline &dt) override;

  void AddItem(ItemId id, size_t count);

  bool RemoveItem(ItemId id, size_t count);

  const std::vector<std::tuple<ItemId, size_t>> &Items() const;

  void Test();

private:
  int test = 0;

  std::vector<std::tuple<ItemId, size_t>> mItems;

};

REGISTER_AGENT_CLASS(Chest);