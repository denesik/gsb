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
  typedef Attribute<4, Vector3> Normal;

	explicit StandartShader();

	StandartShader& setColor(const Color3& color) {
		setUniform(_colorUniform, color);
		return *this;
	}

  StandartShader& setProjection(const Matrix4& mat) {
    setUniform(mUniformProjection, mat);
    return *this;
  }

	StandartShader& setTexture(Texture2D& texture) {
		texture.bind(TextureLayer);
		return *this;
	}

private:
	enum : Int { TextureLayer = 0 };

	Int _colorUniform;

  Int mUniformProjection;
};


#endif
