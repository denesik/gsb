#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <memory>
#include "ThreadProcess.h"
#include <DataBase.h>
#include <boost/filesystem.hpp>

//TODO: rename to sector loader
class MapLoaderFromGenerator : public IMapLoader
{
public:
  MapLoaderFromGenerator(const IMapGenerator &generator, const DataBase &db);

  void Process(Sector &sector) override;

private:
  const IMapGenerator &mGenerator;
};

class MapLoaderFromDisk : public IMapLoader
{
public:
  MapLoaderFromDisk(boost::filesystem::path path, const DataBase &db);

  void Process(Sector &sector) override;

private:
  boost::filesystem::path mPath;
};

//class MapLoaderFromNetwork : public IMapLoader
//{
//public:
//  MapLoaderFromNetwork(boost::asio::ip::address address, const DataBase &db);
//
//  void Process() override;
//
//private:
//  boost::asio::ip::address mAddress;
//};

// class MapSaverToDisk : public IMapSaver
// {
// public:
//   MapSaverToDisk(boost::filesystem::path path, const DataBase &db);
// 
//   void Process() override;
// 
// private:
//   boost::filesystem::path mPath;
// };
