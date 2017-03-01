#pragma once

#include <string>
#include <Magnum/Math/Range.h>
#include <vector>
#include <Magnum/Texture.h>
#include <MagnumExternal/Optional/optional.hpp>

class TextureAtlas
{
public:
  TextureAtlas();
  ~TextureAtlas();

  void LoadDirectory(const std::string &name);

  Magnum::Texture2D &Texture();

  std::optional<Magnum::Range2D> GetTextureCoord(const std::string &name) const;

private:
  std::vector<std::string> file_names;
  std::vector<Magnum::Range2D> coordinates;
  Magnum::Texture2D _texture;
};

