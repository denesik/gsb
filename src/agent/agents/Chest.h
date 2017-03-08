#pragma once
#include "../Agent.h"
#include "../../tools/Crc32.h"

class Chest : public NumeredAgent<Chest, gsb::crc32<std::string>()("Chest")>
{
public:
  Chest();
  Chest(const Chest & other);

  void JsonLoad(const rapidjson::Value &val) override;

private:
  int test = 0;
};

REGISTER_AGENT(Chest);