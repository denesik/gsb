#ifndef Magnum_Examples_TexturedTriangleShader_h
#define Magnum_Examples_TexturedTriangleShader_h


#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Framebuffer.h>
#include <Magnum/TextureArray.h>

class StandartShader : public Magnum::AbstractShaderProgram
{
public:
  using Position = Magnum::Attribute<0, Magnum::Vector3>;
  using TextureCoordinates = Magnum::Attribute<1, Magnum::Vector2>;
  using Normal = Magnum::Attribute<2, Magnum::Vector3>;
  constexpr static size_t ShadowMapLevels = 4;

  explicit StandartShader();

  StandartShader& setProjection(const Magnum::Matrix4& mat);

  StandartShader& setShadowMatrix(const Magnum::Containers::ArrayView<const Magnum::Matrix4>& mat);

  StandartShader& setLightVector(const Magnum::Vector3& light);

  StandartShader& setTexture(Magnum::Texture2D& texture);

  StandartShader& setShadowDepthTexture(Magnum::Texture2DArray& shadowDepth);

private:
  enum : Magnum::Int { TextureLayer = 0, shadowmapTexture = 2 };

  Magnum::Int mProjectionMatrixUniform;
  Magnum::Int mShadowmapMatrixUniform;
  Magnum::Int mLightVectorUniform;
};

class SingleFileShader : Magnum::AbstractShaderProgram
{

};

class ShadowShader : public Magnum::AbstractShaderProgram
{
public:
  typedef Magnum::Attribute<0, Magnum::Vector3> Position;

  explicit ShadowShader();

  ShadowShader& setProjection(const Magnum::Matrix4& mat)
  {
    setUniform(mUniformProjection, mat);
    return *this;
  }

private:
  Magnum::Int mUniformProjection;
};

class TexGenShader : public Magnum::AbstractShaderProgram
{
public:
  typedef Magnum::Attribute<0, Magnum::Vector3> Position;

  explicit TexGenShader();

  TexGenShader& setTexture(Magnum::Texture2D& texture) {
    texture.bind(TextureLayer);
    return *this;
  }

private:
  enum : Magnum::Int { TextureLayer = 0 };
};

#endif
