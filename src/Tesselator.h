#pragma once
#ifndef Tesselator_h__
#define Tesselator_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>
#include <boost/noncopyable.hpp>
#include "TemplateFactory.h"
#include <rapidjson/document.h>
#include "TextureAtlas.h"
#include <array>

typedef std::array<Magnum::Byte, 64> TesselatorData;

struct TesselatorVertex
{
  TesselatorVertex() = default;

  TesselatorVertex(const Magnum::Vector3 &_vertex, const Magnum::Vector2 &_textcoord)
    : vertex(_vertex), textcoord(_textcoord)
  {}

  Magnum::Vector3 vertex;
  Magnum::Vector2 textcoord;
};


class Tesselator
{
public:
  using factory = TemplateFactory<std::string, Tesselator, void()>;

  enum class TesselatorType
  {
    SOLID_BLOCK,
    MICRO_BLOCK,
    TEXTURED_BLOCK,
  };

  Tesselator(TesselatorType type);
  virtual ~Tesselator();

  TesselatorType Type() const;

  virtual void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas) = 0;
  virtual bool UseTesselatorData() const = 0;

private:
  const TesselatorType mType;
};

#endif // Tesselator_h__
