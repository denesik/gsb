#include "TextureAtlas.h"

#include "configure.h"
#include <Corrade/Utility/Directory.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/TextureTools/Atlas.h>
#include <Magnum/TextureFormat.h>
#include <Magnum/PixelFormat.h>
#include <algorithm>

using namespace Corrade::Utility;
using namespace Magnum;

TextureAtlas::TextureAtlas()
{
}


TextureAtlas::~TextureAtlas()
{
}

void TextureAtlas::LoadDirectory(const std::string &directory)
{
  PluginManager::Manager<Trade::AbstractImporter> manager{ MAGNUM_PLUGINS_IMPORTER_DIR };
  std::unique_ptr<Trade::AbstractImporter> importer = manager.loadAndInstantiate("TgaImporter");
  if (!importer) std::exit(1);

  auto files = Directory::list(directory, Directory::Flag::SkipDirectories);

  std::vector<Trade::ImageData2D> images;
  std::vector<Vector2i> sizes;

  for (const auto &name : files)
  {
    if (importer->openFile(Directory::join(directory, name)))
    {
      std::optional<Trade::ImageData2D> image = importer->image2D(0);
      if (image)
      {
        file_names.push_back(Directory::join(directory, name));
        sizes.push_back(image->size());
        images.push_back(std::move(*image));
      }
    }
  }

  Vector2i atlas_size(1024);
  auto ranges = TextureTools::atlas(atlas_size, sizes);

  for (const auto &r : ranges)
  {
    Vector2 scale(1.0f / (static_cast<Vector2>(atlas_size)));
    coordinates.push_back({ static_cast<Vector2>(r.bottomLeft()) * scale , static_cast<Vector2>(r.topRight()) * scale });
  }

  _texture.setWrapping(Sampler::Wrapping::ClampToEdge)
    .setMagnificationFilter(Sampler::Filter::Linear)
    .setMinificationFilter(Sampler::Filter::Linear);

  // Перед использованием setSubImage нужно залить текстуру данными с помощью setImage
  {
    std::vector<UnsignedByte> zero_data(atlas_size.x() * atlas_size.y() * 3);
    _texture.setImage(0, TextureFormat::RGB8,
      ImageView2D(PixelFormat::RGB, PixelType::UnsignedByte, atlas_size, {zero_data.data(), zero_data.size()}));
  }

  for (size_t i = 0; i < images.size(); ++i)
  {
    _texture.setSubImage(0, ranges[i].bottomLeft(), images[i]); 
  }

}

Magnum::Texture2D & TextureAtlas::Texture()
{
  return _texture;
}

std::optional<Magnum::Range2D> TextureAtlas::GetTextureCoord(const std::string &name) const
{
  auto it = std::find(file_names.begin(), file_names.end(), name);
  if (it != file_names.end())
  {
    auto index = std::distance(file_names.begin(), it);
    return{ coordinates[index] };
  }
  return{};
}

