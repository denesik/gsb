#pragma once

#include <Magnum/Platform/Sdl2Application.h>
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Framebuffer.h>
#include <Magnum/TextureArray.h>
#include <Magnum/Mesh.h>
#include <Magnum/Texture.h>
#include <Magnum/TextureFormat.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include "StandartShader.h"
#include "configure.h"
#include "ImguiPort.h"
#include "../imgui/imgui.h"
#include "TextureAtlas.h"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Timeline.h>
#include <memory>
#include "DrawableArea.h"
#include "DataBase.h"
#include "Sector.h"
#include "World.h"
#include "tools/FpsCounter.h"
#include "UpdatableArea.h"
#include "Camera.h"
#include "tools/DebugLines.h"

#include <Magnum/Timeline.h>
#include "../TextureGenerator.h"
#include "../GuiWindow.h"

using namespace Magnum;

class Game : public Platform::Application
{
public:
	explicit Game(const Arguments& arguments);
  ~Game();

	void drawEvent() override;

  void viewportEvent(const Vector2i& size) override;

  void keyPressEvent(KeyEvent& event) override;

  void keyReleaseEvent(KeyEvent& event) override;

  void mousePressEvent(MouseEvent& event) override;

  void mouseReleaseEvent(MouseEvent& event) override;

  void mouseMoveEvent(MouseMoveEvent& event) override;

  void mouseScrollEvent(MouseScrollEvent& event) override;

  void textInputEvent(TextInputEvent& event) override;

  void textEditingEvent(TextEditingEvent& event) override;

private:
  ImguiPort mImguiPort;

  bool show_test_window = false;
  bool show_another_window = false;
  ImVec4 clear_color = ImColor(114, 144, 154);

  TextureAtlas atlas;
  Magnum::Texture2D mTexture;


  StandartShader mShader;
  ShadowShader mShadowPass;

  Vector3 mCameraVelocity;
  float move_speed = 1000.f;

  Timeline mTimeline;

  WBPos mDrawModal;
  std::unique_ptr<GuiWindow> modalWindow;
  std::unique_ptr<GuiWindowPlayerInventory> inventoryWindow;
  std::unique_ptr<GuiWindowDb> dbWindow;
  std::unique_ptr<GuiWindowAutocraft> autocraftWindow;

  std::unique_ptr<DataBase> mBlocksDataBase;
  std::unique_ptr<World> mWorld;
//  std::unique_ptr<UpdatableArea> mUpdatableArea;
  std::unique_ptr<DrawableArea> mDrawableArea; 
  std::unique_ptr<SectorLoader> mSectorLoader;

  FpsCounter mFpsCounter;
  DebugLines debugLines;
  bool centering = true;

  std::unique_ptr<MovableOffseted> playerHead;
  std::unique_ptr<Camera> mCamera;
  std::unique_ptr<SunCamera> mSunCamera;
  Camera * mCurrentCamera;
  Movable mSun;

  Texture2DArray mShadowTextureArray;

  TextureGenerator test_texgen;
  TexGenShader test_texgenshader;
};

