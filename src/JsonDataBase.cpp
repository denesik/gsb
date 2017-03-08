#include "JsonDataBase.h"
#include "Serialize.h"
#include "Log.h"
#include <boost/filesystem.hpp>
#include "Tesselator.h"
#include "BlocksDataBase.h"
#include <boost/exception/diagnostic_information.hpp>
#include "agent/Agents.hpp"

JsonDataBase::JsonDataBase(const std::string path) : mPath(path)
{
}

JsonDataBase::~JsonDataBase()
{
}

void JsonDataBase::Load(const TextureAtlas &atlas, BlocksDataBase &db) const
{
  int json_count = 0;
  {
    boost::filesystem::path targetDir(mPath);
    boost::filesystem::recursive_directory_iterator iter(targetDir);
    for (const boost::filesystem::path &file : iter) {
      if (is_regular_file(file) && extension(file) == ".json")
        json_count++;
    }
  }
  //Game::SetLoadPhase(0, "Json loading", 0, 4);
  boost::filesystem::path targetDir(mPath);
  boost::filesystem::recursive_directory_iterator iter(targetDir);
  float json_current = 0;
  for (const boost::filesystem::path &file : iter) {
    if (is_regular_file(file) && extension(file) == ".json")
    {
      json_current++;
      //Game::SetLoadPhase(json_current / json_count);
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

          bool dyn = false;
          std::string name;
          BlockId id = 0;
          if (val.HasMember("name"))
          {
            name = val["name"].GetString();
          }
          else
            LOG(error) << "record #" << i + 1 << " from " << file << " has no \"name\"";

          if (val.HasMember("id"))
          {
            id = val["id"].GetUint();
          }
          else
            LOG(error) << "record #" << i + 1 << " from " << file << " has no \"id\"";

          LOG(trace) << "\"" << name << "\" parsing";
          auto static_part = std::make_unique<BlockStaticPart>();
          std::unique_ptr<BlockDynamicPart> dynamic_part = nullptr;

          if(val.HasMember("tesselator"))
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
            if(!dynamic_part)
              dynamic_part = std::make_unique<BlockDynamicPart>();

            rapidjson::Value &arr = val["agents"];
            if (val["agents"].IsArray())
            {
              for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
              {
                rapidjson::Value &part = arr[a];
                if (part.HasMember("type")) {
                  std::string agenttype = part["type"].GetString();
                  auto agent = AgentFactory::Get().Create(agenttype);
                  if (!agent)
                  {
                    LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown type = " << agenttype;
                    continue;
                  }
                  try {
                    agent->JsonLoad(part);
                  }
                  catch (...) {
                    LOG(error) << boost::current_exception_diagnostic_information(true);
                    LOG(error) << id << "'s agent " << agenttype << " json deserialize failed. See agents documentation";
                    continue;
                  }
                  dynamic_part->mAgents.emplace_back(std::move(agent));
                }
                else
                {
                  LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type";
                }
              }
            }
            else
            {
              LOG(error) << "record \"" << id << "\" parts is not valid agents array";
            }
          }

          db.AddBlock(name, id, std::move(static_part), std::move(dynamic_part));
        }
      }
    }
  }
  //LOG(info) << storage.size() - storage_before << " loaded";
}
