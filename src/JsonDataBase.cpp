#include "JsonDataBase.h"
#include "Serialize.h"
#include "Log.h"
#include <boost/filesystem.hpp>
#include "Tesselator.h"
#include "DataBase.h"
#include <boost/exception/diagnostic_information.hpp>
#include "Item.h"
#include "DataBaseFakeData.h"
#include <string>

JsonDataBase::JsonDataBase(const std::string path) : mPath(path)
{
}

JsonDataBase::~JsonDataBase()
{
}

bool LoadItem(const TextureAtlas &atlas, DataBase &db, const rapidjson::Value &val, const std::string & name, int id)
{
  std::string type = "Item";

  if (val.HasMember("type"))
    type = val["type"].GetString();

  auto item = IItem::factory::create(type);
  if (!item)
  {
    LOG(error) << "item #" << 0 << " has unknown type = " << type << ". SKIP.";
    return false;
  }
  try {
	  item->JsonLoad(db, val);
  }
  catch (...) {
    LOG(error) << boost::current_exception_diagnostic_information(true);
    LOG(error) << "item #" << 0 << " type = " << type << " json deserialize failed. See agents documentation. SKIP.";
    return false;
  }

  if(!name.empty())
	item->SetName(name);
  else
	item->SetName(std::string("id") + std::to_string(id));

  if (val.HasMember("description"))
	  item->SetDescription(val["description"].GetString());

  db.AddItem(name, id, std::move(item));
  return true;
}

bool LoadBlock(const TextureAtlas &atlas, DataBase &db, const rapidjson::Value &val, const std::string & name, BlockId id)
{
  auto static_part = std::make_unique<StaticBlock>();
  std::unique_ptr<Block> dynamic_part = nullptr;

  if (val.HasMember("model"))
  {
    static_part->TestAddModel(db);
  }

  if (val.HasMember("type"))
  {
    std::string type = val["type"].GetString();
    if (!dynamic_part)
      dynamic_part = Block::factory::create(type, db, val, db.GetFakeData().sector, id);
  }

  if (val.HasMember("tesselator"))
  {
    const rapidjson::Value &tess_json = val["tesselator"];
    if (tess_json.HasMember("type"))
    {
      std::string tess_type = tess_json["type"].GetString();
      std::unique_ptr<Tesselator> tess = Tesselator::factory::create(tess_type);
      tess->JsonLoad(tess_json, atlas);

      static_part->SetTesselator(std::move(tess));
    }
  }

  if (val.HasMember("drop"))
  {
    const rapidjson::Value &drop = val["drop"];
    std::vector<ItemId> dropList;
    for (const auto &d : drop.GetArray())
    {
      if(auto id = db.ItemIdFromName(d.GetString()))
        dropList.push_back(id.get());
    }

    static_part->SetDropList(dropList);
  }

  db.AddBlock(name, id, std::move(static_part), std::move(dynamic_part));
  return true;
}

bool LoadRecipe(const TextureAtlas &atlas, DataBase &db, const rapidjson::Value &val, const std::string & name, int id)
{
  std::string type;

  if (val.HasMember("type"))
    type = val["type"].GetString();

  auto recipe = IRecipe::factory::create("IRecipe");
  if (!recipe)
  {
    LOG(error) << "recipe has unknown type = " << type << ". SKIP.";
    return false;
  }
  try {
    recipe->JsonLoad(db, val);
  }
  catch (...) {
    LOG(error) << boost::current_exception_diagnostic_information(true);
    LOG(error) << "recipe type = " << type << " json deserialize failed. See agents documentation. SKIP.";
    return false;
  }

  db.AddRecipe(type, std::move(recipe));
  return true;
}

static std::unordered_map<std::string, std::function<bool(const TextureAtlas &, DataBase &, const rapidjson::Value &, const std::string &, int)>> section_loaders = {
  { "block", &LoadBlock },
  { "item", &LoadItem },
  { "recipe", &LoadRecipe },
};


void JsonDataBase::Load(const TextureAtlas &atlas, DataBase &db) const
{
  boost::filesystem::path targetDir(mPath);
  boost::filesystem::directory_iterator iter(targetDir);

  std::vector<rapidjson::Document> documents;
  std::unordered_map<std::string, std::vector<const rapidjson::Value *>> sections;
  std::vector<std::pair<std::string, const std::vector<const rapidjson::Value *> &>> ordered_section;

  // сортируем записи по секциям
  for (const boost::filesystem::path &file : iter) {
    if (is_regular_file(file) && extension(file) == ".json")
    {
      std::ifstream fs(file.string());
      std::string all((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

      LOG(trace) << "---------------";
      LOG(trace) << "parse " << file;
      documents.emplace_back();
      rapidjson::Document &d = *(documents.end() - 1);
      d.Parse<0>(all.c_str());

      if (d.HasParseError())
      {
        LOG(error) << "while parsing " << file;
        LOG(error) << d.GetParseError();
        LOG(error) << all.substr(std::max<int>(d.GetErrorOffset() - 20, 0), std::min<int>(all.length(), 40));
        LOG(error) << "                    ^";
        throw;
      }

      if (d.IsArray())
      {
        for (decltype(d.Size()) i = 0; i < d.Size(); i++)
        {
          rapidjson::Value &val = d[i];

          std::string section;

          if (val.HasMember("section"))
          {
            section = val["section"].GetString();
          }
          else
          {
            LOG(error) << "record #" << i << " from " << file << " has no \"section\". SKIP.";
            continue;
          }

          sections[section].emplace_back(&val);
        }
      }
    }
  }

  // сортируем секции
  ordered_section.emplace_back("item", sections["item"]);
  ordered_section.emplace_back("recipe", sections["recipe"]);
  ordered_section.emplace_back("block", sections["block"]);

  // десериализуем
  for (const auto section : ordered_section)
  {
    for (auto & val : section.second)
    {
      std::string name;
      BlockId id = 0;

      if (val->HasMember("name"))
      {
        name = (*val)["name"].GetString();
      }
      else
        LOG(error) << "record #" << 0 << " from " << 0 << " has no \"name\"";

      if (val->HasMember("id"))
      {
        id = (*val)["id"].GetUint();
      }
      else
      {
        LOG(error) << "record #" << 0 << " from " << 0 << " has no \"id\". SKIP.";
        continue;
      }

      LOG(trace) << "\"" << name << "\" parsing";

      section_loaders[section.first](atlas, db, *val, name, id);
    }
  }
}