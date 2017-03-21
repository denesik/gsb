#pragma once
#ifndef ChunkHelper_h__
#define ChunkHelper_h__

#include <memory>
#include <array>
#include "tools\CoordSystem.h"

// ������ ������� �� ������.
// ����� ����� ��� ���������� ����� (������) � ��������� (������).
// ��������� �� ������ � �������. ��� ������� ����� ������������� ���������, ���� �����.

enum
{
  T_SECTOR_SIZE = 16,
  T_SECTOR_HEIGHT = 256,

  CHUNK_SIZE = 8,
  CHUNK_CAPACITY = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE,

  CHUNK_TABLE_SIZE = T_SECTOR_SIZE / CHUNK_SIZE,
  CHUNK_TABLE_HEIGHT = T_SECTOR_HEIGHT / CHUNK_SIZE,
  CHUNK_TABLE_CAPACITY = CHUNK_TABLE_SIZE * CHUNK_TABLE_SIZE * CHUNK_TABLE_HEIGHT,

  AROUND_MIDDLE = 1,
  AROUND_NEIGHBOR_1 = 6,
  AROUND_NEIGHBOR_2 = 12,
  AROUND_NEIGHBOR_3 = 8,
  AROUND_CAPACITY = AROUND_MIDDLE + AROUND_NEIGHBOR_1 + AROUND_NEIGHBOR_2 + AROUND_NEIGHBOR_3,

  CHUNK_MIDDLE_CAPACITY = CHUNK_CAPACITY,
  CHUNK_NEIGHBOR_1_CAPACITY = CHUNK_SIZE * CHUNK_SIZE,
  CHUNK_NEIGHBOR_2_CAPACITY = CHUNK_SIZE,
  CHUNK_NEIGHBOR_3_CAPACITY = 1,

  T_CAPACITY =
  CHUNK_MIDDLE_CAPACITY * AROUND_MIDDLE +
  CHUNK_NEIGHBOR_1_CAPACITY * AROUND_NEIGHBOR_1 +
  CHUNK_NEIGHBOR_2_CAPACITY * AROUND_NEIGHBOR_2 +
  CHUNK_NEIGHBOR_3_CAPACITY * AROUND_NEIGHBOR_3,

};

class Sector;

struct SectorAround
{
  enum 
  {

  };
  std::weak_ptr<Sector> around[AROUND_CAPACITY];
};

// ������ ������� ������ ��������� � ������� �������.
struct Around
{
  std::array<IndexType, AROUND_MIDDLE> middle;
  std::array<IndexType, AROUND_NEIGHBOR_1> neighbor_1;
  std::array<IndexType, AROUND_NEIGHBOR_2> neighbor_2;
  std::array<IndexType, AROUND_NEIGHBOR_3> neighbor_3;
};

// ����� ������ ������� � ���� ������ �������� 1 � �������� ��������.
struct BlockAround
{
  // ������� ������ � ������� ��� �������� �����.
  std::array<std::array<IndexType, CHUNK_MIDDLE_CAPACITY>, AROUND_MIDDLE> middle;

  // ������� ������ � ������� ��� 1-�� ��������� �����.
  std::array<std::array<IndexType, CHUNK_NEIGHBOR_1_CAPACITY>, AROUND_NEIGHBOR_1> neighbor_1;

  // ������� ������ � ������� ��� 2-� �������� ������.
  std::array<std::array<IndexType, CHUNK_NEIGHBOR_2_CAPACITY>, AROUND_NEIGHBOR_2> neighbor_2;

  // ������� ������ � ������� ��� 3-� �������� ������.
  std::array<std::array<IndexType, CHUNK_NEIGHBOR_3_CAPACITY>, AROUND_NEIGHBOR_3> neighbor_3;
};

// ������ ����� � ������ ������� ������ �������� �����.
// ���� ���� � �������� �������, ������ ��������� ��� ����� � �������� �������. 
struct ChunkAround
{
  IndexType sector;
  IndexType chunk;
};

struct ChunkInfo
{
  // ������� �������� �� ������� ������ �������� �����.
  std::array<ChunkAround, AROUND_CAPACITY> around;

  // ������� ������ ��� ���� � ������ ������� ������� ����.
  BlockAround blocks;

  // ������� ������������ ��� ���� � ������ ������� ������� ����.
  BlockAround tesselators;
};

// ��� ��������� ����� � �����, ������� ������ ����� ����� � ������ ���� �� ��������� �����.
// ���� ���� �� ������� ������, ������ ���� �� ��������� � ��������� �����.
// ����������� ��� ������� ����� � ������� ������ ����� ������ � ������� ��������� ����.
// �������� �����, � ������� ������� ������� ����� ������������� ������ ������.
// ��� ������� ������� ����� ����������� ������ �������� �� �����.
// ��� ������� ������� ����� ����������� ������ �������� �� �����������.
struct ChunkHelper
{
  // ��� ������� ����� � ������� ������������ ������ �����, � ������� ��������� ����.
  std::array<IndexType, SECTOR_CAPACITY> index_chunk_in_sector;

  std::array<ChunkInfo, CHUNK_TABLE_CAPACITY> chunk_info;
};

void test();

#endif // ChunkHelper_h__
