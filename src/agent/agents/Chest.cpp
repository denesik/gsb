#include "Chest.h"
#include "../../Serialize.h"
#include <Magnum/Timeline.h>
#include "../../../imgui/imgui.h"

Chest::Chest()
{
}

Chest::Chest(const Chest& other) : test(other.test)
{

}

void Chest::JsonLoad(const rapidjson::Value& val)
{
  JSONLOAD(NVP(test));
}

void Chest::DrawGui(Magnum::Timeline dt)
{
  ImGui::Text(std::to_string(test).c_str());
  if (ImGui::Button("inc"))
    test++;
}
