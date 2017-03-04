#include "Game.h"
#include "../imgui/imgui.h"

#include <Magnum/Image.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Trade/ImageData.h>
#include "TextureAtlas.h"
#include <Magnum/Renderer.h>
#include <Magnum/Math/Matrix4.h>
#include "TesselatorSolidBlock.h"
#include "BlocksDataBase.h"
#include "Sector.h"
#include "World.h"
#include "JsonDataBase.h"
#include "MapGenerator.h"
#include "IMapGenerator.h"
#include "MapLoader.h"

Game::Game(const Arguments & arguments)
	: Platform::Application{ arguments, Configuration{}.setTitle("Magnum Textured Triangle Example").setWindowFlags(Configuration::WindowFlag::Resizable) }
{
  atlas.LoadDirectory("data");

  mBlocksDataBase = std::make_unique<BlocksDataBase>(atlas);
  mBlocksDataBase->ApplyLoader(std::make_unique<JsonDataBase>("data/json"));

  mWorld = std::make_unique<World>(*mBlocksDataBase);
  auto mapgen = std::make_unique<MapLoader>(std::make_unique<PrimitivaMountains>(*mBlocksDataBase, 1.f));
  mapgen->SetWorld(mWorld.get());
  mWorld->SetLoader(std::move(mapgen));

  mDrawableArea = std::make_unique<DrawableArea>(*mWorld, SPos{});
  mUpdatableArea = std::make_unique<UpdatableArea>(mWorld->GetUpdatableSectors(), SPos{}, 1);

  setSwapInterval(0);


  mCamera.SetResolution(defaultFramebuffer.viewport().size());
  mCamera.Move({0, 40, 0});
}

void Game::drawEvent()
{
  mFpsCounter.Update();

  Renderer::enable(Renderer::Feature::DepthTest);
  Renderer::enable(Renderer::Feature::FaceCulling);

	defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth);

  mCamera.Move(mCameraVelocity * 0.003f);
  mCamera.Rotate(mCameraAngle * 0.003f);

  mShader.setColor({ 1.0f, 0.7f, 0.7f })
    .setTexture(atlas.Texture());

  mDrawableArea->Draw(mCamera, mShader);
  mWorld->GetUpdatableSectors().Update();

  //if (false)
  {
    mImguiPort.NewFrame(windowSize(), defaultFramebuffer.viewport().size());

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
      ImGui::ColorEdit3("clear color", (float*)&clear_color);
      if (ImGui::Button("Test Window")) show_test_window ^= 1;
      if (ImGui::Button("Another Window")) show_another_window ^= 1;
      ImGui::Text("fps: %i; max: %i; min: %i; long frame: %i%%", 
        mFpsCounter.GetCount(), mFpsCounter.GetMaxFps(), mFpsCounter.GetMinFps(), mFpsCounter.GetPercentLongFrame());
      
      static int drawable_area_size = 5;
      int das = drawable_area_size;
      ImGui::SliderInt("Drawable area size", &drawable_area_size, 0, 10);
      if (das != drawable_area_size)
        mDrawableArea->SetRadius(drawable_area_size);

      static int updatable_area_size = 5;
      int uas = updatable_area_size;
      ImGui::SliderInt("Updatable area size", &updatable_area_size, 0, 10);
      if (uas != updatable_area_size)
        mUpdatableArea->SetRadius(updatable_area_size);
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
  mCamera.SetResolution(size);
}

void Game::keyPressEvent(KeyEvent& event)
{
  mImguiPort.keyPressEvent(event);

  if (event.key() == KeyEvent::Key::A)
    mCameraVelocity.x() = -1.0f;

  if (event.key() == KeyEvent::Key::D)
    mCameraVelocity.x() = 1.0f;

  if (event.key() == KeyEvent::Key::W)
    mCameraVelocity.z() = -1.0f;

  if (event.key() == KeyEvent::Key::S)
    mCameraVelocity.z() = 1.0f;

  if (event.key() == KeyEvent::Key::Left)
    mCameraAngle.x() = 1.0f;

  if (event.key() == KeyEvent::Key::Right)
    mCameraAngle.x() = -1.0f;

  if (event.key() == KeyEvent::Key::Up)
    mCameraAngle.y() = 1.0f;

  if (event.key() == KeyEvent::Key::Down)
    mCameraAngle.y() = -1.0f;

  event.setAccepted();
}

void Game::keyReleaseEvent(KeyEvent& event)
{
  mImguiPort.keyReleaseEvent(event);

  if (event.key() == KeyEvent::Key::A)
    mCameraVelocity.x() = 0.0f;

  if (event.key() == KeyEvent::Key::D)
    mCameraVelocity.x() = 0.0f;

  if (event.key() == KeyEvent::Key::W)
    mCameraVelocity.z() = 0.0f;

  if (event.key() == KeyEvent::Key::S)
    mCameraVelocity.z() = 0.0f;

  if (event.key() == KeyEvent::Key::Left)
    mCameraAngle.x() = 0.0f;

  if (event.key() == KeyEvent::Key::Right)
    mCameraAngle.x() = 0.0f;

  if (event.key() == KeyEvent::Key::Up)
    mCameraAngle.y() = 0.0f;

  if (event.key() == KeyEvent::Key::Down)
    mCameraAngle.y() = 0.0f;

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
