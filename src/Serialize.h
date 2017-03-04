#pragma once
#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <string>
#include <sstream>
#include <functional>
#include <rapidjson/document.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Math.h>
#include <boost/format.hpp>
#include <vector>
#include <memory>
#include <Magnum/SceneGraph/AbstractFeature.h>
#include <Magnum/Math/Vector4.h>

#define NVP(T) sge::make_nvp(#T, T)
#define JSONLOAD(...) DeserializeHelper::deserialize(val, __VA_ARGS__)
#define BINSAVE(...) BinSaveHelper::bserialize(val, __VA_ARGS__)
#define BINLOAD(...) BinLoadHelper::bdeserialize(val, __VA_ARGS__)

namespace sge {
  template <class T>
  std::pair<const char *, T> make_nvp(const char *name, T &&value) {
    return{ name, std::forward<T>(value) };
  }
}

class DeserializeHelper
{
public:
  static void deserialize(const rapidjson::Value &val)
  {
    (void)val;
  }

  template <typename Last>
  static void deserialize(const rapidjson::Value &val, const Last &last)
  {
    __deserialize(val, last.first, last.second);
  }

  template <typename First, typename... Rest>
  static void deserialize(const rapidjson::Value &val, const First &first, const Rest&... rest)
  {
    __deserialize(val, first.first, first.second);
    deserialize(val, rest...);
  }
};

namespace 
{
  template<typename _Ty>
  void _deserialize_array_part(const rapidjson::Value &val, _Ty &target)
  {
    target.JsonLoad(val);
  }

  template<typename _Ty>
  void _deserialize_array_part(const rapidjson::Value &arr, std::vector<_Ty> &target)
  {
    if (arr.IsArray())
    {
      for (decltype(arr.Size()) i = 0; i < arr.Size(); i++)
      {
        _Ty part;
        _deserialize_array_part(arr[i], part);
        target.push_back(std::move(part));
      }
    }
  }

  template<typename _Ty>
  void _deserialize(const rapidjson::Value &val, const char *s, _Ty &target)
  {
    if (!val.HasMember(s))
      return;
    const rapidjson::Value &v = val[s];
    target.JsonLoad(v);
  }

  template<typename _Ty>
  void _deserialize(const rapidjson::Value &val, const char *s, std::shared_ptr<_Ty> &target)
  {
    if (!val.HasMember(s))
      return;
    const rapidjson::Value &v = val[s];
    target->JsonLoad(v);
  }

