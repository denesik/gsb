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
#include "IMapGenerator.h"
#include "MapLoader.h"
#include "tools\Brezenham3D.h"
#include "WorldGeneratorFlat.h"
#include <boost/optional.hpp>
#include <exception>
#include <IMapLoader.h>
#include "ChunkHelper.h"
#include <Magnum/Version.h>

static const int tmp_area_size = 1;
#include "../GuiInventory.h"


Game::Game(const Arguments & arguments)
  : Platform::Application{ arguments, Configuration{}.setTitle("sge").setWindowFlags(Configuration::WindowFlag::Resizable).setVersion(Magnum::Version::GL330) }
  , test_texgen{ test_texgenshader, {200, 200} }
{
  test();

  auto t = std::uncaught_exceptions();

  atlas.LoadDirectory("data");
  mTexture.setWrapping(Sampler::Wrapping::ClampToEdge)
    .setMagnificationFilter(Sampler::Filter::Nearest)
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
  mWorld->mPlayer.Inventory()[10] = std::make_pair(mBlocksDataBase->ItemIdFromName("white bricks").value_or(0), 10);
  mWorld->mPlayer.Inventory()[11] = std::make_pair(mBlocksDataBase->ItemIdFromName("yellow bricks").value_or(0), 10);
  mWorld->mPlayer.Inventory()[12] = std::make_pair(mBlocksDataBase->ItemIdFromName("red bricks").value_or(0), 10);

  modalWindow = std::make_unique<GuiWindow>(*mBlocksDataBase, "Selected");
  inventoryWindow = std::make_unique<GuiWindowPlayerInventory>(mWorld->mPlayer, *mBlocksDataBase);

  mDrawableArea = std::make_unique<DrawableArea>(*mWorld);
  mWorld->GetWorldSectorObserver().attach(*mDrawableArea);
  mSectorLoader = std::make_unique<SectorLoader>(*mWorld);

  mDrawableArea->SetPos(SPos{});
  mSectorLoader->SetPos(SPos{});

  mDrawableArea->SetRadius(tmp_area_size);
  mSectorLoader->SetRadius(tmp_area_size);

  //mUpdatableArea = std::make_unique<UpdatableArea>(*mWorld, SPos{}, tmp_area_size);

  setSwapInterval(0);
  setMouseLocked(true);

  playerHead = std::make_unique<MovableOffseted>(mWorld->mPlayer, Vector3{ 0, 1.8f, 0 });
  mCamera = std::make_unique<Camera>(*playerHead, defaultFramebuffer.viewport());
  mWorld->mPlayer.SetPos({ 0, 400, 0 });

  mShadowTextureArray.setImage(0, TextureFormat::DepthComponent, ImageView3D{ PixelFormat::DepthComponent, PixelType::Float,{ 1024, 1024, Int(StandartShader::ShadowMapLevels) }, nullptr })
    .setMaxLevel(0)
    .setCompareFunction(Sampler::CompareFunction::LessOrEqual)
    .setCompareMode(Sampler::CompareMode::CompareRefToTexture)
    .setMinificationFilter(Sampler::Filter::Linear, Sampler::Mipmap::Base)
    .setMagnificationFilter(Sampler::Filter::Linear);

  mSunCamera = std::make_unique<SunCamera>(mSun, Range2Di{ {},{ 1024, 1024 } }, Camera::Type::Ortho, mShadowTextureArray);
  mCurrentCamera = mCamera.get();

  mWorld->mPlayer.SetPos({ 0, 70, 0 });

  mTimeline.start();
}

