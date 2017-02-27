#include "Game.h"
#include "../imgui/imgui.h"

#include <Magnum/Image.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Trade/ImageData.h>
#include "TextureAtlas.h"


Game::Game(const Arguments & arguments)
	: Platform::Application{ arguments, Configuration{}.setTitle("Magnum Textured Triangle Example").setWindowFlags(Configuration::WindowFlag::Resizable) }
{
  static const Vector2 triangle_data[] = {
    { -1.0f, -1.0f },{ 0.0f, 0.0f }, // низ лево
    { 1.0f, -1.0f },{ 1.0f, 0.0f }, // низ право
    { 1.0f, 1.0f },{ 1.0f, 1.0f }, // верх право
    { 1.0f, 1.0f },{ 1.0f, 1.0f }, // верх право
    { -1.0f, 1.0f },{ 0.0f, 1.0f }, // верх лево
    { -1.0f, -1.0f },{ 0.0f, 0.0f }, // низ лево
  };

  _buffer.setData(triangle_data, BufferUsage::StaticDraw);
  _mesh.setPrimitive(MeshPrimitive::Triangles)
    .setCount(6)
    .addVertexBuffer(_buffer, 0, TexturedTriangleShader::Position{}, TexturedTriangleShader::TextureCoordinates{});

  if (false)
  {
    PluginManager::Manager<Trade::AbstractImporter> manager{ MAGNUM_PLUGINS_IMPORTER_DIR };
    std::unique_ptr<Trade::AbstractImporter> importer = manager.loadAndInstantiate("TgaImporter");
    if (!importer) std::exit(1);

    if (!importer->openFile("data\\stone.tga"))
      std::exit(2);

    std::optional<Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);

    _texture.setWrapping(Sampler::Wrapping::ClampToEdge)
      .setMagnificationFilter(Sampler::Filter::Linear)
      .setMinificationFilter(Sampler::Filter::Linear)
      .setStorage(1, TextureFormat::RGB8, image->size())
      .setSubImage(0, {}, *image);
  }

  atlas.LoadDirectory("data");
}

void Game::drawEvent()
{
	defaultFramebuffer.clear(FramebufferClear::Color);

  _shader.setColor({ 1.0f, 0.7f, 0.7f })
    .setTexture(atlas.Texture());
  _mesh.draw(_shader);

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
}

void Game::keyReleaseEvent(KeyEvent& event)
{
  mImguiPort.keyReleaseEvent(event);
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
