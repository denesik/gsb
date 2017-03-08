#include "Chest.h"
#include "../../Serialize.h"
#include <Magnum/Timeline.h>
#include "../../../imgui/imgui.h"
#include "../../BlockDynamicPart.h"
#include "../../BlocksDataBase.h"
#include "../../Item.h"

Chest::Chest()
{
}

Chest::Chest(const Chest& other) : test(other.test)
{
  AddItem(1, 1);
}

void Chest::JsonLoad(BlocksDataBase & db, const rapidjson::Value& val)
{
  JSONLOAD(NVP(test));
}

void Chest::DrawGui(const Magnum::Timeline &dt)
{
  ImGui::Text(std::to_string(test).c_str());
  if (ImGui::Button("inc"))
    test++;

  const auto &db = mParent->GetDataBase();
  for (auto &i : mItems)
  {
    static_cast<const Item &>(*(db.GetItem(std::get<0>(i)))).DrawGui(dt);
  }
}

void Chest::AddItem(ItemId id, size_t count)
{
  mItems.emplace_back(id, count);
}
