#pragma once

#include <string>
#include <Magnum/Math/Range.h>
#include <vector>
#include <Magnum/Texture.h>
#include <MagnumExternal/Optional/optional.hpp>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/ImageView.h>


class TextureAtlas
{
public:
  TextureAtlas(const Magnum::Vector2i &max_size = Magnum::Vector2i(1024));
  ~TextureAtlas();

  void LoadDirectory(const std::string &name);

  void AddImage(const std::string &name, const Magnum::ImageView2D &image);

  void Fill(Magnum::Texture2D &texture);

  std::optional<Magnum::Range2D> GetTextureCoord(const std::string &name) const;

  const Magnum::Vector2i &Size() const;

private:
  std::vector<Magnum::Trade::ImageData2D> mImageData;
  std::vector<Magnum::ImageView2D> mImageViews;
  std::vector<Magnum::Vector2i> mSizes;

  std::vector<std::string> file_names;
  std::vector<Magnum::Range2D> coordinates;
  Magnum::Vector2i mSize;
};

