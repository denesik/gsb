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

void JsonDataBase::Load(const TextureAtlas &atlas, BlocksDataBase &db) const
{
  // blocks, items
  {
    boost::filesystem::path targetDir(mPath);
    boost::filesystem::directory_iterator iter(targetDir);
    for (const boost::filesystem::path &file : iter) {
      if (is_regular_file(file) && extension(file) == ".json")
      {
        std::ifstream fs(file.string());
        std::string all((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

        LOG(trace) << "---------------";
        LOG(trace) << "parse " << file;
        rapidjson::Document d;
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

            std::string name;
            BlockId id = 0;
            std::string section;
            if (val.HasMember("name"))
            {
              name = val["name"].GetString();
            }
            else
              LOG(error) << "record #" << i << " from " << file << " has no \"name\"";

            if (val.HasMember("id"))
            {
              id = val["id"].GetUint();
            }
            else
            {
              LOG(error) << "record #" << i << " from " << file << " has no \"id\". SKIP.";
              continue;
            }

            LOG(trace) << "\"" << name << "\" parsing";

            if (val.HasMember("section"))
            {
              section = val["section"].GetString();
            }
            else
            {
              LOG(error) << "record #" << i << " from " << file << " has no \"section\". SKIP.";
              continue;
            }

            //TODO: переделать на наследников BlockStaticPart, как в секции рецептов, унифицировать разбор разных секций
            if (section == "block")
            {
              auto static_part = std::make_unique<BlockStaticPart>();
              std::unique_ptr<BlockDynamicPart> dynamic_part = nullptr;

              if (val.HasMember("tesselator"))
              {
                rapidjson::Value &tess_json = val["tesselator"];
                if (tess_json.HasMember("type"))
                {
                  std::string tess_type = tess_json["type"].GetString();
                  std::unique_ptr<Tesselator> tess = TessellatorFactory::Get().Create(tess_type);
                  tess->JsonLoad(tess_json, atlas);

                  static_part->SetTesselator(std::move(tess));

                  if (static_part->GetTesselator()->UseTesselatorData())
                  {
                    dynamic_part = std::make_unique<BlockDynamicPart>();
                    dynamic_part->mTesselatorData = std::make_unique<TesselatorData>();
                  }
                }
              }

              if (val.HasMember("agents"))
              {
                if (!dynamic_part)
                  dynamic_part = std::make_unique<BlockDynamicPart>();

                rapidjson::Value &arr = val["agents"];
                if (val["agents"].IsArray())
                {
                  for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
                  {
                    rapidjson::Value &part = arr[a];
                    if (part.HasMember("type"))
                    {
                      std::string agenttype = part["type"].GetString();
                      auto agent = AgentFactory::Get().Create(agenttype);
                      if (!agent)
                      {
                        LOG(error) << "record \"" << id << "\" agent #" << a << " has unknown type = " << agenttype << ". SKIP.";
                        continue;
                      }
                      try {
                        agent->JsonLoad(db, part);
                      }
                      catch (...) {
                        LOG(error) << boost::current_exception_diagnostic_information(true);
                        LOG(error) << id << "'s agent " << agenttype << " json deserialize failed. See agents documentation. SKIP.";
                        continue;
                      }
                      dynamic_part->mAgents.insert(std::make_pair(agent->Id(), std::move(agent)));
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
            }
            else if (section == "item")
            {
              std::string type = "Item";

              if (val.HasMember("type"))
                type = val["type"].GetString();

              auto recipe = ItemFactory::Get().Create(type);
              if (!recipe)
              {
                LOG(error) << "item #" << id << " has unknown type = " << type << ". SKIP.";
                continue;
              }
              try {
                recipe->JsonLoad(db, val);
              }
              catch (...) {
                LOG(error) << boost::current_exception_diagnostic_information(true);
                LOG(error) << "item #" << id << " type = " << type << " json deserialize failed. See agents documentation. SKIP.";
                continue;
              }

              db.AddItem(name, id, std::move(recipe));
            }
          }
        }
      }
    }
  }

  //recipes
  {
    boost::filesystem::path targetDir(mPath + "/recipes/");
    boost::filesystem::directory_iterator iter(targetDir);
    for (const boost::filesystem::path &file : iter) {
      if (is_regular_file(file) && extension(file) == ".json")
      {
        std::ifstream fs(file.string());
        std::string all((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

        LOG(trace) << "---------------";
        LOG(trace) << "parse " << file;
        rapidjson::Document d;
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
            std::string type = "Recipe";

            if (val.HasMember("type"))
              type = val["type"].GetString();

            auto recipe = RecipeFactory::Get().Create(type);
            if (!recipe)
            {
              LOG(error) << "recipe has unknown type = " << type << ". SKIP.";
              continue;
            }
            try {
              recipe->JsonLoad(db, val);
            }
            catch (...) {
              LOG(error) << boost::current_exception_diagnostic_information(true);
              LOG(error) << "recipe type = " << type << " json deserialize failed. See agents documentation. SKIP.";
              continue;
            }

            db.AddRecipe(std::move(recipe));
          }
        }
      }
    }
    //LOG(info) << storage.size() - storage_before << " loaded";
  }
}
