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

  TesselatorVertex(const Magnum::Vector3 &_vertex, const Magnum::Vector2 &_textcoord, const Magnum::Vector3 _normal)
    : vertex(_vertex), textcoord(_textcoord), normal(_normal)
  {}

  Magnum::Vector3 vertex;
  Magnum::Vector2 textcoord;
  Magnum::Vector3 normal;
};


class Tesselator
{
public:
  using factory = TemplateFactory<std::string, Tesselator, void()>;

  Tesselator() = delete;
  virtual ~Tesselator() = default;

  Tesselator(const Tesselator &other) = delete;
  Tesselator(Tesselator &&other) = delete;

  Tesselator &operator=(const Tesselator &other) = delete;
  Tesselator &operator=(Tesselator &&other) = delete;

  uint32_t id() const { return m_id; }
  const char *name() const { return m_name; }

  virtual void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas) = 0;
  virtual bool UseTesselatorData() const = 0;

protected:
  template<class T>
  struct identity
  {
    using type = T;
  };

  template<class T>
  Tesselator(identity<T>)
    : m_name(T::name()), m_id(T::id())
  {}

private:
  const uint32_t m_id;
  const char *m_name;
};

#endif // Tesselator_h__
