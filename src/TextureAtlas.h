#pragma once

#include <string>
#include <Magnum/Math/Range.h>
#include <vector>
#include <Magnum/Texture.h>

class TextureAtlas
{
public:
  TextureAtlas();
  ~TextureAtlas();

  void LoadDirectory(const std::string &name);

  Magnum::Texture2D &Texture();

private:
  std::vector<std::string> file_names;
  std::vector<Magnum::Range2D> coordinates;
  Magnum::Texture2D _texture;
};

