#ifndef Magnum_Examples_TexturedTriangleShader_h
#define Magnum_Examples_TexturedTriangleShader_h


#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

using namespace Magnum;

class StandartShader : public AbstractShaderProgram {
public:
	typedef Attribute<0, Vector3> Position;
	typedef Attribute<1, Vector2> TextureCoordinates;
	typedef Attribute<2, Vector3> Normal;

	explicit StandartShader();

  StandartShader& setProjection(const Matrix4& mat) {
    setUniform(mUniformProjection, mat);
    return *this;
  }

  StandartShader& setShadowMatrix(const Matrix4& mat) {
	  setUniform(mShadowProjection, mat);
	  return *this;
  }

  StandartShader& setLightVector(const Vector3& light) {
	  setUniform(mLightVector, light);
	  return *this;
  }

	StandartShader& setTexture(Texture2D& texture) {
		texture.bind(TextureLayer);
		return *this;
	}

	StandartShader& setShadowDepthTexture(Texture2D& shadowDepth) {
		shadowDepth.bind(ShadowDepth);
		return *this;
	}

private:
	enum : Int { TextureLayer = 0, ShadowDepth = 1 };

  Int mUniformProjection;
  Int mShadowProjection;
  Int mLightVector;
};


#endif
