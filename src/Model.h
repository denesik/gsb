#pragma once
#ifndef Model_h__
#define Model_h__

#include <Magnum/Magnum.h>
#include <Magnum/Mesh.h>
#include <Magnum/Buffer.h>
#include <Magnum/AbstractShaderProgram.h>

class DataBase;

// TODO: Переделать все это...
// Модель, содержит все данные для рисования.
// Умеет рисоваться.
class Model
{
public:
  Model(const DataBase &db);

  void Draw(const Magnum::Matrix4 &mvp, Magnum::AbstractShaderProgram& shader, size_t frame) const;

private:
  struct
  {
    Magnum::Buffer vertex_buffer;
    Magnum::Buffer index_buffer;
    mutable Magnum::Mesh mesh;
  } data[16];

};



#endif // Model_h__
