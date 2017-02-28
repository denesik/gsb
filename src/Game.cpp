#include "Game.h"
#include "../imgui/imgui.h"

#include <Magnum/Image.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Trade/ImageData.h>
#include "TextureAtlas.h"
#include <Magnum/Renderer.h>
#include <Magnum/Math/Matrix4.h>
#include "../TesselatorSolidBlock.h"


Game::Game(const Arguments & arguments)
	: Platform::Application{ arguments, Configuration{}.setTitle("Magnum Textured Triangle Example").setWindowFlags(Configuration::WindowFlag::Resizable) }
{
  atlas.LoadDirectory("data");

  std::vector<TesselatorVertex> vertex_data;
  std::vector<UnsignedInt> index_data;

  UnsignedInt index = 0;
  TesselatorSolidBlock().PushBack(vertex_data, index_data, index);

  mVertexBuffer.setData(vertex_data, BufferUsage::StaticDraw);
  mIndexBuffer.setData(index_data, BufferUsage::StaticDraw);

  mMesh.setPrimitive(MeshPrimitive::Triangles);
  mMesh.addVertexBuffer(mVertexBuffer, 0, TexturedTriangleShader::Position{}, TexturedTriangleShader::TextureCoordinates{});
  mMesh.setIndexBuffer(mIndexBuffer, 0, Mesh::IndexType::UnsignedInt);
  mMesh.setCount(mIndexBuffer.size());

  mTimeline.start();
}

void Game::drawEvent()
{
  mTimeline.nextFrame();
  Renderer::enable(Renderer::Feature::DepthTest);
  //Renderer::enable(Renderer::Feature::FaceCulling);

  mProjection = Matrix4::perspectiveProjection(35.0_degf, Vector2{ defaultFramebuffer.viewport().size() }.aspectRatio(), 0.01f, 100.0f)
    //* Matrix4::translation(Vector3::zAxis(-10.0f))
    ;

	defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth);

  mShader.setColor({ 1.0f, 0.7f, 0.7f })
    .setTexture(atlas.Texture())
    .setProjection(mProjection * mView.inverted() * mModel);

  mMesh.draw(mShader);

  //if (false)
  {
    mImguiPort.NewFrame(windowSize(), defaultFramebuffer.viewport().size());

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
      static float f = 0.0f;
      ImGui::Text("Hello, world!");
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float*)&clear_color);
      if (ImGui::Button("Test Window")) show_test_window ^= 1;
      if (ImGui::Button("Another Window")) show_another_window ^= 1;
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
    {
      ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello");
      ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&show_test_window);
    }

    mImguiPort.Draw();
  }

	swapBuffers();

  redraw();
}

void Game::viewportEvent(const Vector2i& size)
{
  defaultFramebuffer.setViewport({ {}, size });
}

void Game::keyPressEvent(KeyEvent& event)
{
  mImguiPort.keyPressEvent(event);

  float val = 15.0f * mTimeline.previousFrameDuration();

  if (event.key() == KeyEvent::Key::A)
    mView = mView * Math::Matrix4<Float>::translation(Vector3::xAxis(-val));

  if (event.key() == KeyEvent::Key::D)
    mView = mView * Math::Matrix4<Float>::translation(Vector3::xAxis(val));

  if (event.key() == KeyEvent::Key::W)
    mView = mView * Math::Matrix4<Float>::translation(Vector3::zAxis(-val));

  if (event.key() == KeyEvent::Key::S)
    mView = mView * Math::Matrix4<Float>::translation(Vector3::zAxis(val));

  if (event.key() == KeyEvent::Key::Left)
    mView = mView * Math::Matrix4<Float>::rotationY(Rad(val));

  if (event.key() == KeyEvent::Key::Right)
    mView = mView * Math::Matrix4<Float>::rotationY(Rad(-val));

  if (event.key() == KeyEvent::Key::Up)
    mView = mView * Math::Matrix4<Float>::rotationX(Rad(val));

  if (event.key() == KeyEvent::Key::Down)
    mView = mView * Math::Matrix4<Float>::rotationX(Rad(-val));

  event.setAccepted();
}

void Game::keyReleaseEvent(KeyEvent& event)
{
  mImguiPort.keyReleaseEvent(event);

  event.setAccepted();
}

Game::~Game()
{
  ImGui::Shutdown();
}

void Game::mousePressEvent(MouseEvent& event)
{
  mImguiPort.mousePressEvent(event);
}

void Game::mouseReleaseEvent(MouseEvent& event)
{
  mImguiPort.mouseReleaseEvent(event);
}

void Game::mouseMoveEvent(MouseMoveEvent& event)
{
  mImguiPort.mouseMoveEvent(event);
}

void Game::mouseScrollEvent(MouseScrollEvent& event)
{
  mImguiPort.mouseScrollEvent(event);
}

void Game::textInputEvent(TextInputEvent& event)
{
  mImguiPort.textInputEvent(event);
  event.setAccepted();
}

void Game::textEditingEvent(TextEditingEvent& event)
{

}

MAGNUM_APPLICATION_MAIN(Game)
