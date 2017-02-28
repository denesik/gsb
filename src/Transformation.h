#pragma once
#ifndef Transformation_h__
#define Transformation_h__

#include <Magnum/Math/Matrix4.h>
#include <Magnum/Magnum.h>


class Transformation
{
public:
  Transformation();
  ~Transformation();

  void translate(const Magnum::Vector3 &vector);

  void rotate(Magnum::Math::Rad<float> angle, const Magnum::Math::Vector3<float>& normalizedAxis);

private:
  Magnum::Matrix4 mMatrix;

};



#endif // Transformation_h__
