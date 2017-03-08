#include "JsonDataBase.h"
#include "Serialize.h"
#include "Log.h"
#include <boost/filesystem.hpp>
#include "Tesselator.h"
#include "BlocksDataBase.h"
#include <boost/exception/diagnostic_information.hpp>
#include "agent/Agents.hpp"
#include "Item.h"

JsonDataBase::JsonDataBase(const std::string path) : mPath(path)
{
}

JsonDataBase::~JsonDataBase()
{
}

bool LoadItem(const TextureAtlas &atlas, BlocksDataBase &db, const rapidjson::Value &val, const std::string & name, int id)
{
  std::string type = "Item";

  if (val.HasMember("type"))
    type = val["type"].GetString();

  auto recipe = ItemFactory::Get().Create(type);
  if (!recipe)
  {
    LOG(error) << "item #" << 0 << " has unknown type = " << type << ". SKIP.";
    return false;
  }
  try {
    recipe->JsonLoad(db, val);
  }
  catch (...) {
    LOG(error) << boost::current_exception_diagnostic_information(true);
    LOG(error) << "item #" << 0 << " type = " << type << " json deserialize failed. See agents documentation. SKIP.";
    return false;
  }

  db.AddItem(name, id, std::move(recipe));
  return true;
}

bool LoadBlock(const TextureAtlas &atlas, BlocksDataBase &db, const rapidjson::Value &val, const std::string & name, int id)
{
  auto static_part = std::make_unique<BlockStaticPart>();
  std::unique_ptr<BlockDynamicPart> dynamic_part = nullptr;

  if (val.HasMember("tesselator"))
  {
    const rapidjson::Value &tess_json = val["tesselator"];
    if (tess_json.HasMember("type"))
    {
      std::string tess_type = tess_json["type"].GetString();
      std::unique_ptr<Tesselator> tess = TessellatorFactory::Get().Create(tess_type);
      tess->JsonLoad(tess_json, atlas);

      static_part->SetTesselator(std::move(tess));

      if (static_part->GetTesselator()->UseTesselatorData())
      {
        dynamic_part = std::make_unique<BlockDynamicPart>(id, db);
        dynamic_part->GetTesselatorData() = std::make_unique<TesselatorData>();
      }
    }
  }

  if (val.HasMember("agents"))
  {
    if (!dynamic_part)
      dynamic_part = std::make_unique<BlockDynamicPart>(id, db);

    const rapidjson::Value &arr = val["agents"];
    if (val["agents"].IsArray())
    {
      for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
      {
        const rapidjson::Value &part = arr[a];
        if (part.HasMember("type"))
        {
          std::string agenttype = part["type"].GetString();
          auto agent = AgentFactory::Get().Create(agenttype);
          if (!agent)
          {
            LOG(error) << "record \"" << id << "\" agent #" << a << " has unknown type = " << agenttype << ". SKIP AGENT.";
            continue;
          }
          try {
            agent->JsonLoad(db, part);
          }
          catch (...) {
            LOG(error) << boost::current_exception_diagnostic_information(true);
            LOG(error) << id << "'s agent " << agenttype << " json deserialize failed. See agents documentation. SKIP AGENT.";
            continue;
          }
          dynamic_part->AddAgent(std::move(agent));
        }
        else
        {
          LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type.";
        }
      }
    }
    else
    {
      LOG(error) << "record \"" << id << "\" parts is not valid agents array.";
    }
  }

  db.AddBlock(name, id, std::move(static_part), std::move(dynamic_part));
  return true;
}

bool LoadRecipe(const TextureAtlas &atlas, BlocksDataBase &db, const rapidjson::Value &val, const std::string & name, int id)
{
  std::string type;

  if (val.HasMember("type"))
    type = val["type"].GetString();

  auto recipe = RecipeFactory::Get().Create(type);
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

  db.AddRecipe(std::move(recipe));
  return true;
}

static std::unordered_map<std::string, std::function<bool(const TextureAtlas &, BlocksDataBase &, const rapidjson::Value &, const std::string &, int)>> section_loaders = {
  { "block", &LoadBlock },
  { "item", &LoadItem },
  { "recipe", &LoadRecipe },
};


void JsonDataBase::Load(const TextureAtlas &atlas, BlocksDataBase &db) const
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
  for (auto & sec : sections)
  {
    if (sec.first != "recipe")
      ordered_section.emplace_back(sec.first, sec.second);
  }
  ordered_section.emplace_back("recipe", sections["recipe"]);

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