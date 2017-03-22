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
#include <Magnum/ImageView.h>

using namespace Corrade::Utility;
using namespace Magnum;

TextureAtlas::TextureAtlas(const Magnum::Vector2i &max_size)
  : mSize(max_size)
{
}


TextureAtlas::~TextureAtlas()
{
}

void TextureAtlas::LoadDirectory(const std::string &directory)
{
  PluginManager::Manager<Trade::AbstractImporter> manager{ MAGNUM_PLUGINS_IMPORTER_DIR };
  std::unique_ptr<Trade::AbstractImporter> importer = manager.loadAndInstantiate("PngImporter");
  if (!importer) std::exit(1);

  auto files = Directory::list(directory, Directory::Flag::SkipDirectories);

  //std::vector<Trade::ImageData2D> images;
  //std::vector<Vector2i> mSizes;

  for (const auto &name : files)
  {
    if (importer->openFile(Directory::join(directory, name)))
    {
      std::optional<Trade::ImageData2D> image = importer->image2D(0);
      if (image)
      {
        file_names.push_back(Directory::join(directory, name));
        mSizes.push_back(image->size());
        mImageData.emplace_back(std::move(*image));
        mImageViews.emplace_back(mImageData.back());
      }
    }
  }
}

void TextureAtlas::AddImage(const std::string &name, const Magnum::ImageView2D &image)
{
  file_names.push_back(name);
  mSizes.push_back(image.size());
  mImageViews.emplace_back(image);
}

void TextureAtlas::Fill(Magnum::Texture2D &texture)
{
  auto ranges = TextureTools::atlas(mSize, mSizes);

  for (const auto &r : ranges)
  {
    Vector2 scale(1.0f / (static_cast<Vector2>(mSize)));
    coordinates.push_back({ static_cast<Vector2>(r.bottomLeft()) * scale , static_cast<Vector2>(r.topRight()) * scale });
  }

  for (size_t i = 0; i < mImageViews.size(); ++i)
  {
    texture.setSubImage(0, ranges[i].bottomLeft(), mImageViews[i]);
  }

  mImageData.clear();
  mImageData.shrink_to_fit();  
  mImageViews.clear();
  mImageViews.shrink_to_fit();
  mSizes.clear();
  mSizes.shrink_to_fit();
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

const Magnum::Vector2i & TextureAtlas::Size() const
{
  return mSize;
}

