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

static const int tmp_area_size = 0;

Game::Game(const Arguments & arguments)
  : Platform::Application{ arguments, Configuration{}.setTitle("sge").setWindowFlags(Configuration::WindowFlag::Resizable).setVersion(Magnum::Version::GL330) }
  , mShadowFramebuffer{ Range2Di{{}, {512, 512}} }
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

  //auto mapgen = std::make_unique<MapLoader>(std::make_unique<PrimitivaMountains>(*mBlocksDataBase, 1.f));
  //auto mapgen = std::make_unique<MapLoader>(std::make_unique<WorldGeneratorFlat>(*mBlocksDataBase));
  //mapgen->SetWorld(mWorld.get());

  //mWorld->SetLoader(std::move(mapgen));

  mDrawableArea = std::make_unique<DrawableArea>(*mWorld, SPos{}, tmp_area_size);
  mUpdatableArea = std::make_unique<UpdatableArea>(*mWorld, SPos{}, tmp_area_size);

  setSwapInterval(0);
  setMouseLocked(true);

  mCamera = std::make_unique<Camera>(mWorld->mPlayer, defaultFramebuffer.viewport());
  mSunCamera = std::make_unique<Camera>(mSun, Range2Di{ {},{ 512, 512 } }, Camera::Type::Ortho);
  mCurrentCamera = mCamera.get();

  mWorld->mPlayer.SetPos({ 0, 70, 0 });

  mShadowTexture.setImage(0, TextureFormat::DepthComponent, ImageView2D{ PixelFormat::DepthComponent, PixelType::Float,{ 512, 512 }, nullptr })
	  .setMaxLevel(0)
	  .setCompareFunction(Sampler::CompareFunction::LessOrEqual)
    .setCompareMode(Sampler::CompareMode::CompareRefToTexture)
    .setMinificationFilter(Sampler::Filter::Linear, Sampler::Mipmap::Base)
    .setMagnificationFilter(Sampler::Filter::Linear);

  mShadowFramebuffer.attachTexture(Framebuffer::BufferAttachment::Depth, mShadowTexture, 0)
	  .mapForDraw(Framebuffer::DrawAttachment::None)
	  .bind();

  CORRADE_INTERNAL_ASSERT(mShadowFramebuffer.checkStatus(FramebufferTarget::Draw) == Framebuffer::Status::Complete);

  mTimeline.start();
}

void Game::drawEvent()
{
  mFpsCounter.Update();

  Renderer::enable(Renderer::Feature::DepthTest);
  Renderer::enable(Renderer::Feature::FaceCulling);
  Renderer::setClearColor({ clear_color.x, clear_color.y, clear_color.z, clear_color.w });

  mWorld->mPlayer.Move(mCameraVelocity * 0.006f);
  mWorld->mPlayer.Rotate(mCameraAngle * 0.003f);
  mWorld->mPlayer.Update();

  auto spos = Vector3{ std::sin(mTimeline.previousFrameTime()) * 100, 111, std::cos(mTimeline.previousFrameTime()) * 100 };
  mSun.SetPos(spos);
  mSun.LookAt({});
  mSun.Update();

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

  debugLines.addLine(picked, picked + Vector3i{ 1,0,0 }, { 1,0,0 });
  debugLines.addLine(picked, picked + Vector3i{ 0,1,0 }, { 0,1,0 });
  debugLines.addLine(picked, picked + Vector3i{ 0,0,1 }, { 0,0,1 });

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

  //shadow pass
  mShadowFramebuffer.clear(FramebufferClear::Depth).bind();
  //Renderer::setColorMask(false, false, false, false);
  Renderer::setFaceCullingMode(Magnum::Renderer::PolygonFacing::Front);
  mDrawableArea->DrawShadowPass(*mSunCamera, mShadowPass);

  //auto & test_t = test_texgen.Generate(mShadowTexture);

  //forward pass
  //Renderer::setColorMask(true, true, true, true);
  defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth).bind();
  Renderer::setFaceCullingMode(Magnum::Renderer::PolygonFacing::Back);
  mShader.setTexture(mTexture);
  mShader.setShadowDepthTexture(mShadowTexture);
  mDrawableArea->Draw(*mCurrentCamera, *mSunCamera, mSun.Direction(), mShader);
  //debugLines.addLine(mSun.Pos(), mSun.Pos() + mSun.Direction()*100, { 1,1,0 });

  mWorld->Update();

  debugLines.draw(mCamera->Project() * mCamera->View());
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

      static int drawable_area_size = tmp_area_size;
      int das = drawable_area_size;
      ImGui::SliderInt("Drawable area size", &drawable_area_size, 0, 10);
      if (das != drawable_area_size)
        mDrawableArea->SetRadius(drawable_area_size);

      static int updatable_area_size = tmp_area_size;
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

  if (event.key() == KeyEvent::Key::F5)
    mCurrentCamera = (mCurrentCamera == mCamera.get()) ? mSunCamera.get() : mCamera.get();

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