  template<typename _Ty>
  void _deserialize(const rapidjson::Value &val, const char *s, std::vector<_Ty> &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      for (decltype(arr.Size()) i = 0; i < arr.Size(); i++)
      {
        _Ty part;
        _deserialize_array_part(arr[i], part);
        target.push_back(std::move(part));
      }
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, int &target)
  {
    if (val.HasMember(s))
    {
      if (!val[s].IsInt())
        throw std::invalid_argument((boost::format("value %1% is not a integer") % s).str());
      target = val[s].GetInt();
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, std::string &target)
  {
    if (val.HasMember(s))
    {
      if (!val[s].IsString())
        throw std::invalid_argument((boost::format("value %1% is not a string") % s).str());
      target = val[s].GetString();
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, float &target)
  {
    if (val.HasMember(s))
    {
      if (!val[s].IsNumber())
        throw std::invalid_argument((boost::format("value %1% is not a number") % s).str());
      target = static_cast<float>(val[s].GetDouble());
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, bool &target)
  {
    if (val.HasMember(s))
    {
      if (!val[s].IsBool())
        throw std::invalid_argument((boost::format("value %1% is not a bool") % s).str());
      target = val[s].GetBool();
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, Magnum::Vector2 &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      if (arr.Size() != 2)          throw std::invalid_argument((boost::format("value %1% is not vec2") % s).str());
      if (!arr.Begin()->IsNumber()) throw std::invalid_argument((boost::format("value %1%[0] is not a number") % s).str());
      if (!arr[1].IsNumber())       throw std::invalid_argument((boost::format("value %1%[1] is not a number") % s).str());
      target.x() = static_cast<float>(arr.Begin()->GetDouble());
      target.y() = static_cast<float>(arr[1].GetDouble());
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, Magnum::Vector3 &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      if (arr.Size() != 3)          throw std::invalid_argument("value is not vec3");
      if (!arr.Begin()->IsNumber()) throw std::invalid_argument((boost::format("value %1%[0] is not a number") % s).str());
      if (!arr[1].IsNumber())       throw std::invalid_argument((boost::format("value %1%[1] is not a number") % s).str());
      if (!arr[2].IsNumber())       throw std::invalid_argument((boost::format("value %1%[2] is not a number") % s).str());
      target.x() = static_cast<float>(arr.Begin()->GetDouble());
      target.y() = static_cast<float>(arr[1].GetDouble());
      target.z() = static_cast<float>(arr[2].GetDouble());
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, Magnum::Vector4 &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      if (arr.Size() != 4)          throw std::invalid_argument("target variable is not vec4");
      if (!arr.Begin()->IsNumber()) throw std::invalid_argument((boost::format("value %1%[0] is not a number") % s).str());
      if (!arr[1].IsNumber())       throw std::invalid_argument((boost::format("value %1%[1] is not a number") % s).str());
      if (!arr[2].IsNumber())       throw std::invalid_argument((boost::format("value %1%[2] is not a number") % s).str());
      if (!arr[3].IsNumber())       throw std::invalid_argument((boost::format("value %1%[3] is not a number") % s).str());
      target.x() = static_cast<float>(arr.Begin()->GetDouble());
      target.y() = static_cast<float>(arr[1].GetDouble());
      target.z() = static_cast<float>(arr[2].GetDouble());
      target.w() = static_cast<float>(arr[3].GetDouble());
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, std::vector<int> &target)
  {
    if (val.HasMember(s))
    {
      const rapidjson::Value &arr = val[s];
      for (decltype(arr.Size()) i = 0; i < arr.Size(); i++)
      {
        if (!arr[i].IsInt())
          throw std::invalid_argument((boost::format("value %1%[%2%] is not a integer") % s % i).str());
        target.push_back(arr[i].GetInt());
      }
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, std::vector<std::string> &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      for (decltype(arr.Size()) i = 0; i < arr.Size(); i++)
      {
        if (!arr[i].IsString())
          throw std::invalid_argument((boost::format("value %1%[%2%] is not a string") % s % i).str());
        target.push_back(arr[i].GetString());
      }
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, std::vector<bool> &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      for (decltype(arr.Size()) i = 0; i < arr.Size(); i++)
      {
        if (!arr[i].IsBool())
          throw std::invalid_argument((boost::format("value %1%[%2%] is not a bool") % s % i).str());
        target.push_back(arr[i].GetBool());
      }
    }
  }

  template<> void _deserialize(const rapidjson::Value &val, const char *s, std::vector<float> &target)
  {
    if (val.HasMember(s) && val[s].IsArray())
    {
      const rapidjson::Value &arr = val[s];
      for (decltype(arr.Size()) i = 0; i < arr.Size(); i++)
      {
        if (!arr[i].IsDouble())
          throw std::invalid_argument((boost::format("value %1%[%2%] is not a number") % s % i).str());
        target.push_back(static_cast<float>(arr[i].GetDouble()));
      }
    }
  }
}

class BinSaveHelper
{
public:
  static void bserialize(std::ostream &val)
  {
    (void)val;
  }

  template <typename Last> 
  static void bserialize(std::ostream &val, const Last &last)
  {
    _bserialize(val, last);
  }

  template <typename First, typename... Rest> 
  static void bserialize(std::ostream &val, const First &first, const Rest&... rest)
  {
    _bserialize(val, first);
    bserialize(val, rest...);
  }
};

namespace 
{

  template<typename _Ty> 
  void _bserialize(std::ostream &val, const _Ty &target)
  {
    target.BinSave(val);
  }

  template<typename _Ty> 
  void _bserialize(std::ostream &val, const std::shared_ptr<_Ty> &target)
  {
    _bserialize(val, !!target);
    if (target)
      target->BinSave(val);
  }

  template<typename _Ty> 
  void _bserialize(std::ostream &val, const std::vector<_Ty> &target)
  {
    _bserialize(val, target.size());
    for (decltype(target.size()) i = 0; i < target.size(); ++i)
    {
      _bserialize(val, target[i]);
    }
  }

  template<> void _bserialize(std::ostream &val, const int &target)
  {
    val.write(reinterpret_cast<const char *>(&target), sizeof(target));
  }

  template<> void _bserialize(std::ostream &val, const unsigned short &target)
  {
    val.write(reinterpret_cast<const char *>(&target), sizeof(target));
  }

  template<> void _bserialize(std::ostream &val, const bool &target)
  {
    val.write(reinterpret_cast<const char *>(&target), sizeof(target));
  }

  template<> void _bserialize(std::ostream &val, const unsigned int &target)
  {
    val.write(reinterpret_cast<const char *>(&target), sizeof(target));
  }

  template<> void _bserialize(std::ostream &val, const float &target)
  {
    val.write(reinterpret_cast<const char *>(&target), sizeof(target));
  }

  template<> void _bserialize(std::ostream &val, const std::string &target)
  {
    _bserialize(val, target.size());
    val.write(target.c_str(), target.size());
  }

  template<> void _bserialize(std::ostream &val, const Magnum::Vector2 &target)
  {
    _bserialize(val, target.x());
    _bserialize(val, target.y());
  }

  template<> void _bserialize(std::ostream &val, const Magnum::Vector3 &target)
  {
    _bserialize(val, target.x());
    _bserialize(val, target.y());
    _bserialize(val, target.z());
  }

  template<> void _bserialize(std::ostream &val, const Magnum::Vector4 &target)
  {
    _bserialize(val, target.x());
    _bserialize(val, target.y());
    _bserialize(val, target.z());
    _bserialize(val, target.w());
  }

  template<> void _bserialize(std::ostream &val, const Magnum::Vector2i &target)
  {
    _bserialize(val, target.x());
    _bserialize(val, target.y());
  }

  template<> void _bserialize(std::ostream &val, const Magnum::Vector3i &target)
  {
    _bserialize(val, target.x());
    _bserialize(val, target.y());
    _bserialize(val, target.z());
  }

  template<> void _bserialize(std::ostream &val, const Magnum::Vector4i &target)
  {
    _bserialize(val, target.x());
    _bserialize(val, target.y());
    _bserialize(val, target.z());
    _bserialize(val, target.w());
  }
}
class BinLoadHelper
{
public:
  static void bdeserialize(std::istream &val)
  {
    (void)val;
  }

  template <typename Last>
  static void bdeserialize(std::istream &val, Last &last)
  {
    _bdeserialize(val, last);
  }

  template <typename First, typename... Rest>
  static void bdeserialize(std::istream &val, First &first, Rest&... rest)
  {
    _bdeserialize(val, first);
    bdeserialize(val, rest...);
  }
};
namespace 
{
  template<typename _Ty> 
  void _bdeserialize(std::istream &val, _Ty &target)
  {
    target.BinLoad(val);
  }

  template<typename _Ty> 
  void _bdeserialize(std::istream &val, std::shared_ptr<_Ty> &target)
  {
    bool exist;
    _bdeserialize(val, exist);
    if (exist)
      target->BinLoad(val);
    else
      target = nullptr;
  }

  template<typename _Ty> void _bdeserialize(std::istream &val, std::vector<_Ty> &target)
  {
    size_t size;
    _bdeserialize(val, size);
    target.resize(size);
    for (decltype(target.size()) i = 0; i < target.size(); ++i)
    {
      _bdeserialize(val, target[i]);
    }
  }

  template<> void _bdeserialize(std::istream &val, int &target)
  {
    val.read(reinterpret_cast<char *>(&target), sizeof(target));
  }

  template<> void _bdeserialize(std::istream &val, unsigned short &target)
  {
    val.read(reinterpret_cast<char *>(&target), sizeof(target));
  }

  template<> void _bdeserialize(std::istream &val, bool &target)
  {
    val.read(reinterpret_cast<char *>(&target), sizeof(target));
  }
  template<> void _bdeserialize(std::istream &val, unsigned int &target)
  {
    val.read(reinterpret_cast<char *>(&target), sizeof(target));
  }

  template<> void _bdeserialize(std::istream &val, float &target)
  {
    val.read(reinterpret_cast<char *>(&target), sizeof(target));
  }

  template<> void _bdeserialize(std::istream &val, std::string &target)
  {
    size_t size;
    _bdeserialize(val, size);
    target.resize(size);
    val.read(&target[0], size);
  }

  template<> void _bdeserialize(std::istream &val, Magnum::Vector2 &target)
  {
    _bdeserialize(val, target.x());
    _bdeserialize(val, target.y());
  }

  template<> void _bdeserialize(std::istream &val, Magnum::Vector3 &target)
  {
    _bdeserialize(val, target.x());
    _bdeserialize(val, target.y());
    _bdeserialize(val, target.z());
  }

  template<> void _bdeserialize(std::istream &val, Magnum::Vector4 &target)
  {
    _bdeserialize(val, target.x());
    _bdeserialize(val, target.y());
    _bdeserialize(val, target.z());
    _bdeserialize(val, target.w());
  }

  template<> void _bdeserialize(std::istream &val, Magnum::Vector2i &target)
  {
    _bdeserialize(val, target.x());
    _bdeserialize(val, target.y());
  }

  template<> void _bdeserialize(std::istream &val, Magnum::Vector3i &target)
  {
    _bdeserialize(val, target.x());
    _bdeserialize(val, target.y());
    _bdeserialize(val, target.z());
  }

  template<> void _bdeserialize(std::istream &val, Magnum::Vector4i &target)
  {
    _bdeserialize(val, target.x());
    _bdeserialize(val, target.y());
    _bdeserialize(val, target.z());
    _bdeserialize(val, target.w());
  }
}
#endif // SERIALIZE_H