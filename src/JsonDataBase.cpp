#include "JsonDataBase.h"
#include "Serialize.h"
#include "Log.h"
#include <boost/filesystem.hpp>
#include "Tesselator.h"

JsonDataBase::JsonDataBase(const std::string path) : mPath(path)
{
}

JsonDataBase::~JsonDataBase()
{
}

void JsonDataBase::Load(const TextureAtlas &atlas, std::vector<std::unique_ptr<BlockStaticPart>>& storage) const
{
  int storage_before = storage.size();
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
          std::string id;
          if (val.HasMember("id"))
          {
            id = val["id"].GetString();
          }
          else
            LOG(error) << "record #" << i + 1 << " from " << file << " has no \"id\"";

          LOG(trace) << "\"" << id << "\" parsing";
          auto b = std::make_unique<BlockStaticPart>();

          if(val.HasMember("tesselator"))
          {
            rapidjson::Value &tess_json = val["tesselator"];
            if (tess_json.HasMember("type"))
            {
              std::string tess_type = tess_json["type"].GetString();
              std::unique_ptr<Tesselator> tess = TessellatorFactory::Get().Create(tess_type);
              tess->JsonLoad(tess_json, atlas);

              b->SetTesselator(std::move(tess));
            } 
          }

          /*if (val.HasMember("agents"))
          {
            rapidjson::Value &arr = val["agents"];
            if (val["agents"].IsArray())
            {
              for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
              {
                rapidjson::Value &part = arr[a];
                if (part.HasMember("type")) {
                  std::string agenttype = part["type"].GetString();
                  auto c = AgentFactory::Get().Create(StringIntern(agenttype));
                  if (!c)
                  {
                    LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown type = " << agenttype;
                    continue;
                  }
                  try {
                    c->JsonLoad(part);
                  }
                  catch (...) {
                    LOG(error) << boost::current_exception_diagnostic_information(true);
                    LOG(error) << id << "'s agent " << agenttype << " json deserialize failed. See agents documentation";
                    continue;
                  }
                  b->mAgents[StringIntern(agenttype)] = std::move(c);
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
          }*/

          storage.emplace_back(std::move(b));
        }
      }
    }
  }
  LOG(info) << storage.size() - storage_before << " loaded";
}
