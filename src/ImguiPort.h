#pragma once
#ifndef ImguiPort_h__
#define ImguiPort_h__

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Texture.h>
#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Timeline.h>
#include <Magnum/Attribute.h>
#include "TextureAtlas.h"
#include <vector>

using namespace Magnum;

class ImguiShader : public AbstractShaderProgram
{
public:
  typedef Attribute<0, Vector2> Position;
  typedef Attribute<1, Vector2> TextureCoordinates;
  typedef Attribute<2, Vector4> Color;

  ImguiShader();

  ImguiShader& setProjectMatrix(const Matrix4& matrix)
  {
    setUniform(mProjMatrixUniform, matrix);
    return *this;
  }

  ImguiShader& setTexture(Texture2D& texture)
  {
    texture.bind(TextureLayer);
    return *this;
  }

private:
  enum : Int { TextureLayer = 0 };

  Int mProjMatrixUniform;
};


class ImguiPort
{
public:
  ImguiPort();

  void NewFrame(const Vector2i &winSize, const Vector2i &viewportSize);

  void Draw();

  void mousePressEvent(const Platform::Application::MouseEvent& event);

  void mouseReleaseEvent(const Platform::Application::MouseEvent& event);

  void mouseScrollEvent(const Platform::Application::MouseScrollEvent& event);

  void mouseMoveEvent(const Platform::Application::MouseMoveEvent& event);

  void keyPressEvent(const Platform::Application::KeyEvent& event);

  void keyReleaseEvent(const Platform::Application::KeyEvent& event);

  void textInputEvent(const Platform::Application::TextInputEvent& event);

  const TextureAtlas &Atlas() const;

private:
  std::vector<Texture2D> mTexture;
  ImguiShader mShader;
  Buffer mVertexBuffer;
  Buffer mIndexBuffer;
  Timeline mTimeline;

  bool mMousePressed[3] = { false, false, false };
  float mMouseScroll = 0.0f;
  Vector2i mMousePos;

  Mesh mesh;

  TextureAtlas mAtlas;

private:
  void Init();
  void Load();

  void keyPressReleaseEvent(const Platform::Application::KeyEvent& event, bool value);
  void mousePressReleaseEvent(const Platform::Application::MouseEvent& event, bool value);
};




#endif // ImguiPort_h__
