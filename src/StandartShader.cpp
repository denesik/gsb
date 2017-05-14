#include "StandartShader.h"

#include <Corrade/Utility/Resource.h>
#include <Magnum/Context.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>

using namespace Magnum;

StandartShader::StandartShader()
{
  MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

  Shader vert{ Version::GL330, Shader::Type::Vertex };
  Shader frag{ Version::GL330, Shader::Type::Fragment };

  constexpr char *peamble =
    "\n"
    "#define GSB_SHADOWMAP_LGHT\n"
    "#define GSB_NORMAL_LGHT\n";

  std::string shadowLevels = "#define NUM_SHADOW_MAP_LEVELS " + std::to_string(ShadowMapLevels) + "\n";

  constexpr char *vertDefine = 
    "\n"
    "#define _VERTEX_";

  constexpr char *fragDefine =
    "\n"
    "#define _FRAGMENT_";

  vert.addSource(peamble)
    .addSource(shadowLevels)
    .addSource(vertDefine)
    .addFile("data\\TexturedTriangleShader.glsl");

  frag.addSource(peamble)
    .addSource(shadowLevels)
    .addSource(fragDefine)
    .addFile("data\\TexturedTriangleShader.glsl");

  CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({ vert, frag }));

  attachShaders({ vert, frag });

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  mProjectionMatrixUniform = uniformLocation("projectionMatrix");
  mShadowmapMatrixUniform = uniformLocation("shadowmapMatrix");
  mLightVectorUniform = uniformLocation("lightVector");

  setUniform(uniformLocation("textureData"), TextureLayer);
  setUniform(uniformLocation("shadowDepth"), ShadowDepth);
}

StandartShader & StandartShader::setProjection(const Matrix4 & mat)
{
  setUniform(mProjectionMatrixUniform, mat);
  return *this;
}

StandartShader & StandartShader::setShadowMatrix(const Magnum::Containers::ArrayView<const Magnum::Matrix4>& mat)
{
  setUniform(mShadowmapMatrixUniform, mat);
  return *this;
}

StandartShader & StandartShader::setLightVector(const Vector3 & light)
{
  setUniform(mLightVectorUniform, light);
  return *this;
}

StandartShader & StandartShader::setTexture(Texture2D & texture) {
  texture.bind(TextureLayer);
  return *this;
}

StandartShader & StandartShader::setShadowDepthTexture(Texture2D & shadowDepth)
{
  shadowDepth.bind(ShadowDepth);
  return *this;
}

ShadowShader::ShadowShader()
{
  MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

  Shader vert{ Version::GL330, Shader::Type::Vertex };
  Shader frag{ Version::GL330, Shader::Type::Fragment };

  vert.addFile("data\\ShadowPass.vert");
  frag.addFile("data\\ShadowPass.frag");

  CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({ vert, frag }));

  attachShaders({ vert, frag });

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  mUniformProjection = uniformLocation("projectionMatrix");
}

TexGenShader::TexGenShader()
{
  MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

  Shader vert{ Version::GL330, Shader::Type::Vertex };
  Shader frag{ Version::GL330, Shader::Type::Fragment };

  vert.addFile("data\\Test.vert");
  frag.addFile("data\\Test.frag");

  CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({ vert, frag }));

  attachShaders({ vert, frag });

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  setUniform(uniformLocation("inputTex0"), TextureLayer);
}
