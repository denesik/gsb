#pragma once
#include "Accessor.h"
#include "../tools/Crc32.h"
#include <vector>
#include <tuple>
#include "../tools/Common.h"
#include <boost/optional/optional.hpp>

class AccessorItem;
namespace
{
  static auto AccessorItemLoaded = Accessor::factory::Register<AccessorItem>::add("AccessorItem");
}

class AccessorItem : public NumeredAgent<AccessorItem, gsb::crc32<std::string>()("AccessorItem")>
{
public:
  using ParentType = NumeredAgent<AccessorItem, gsb::crc32<std::string>()("AccessorItem")>;

  AccessorItem() = delete;
  ~AccessorItem() override = default;

  AccessorItem(const AccessorItem &other);
  AccessorItem(AccessorItem &&other);
  /// Не используем операторы копирования и перемещения.
  const AccessorItem &operator=(const AccessorItem &other) = delete;
  AccessorItem &operator=(AccessorItem &&other) = delete;

  /// Конструкторы для клонирования.
  AccessorItem(const AccessorItem &other, Block &parent);
  AccessorItem(AccessorItem &&other, Block &parent);

  /// Создаем элемент через фабрику.
  AccessorItem(const DataBase &db, const rapidjson::Value &val, Block &parent);


  void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) override;

  // Возвращает сколько итемов добавлено
  size_t AddItem(ItemId id, size_t count);

  // Возвращает сколько итемов удалено
  size_t RemoveItem(ItemId id, size_t count);

  /// Количество итемов с указанным ид.
  size_t ItemCount(ItemId id) const;

  const ItemList &Items() const;

private:
  ItemList mItems;

private:
  boost::optional<size_t> find_item(ItemId id) const;
};
