#include "Chest.h"
#include "../../Serialize.h"

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
