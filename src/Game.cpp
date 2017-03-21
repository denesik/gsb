#include "Game.h"
#include "../imgui/imgui.h"

#include <Magnum/Image.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Trade/ImageData.h>
#include "TextureAtlas.h"
#include <Magnum/Renderer.h>
#include <Magnum/Math/Matrix4.h>
#include "TesselatorSolidBlock.h"
#include "DataBase.h"
#include "Sector.h"
#include "World.h"
#include "JsonDataBase.h"
#include "MapGenerator.h"
#include "IMapGenerator.h"
#include "MapLoader.h"
#include "tools/Brezenham3D.h"
#include "WorldGeneratorFlat.h"
#include <boost/optional.hpp>
#include <exception>
#include <IMapLoader.h>
#include "ChunkHelper.h"

Game::Game(const Arguments & arguments)
  : Platform::Application{ arguments, Configuration{}.setTitle("sge").setWindowFlags(Configuration::WindowFlag::Resizable) }
{
  test();

  auto t = std::uncaught_exceptions();

  atlas.LoadDirectory("data");
  mTexture.setWrapping(Sampler::Wrapping::ClampToEdge)
    .setMagnificationFilter(Sampler::Filter::Linear)
    .setMinificationFilter(Sampler::Filter::Linear);

  // Перед использованием setSubImage нужно залить текстуру данными с помощью setImage
  {
    std::vector<UnsignedByte> zero_data(atlas.Size().x() * atlas.Size().y() * 4);
    mTexture.setImage(0, TextureFormat::RGBA,
      ImageView2D(PixelFormat::RGBA, PixelType::UnsignedByte, atlas.Size(), { zero_data.data(), zero_data.size() }));
  }
  atlas.Fill(mTexture);

  mBlocksDataBase = std::make_unique<DataBase>(atlas, mImguiPort.Atlas());
  mBlocksDataBase->ApplyLoader(std::make_unique<JsonDataBase>("data/json"));

  mWorld = std::make_unique<World>(*mBlocksDataBase);

  //auto mapgen = std::make_unique<MapLoader>(std::make_unique<PrimitivaMountains>(*mBlocksDataBase, 1.f));
  //auto mapgen = std::make_unique<MapLoader>(std::make_unique<WorldGeneratorFlat>(*mBlocksDataBase));
  //mapgen->SetWorld(mWorld.get());

  //mWorld->SetLoader(std::move(mapgen));

  mDrawableArea = std::make_unique<DrawableArea>(*mWorld, SPos{});
  mUpdatableArea = std::make_unique<UpdatableArea>(*mWorld, SPos{}, 0);

  setSwapInterval(0);
  setMouseLocked(true);

  mCamera = std::make_unique<Camera>(mWorld->mPlayer);
  mCamera->SetViewport(defaultFramebuffer.viewport());
  mWorld->mPlayer.SetPos({ 0, 70, 0 });


  mTimeline.start();
}

void Game::drawEvent()
{
  mFpsCounter.Update();

  Renderer::enable(Renderer::Feature::DepthTest);
  Renderer::enable(Renderer::Feature::FaceCulling);

  defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth);

  mWorld->mPlayer.Move(mCameraVelocity * 0.006f);
  mWorld->mPlayer.Rotate(mCameraAngle * 0.003f);
  mWorld->mPlayer.Update();

  auto ray = mCamera->Ray({ static_cast<Float>(defaultFramebuffer.viewport().centerX()) ,
    static_cast<Float>(defaultFramebuffer.viewport().centerY()) });

  auto blocks = voxel_traversal(mWorld->mPlayer.Pos(), mWorld->mPlayer.Pos() + ray.normalized() * 100.0f);

  Vector3i picked;
  for (auto &i : blocks)
  {
    if (mWorld->GetBlockId(i) != 0)
    {
      picked = i;
      break;
    }
  }

  debugLines.addLine(picked, picked + Vector3i{ 1,0,0 }, { 1,1,1 });
  debugLines.addLine(picked, picked + Vector3i{ 0,1,0 }, { 1,1,1 });
  debugLines.addLine(picked, picked + Vector3i{ 0,0,1 }, { 1,1,1 });

  debugLines.addLine(picked + Vector3i{ 1,1,1 }, picked + Vector3i{ 0,1,1 }, { 1,1,1 });
  debugLines.addLine(picked + Vector3i{ 1,1,1 }, picked + Vector3i{ 1,0,1 }, { 1,1,1 });
  debugLines.addLine(picked + Vector3i{ 1,1,1 }, picked + Vector3i{ 1,1,0 }, { 1,1,1 });

  debugLines.addLine(mWorld->mPlayer.Pos(), mWorld->mPlayer.Pos() + ray * 1, { 1,1,1 });

  if (ImGui::IsMouseClicked(0))
  {
    auto p = mWorld->GetBlockDynamic(picked);
    if (p)
      mDrawModal = picked;
    else
      mDrawModal = {};
  }

  mShader.setTexture(mTexture);

  mDrawableArea->Draw(*mCamera, mShader);
  mWorld->Update();

  debugLines.draw(mCamera->Project() * mCamera->View());
  debugLines.reset();

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

    if (mDrawModal != WBPos{})
    {
      ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Selected");
      auto p = mWorld->GetBlockDynamic(mDrawModal);
      if (p)
        p->DrawGui(mTimeline);
      ImGui::End();
    }

	mWorld->GetWorldGenerator().DrawGui(mTimeline);

	if (ImGui::Button("wipe all"))
		mWorld->Wipe();

    mImguiPort.Draw();
  }

  swapBuffers();
  mTimeline.nextFrame();
  redraw();
}

void Game::viewportEvent(const Vector2i& size)
{
  defaultFramebuffer.setViewport({ {}, size });
  mCamera->SetViewport(defaultFramebuffer.viewport());
}

void Game::keyPressEvent(KeyEvent& event)
{
  mImguiPort.keyPressEvent(event);

  if (event.key() == KeyEvent::Key::A)
    mCameraVelocity.x() = -1000.0f * mTimeline.previousFrameDuration();
  if (event.key() == KeyEvent::Key::D)
    mCameraVelocity.x() = 1000.0f * mTimeline.previousFrameDuration();

  if (event.key() == KeyEvent::Key::W)
    mCameraVelocity.z() = -1000.0f * mTimeline.previousFrameDuration();

  if (event.key() == KeyEvent::Key::S)
    mCameraVelocity.z() = 1000.0f * mTimeline.previousFrameDuration();

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

  if (event.key() == KeyEvent::Key::Tab)
    centering = centering ? (setMouseLocked(false), false) : (setMouseLocked(true), true);

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
  if (centering)
  {
    mWorld->mPlayer.Rotate(Vector3(0.0f, -event.relativePosition().y() / 800.f, 0.0f));
    mWorld->mPlayer.Rotate(Vector3(-event.relativePosition().x() / 800.f, 0.0f, 0.0f));
  }
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
