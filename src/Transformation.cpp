#include "Transformation.h"

using namespace Magnum;


Transformation::Transformation()
{
}


Transformation::~Transformation()
{
}

void Transformation::translate(const Magnum::Vector3 &vector)
{
  // global
  mMatrix = mMatrix * Math::Matrix4<Float>::translation(vector);
}

void Transformation::rotate(Math::Rad<float> angle, const Math::Vector3<float>& normalizedAxis)
{
  // local
  mMatrix = Math::Matrix4<float>::rotation(angle, normalizedAxis) * mMatrix;
}