void Game::drawEvent()
{
  mFpsCounter.Update();

  Renderer::enable(Renderer::Feature::DepthTest);
  Renderer::enable(Renderer::Feature::FaceCulling);
  Renderer::setClearColor({ clear_color.x, clear_color.y, clear_color.z, clear_color.w });

  if(mCameraVelocity.length() > 0)
    mWorld->mPlayer.MoveRelative(mCameraVelocity * 0.006f);
  mWorld->mPlayer.Rotate(mCameraAngle * 0.003f);
  mWorld->mPlayer.Update(mTimeline);

  auto p_sec = cs::WtoS(mWorld->mPlayer.Pos());
  p_sec.y() = 0;

  mDrawableArea->SetPos(p_sec);
  mSectorLoader->SetPos(p_sec);

  auto ray = mCamera->Ray({ static_cast<Float>(defaultFramebuffer.viewport().centerX()) ,
    static_cast<Float>(defaultFramebuffer.viewport().centerY()) }).normalized();

  auto blocks = voxel_traversal(playerHead->Pos(), playerHead->Pos() + ray * 100.0f);
  auto selId = inventoryWindow->HotbarSelection();
  auto &selItem = mBlocksDataBase->GetItem(selId);

  Vector3i prepicked;
  Vector3i picked;
  for (auto &i : blocks)
  {
    auto selid = mWorld->GetBlockId(i);
    if (selid != 0 && selid != 1)
    {
      picked = i;
      break;
    }

    prepicked = i;
  }

  picked = (selId && selItem->GetBlock()) ? prepicked : picked;

  debugLines.addLine(picked, picked + Vector3i{ 1,0,0 }, { 1,0,0 });
  debugLines.addLine(picked, picked + Vector3i{ 0,1,0 }, { 0,1,0 });
  debugLines.addLine(picked, picked + Vector3i{ 0,0,1 }, { 0,0,1 });

  debugLines.addLine(picked + Vector3i{ 1,1,1 }, picked + Vector3i{ 0,1,1 }, { 1,1,1 });
  debugLines.addLine(picked + Vector3i{ 1,1,1 }, picked + Vector3i{ 1,0,1 }, { 1,1,1 });
  debugLines.addLine(picked + Vector3i{ 1,1,1 }, picked + Vector3i{ 1,1,0 }, { 1,1,1 });

  debugLines.addLine(playerHead->Pos(), playerHead->Pos() + ray * 100.0f, { 1,1,1 });

  if (centering && ImGui::IsMouseClicked(1) && !ImGui::IsAnyItemHovered())
  {
    auto p = mWorld->GetBlockDynamic(picked);
    if (p)
    {
      modalWindow->Reset();
      modalWindow->AddGui(*p);
      modalWindow->Open();
    }
    else
    {
      modalWindow->Reset();
      modalWindow->Close();
    }
  }

   static float pressedT = 0;
   if (centering && ImGui::IsMouseDown(0) && !ImGui::IsAnyItemHovered())
   {
     pressedT -= mTimeline.previousFrameDuration();
     if (pressedT <= 0)
     {
       if (selId && selItem->GetBlock())
         mWorld->CreateBlock(picked, selItem->GetBlock());
       else
         mWorld->CreateBlock(picked, 1);
       pressedT = 0.3f;
     }
   }
   else
   {
     pressedT = 0;
   }

  auto sunref = mWorld->mPlayer.Pos(); // Vector3(picked); // 
  auto spos = sunref + Vector3{ std::sin(mTimeline.previousFrameTime() / 10.f) * 100, 111, std::cos(mTimeline.previousFrameTime() / 10.f) * 100 };
  mSun.SetPos(spos);
  mSun.LookAt(sunref);
  mSun.Update(mTimeline);

  //shadow pass
  mSunCamera->Draw(*mDrawableArea, mShadowPass);

  //forward pass
  Renderer::setColorMask(true, true, true, true);
  defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth).bind();
  Renderer::setFaceCullingMode(Magnum::Renderer::PolygonFacing::Back);
  mShader.setTexture(mTexture);
  mShader.setShadowDepthTexture(mShadowTextureArray);
  mDrawableArea->Draw(*mCurrentCamera, *mSunCamera, mSun.Direction(), mShader);

//  mDrawableArea->SetPos(cs::WtoS(mWorld->mPlayer.Pos()));
//  mUpdatableArea->SetPos(cs::WtoS(mWorld->mPlayer.Pos()));

  mWorld->Update();

  debugLines.draw(mCurrentCamera->Project() * mCurrentCamera->View());
  debugLines.reset();

  //if (false)
  {
    mImguiPort.NewFrame(windowSize(), defaultFramebuffer.viewport().size());

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
      //ImGui::Image(ImTextureID(mShadowTexture.id()), {200,200});

      ImGui::ColorEdit3("clear color", (float*)&clear_color);
      if (ImGui::Button("Test Window")) show_test_window ^= 1;
      if (ImGui::Button("Another Window")) show_another_window ^= 1;
      ImGui::Text("fps: %i; max: %i; min: %i; long frame: %i%%",
        mFpsCounter.GetCount(), mFpsCounter.GetMaxFps(), mFpsCounter.GetMinFps(), mFpsCounter.GetPercentLongFrame());
      ImGui::Text("player pos %g %g %g",
        mWorld->mPlayer.Pos().x(), mWorld->mPlayer.Pos().y(), mWorld->mPlayer.Pos().z());

      ImGui::PlotLines("", &mFpsCounter.GetFramesLength()[0], 100, mFpsCounter.GetFramesLengthCurrent(), "", 0, 0.018f, { 100,50 });

      ImGui::SliderFloat("Movement speed", &move_speed, 500, 10000);

      static int drawable_area_size = tmp_area_size;
      int das = drawable_area_size;
      ImGui::SliderInt("Drawable area size", &drawable_area_size, 0, 15);
      if (das != drawable_area_size)
        mDrawableArea->SetRadius(drawable_area_size);

      static int updatable_area_size = tmp_area_size;
      int uas = updatable_area_size;
      ImGui::SliderInt("Updatable area size", &updatable_area_size, 0, 15);
      if (uas != updatable_area_size)
        mSectorLoader->SetRadius(updatable_area_size);

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

    inventoryWindow->Draw(mTimeline);
    modalWindow->Draw(mTimeline);

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
    mCameraVelocity.x() = -move_speed * mTimeline.previousFrameDuration();
  if (event.key() == KeyEvent::Key::D)
    mCameraVelocity.x() = move_speed * mTimeline.previousFrameDuration();

  if (event.key() == KeyEvent::Key::W)
    mCameraVelocity.z() = -move_speed * mTimeline.previousFrameDuration();

  if (event.key() == KeyEvent::Key::S)
    mCameraVelocity.z() = move_speed * mTimeline.previousFrameDuration();

  if (event.key() == KeyEvent::Key::Space)
    mWorld->mPlayer.SetAcceleration({0, 6, 0});

  if (event.key() == KeyEvent::Key::F5)
    mCurrentCamera = (mCurrentCamera == mCamera.get()) ? mSunCamera.get() : mCamera.get();

  if (event.key() == KeyEvent::Key::E)
    inventoryWindow->Toggle();

  if (event.key() == KeyEvent::Key::F6)
    mSunCamera->SwitchLayerDebug();
    
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
