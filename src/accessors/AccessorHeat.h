#pragma once
#include "Accessor.h"
#include "../tools/Crc32.h"
#include <vector>
#include <tuple>
#include "../tools/Common.h"
#include <boost/optional/optional.hpp>

class AccessorHeat;
namespace
{
  static auto AccessorHeatLoaded = Accessor::factory::Register<AccessorHeat>::add("AccessorHeat");
}

class AccessorHeat : public NumeredAgent<AccessorHeat, "AccessorHeat"_crc32>
{
public:
  using ParentType = NumeredAgent<AccessorHeat, "AccessorHeat"_crc32>;

  AccessorHeat() = delete;
  ~AccessorHeat() override = default;

  AccessorHeat(const AccessorHeat &other);
  AccessorHeat(AccessorHeat &&other);
  /// Не используем операторы копирования и перемещения.
  const AccessorHeat &operator=(const AccessorHeat &other) = delete;
  AccessorHeat &operator=(AccessorHeat &&other) = delete;

  /// Конструкторы для клонирования.
  AccessorHeat(const AccessorHeat &other, Block &parent);
  AccessorHeat(AccessorHeat &&other, Block &parent);

  /// Создаем элемент через фабрику.
  AccessorHeat(const DataBase &db, const rapidjson::Value &val, Block &parent);


  void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) override;

private:
  float mCollected;
  float mMax;

private:
  boost::optional<size_t> find_item(ItemId id) const;
};
