#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <memory>
#include "ThreadProcess.h"
#include <DataBase.h>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

class MapLoaderFromGenerator : public IMapLoader
{
public:
  MapLoaderFromGenerator(const IMapGenerator &generator, const DataBase &db);

  void Process() override;

private:
  const IMapGenerator &mGenerator;
};

class MapLoaderFromDisk : public IMapLoader
{
public:
  MapLoaderFromDisk(boost::filesystem::path path, const DataBase &db);

  void Process() override;

private:
  boost::filesystem::path mPath;
};

class MapLoaderFromNetwork : public IMapLoader
{
public:
  MapLoaderFromNetwork(boost::asio::ip::address address, const DataBase &db);

  void Process() override;

private:
  boost::asio::ip::address mAdress;
};