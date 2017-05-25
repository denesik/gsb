#ifndef _WORLD_SAVE_LOAD__H_
#define _WORLD_SAVE_LOAD__H_
#pragma once

#include <boost/optional.hpp>
#include <memory>

class Sector;

/// ������ ����������� ��� �������� �� ����.
/// ���������� ����������� ������.
/// ��� ��������, ����������� ����� �� ��������� ������ �� �����.
/// ��������� �� ����������� �� ������, ��� ���� �� ���� ��� ��������.
/// ���� ������ �����������, ���������� ��������� � �������� ������.
/// ���� ������ ����, ������ �� �����.
class WorldSaveLoad
{
public:
  /// ��������� ������.
  /// ���������� true ���� ������ ����� ��������� �� �������� � false � ��������� ������.
  bool Load(Sector &sector);

  /// ��������� ������.
  /// ���������� true ���� ������ ����� �������� � false � ��������� ������.
  /// ������ ����� ���� �� ��������� �� ����������, ���� �� ����������� � ������ ������.
  bool Save(const Sector &sector);

  void Update();

private:
  struct Data
  {
    enum 
    {
      WAIT,
      SAVE,
      LOAD,
    } status = WAIT; // ����� �������� ����������� � ������ ������.
    
    boost::optional<Sector &> sector_for_load;
    std::unique_ptr<Sector> sector_for_save;
  };

};


#endif
