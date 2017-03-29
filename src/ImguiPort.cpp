#include "ImguiPort.h"

#include "../imgui/imgui.h"
#include <Magnum/Context.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>
#include <Corrade/Containers/Array.h>
#include <Magnum/Image.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/TextureFormat.h>
#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Renderer.h>

using namespace Magnum;

void ImguiPort::Init()
{
  ImGuiIO& io = ImGui::GetIO();
  io.KeyMap[ImGuiKey_Tab] = ImGuiKey_Tab;
  io.KeyMap[ImGuiKey_LeftArrow] = ImGuiKey_LeftArrow;
  io.KeyMap[ImGuiKey_RightArrow] = ImGuiKey_RightArrow;
  io.KeyMap[ImGuiKey_UpArrow] = ImGuiKey_UpArrow;
  io.KeyMap[ImGuiKey_DownArrow] = ImGuiKey_DownArrow;
  io.KeyMap[ImGuiKey_PageUp] = ImGuiKey_PageUp;
  io.KeyMap[ImGuiKey_PageDown] = ImGuiKey_PageDown;
  io.KeyMap[ImGuiKey_Home] = ImGuiKey_Home;
  io.KeyMap[ImGuiKey_End] = ImGuiKey_End;
  io.KeyMap[ImGuiKey_Delete] = ImGuiKey_Delete;
  io.KeyMap[ImGuiKey_Backspace] = ImGuiKey_Backspace;
  io.KeyMap[ImGuiKey_Enter] = ImGuiKey_Enter;
  io.KeyMap[ImGuiKey_Escape] = ImGuiKey_Escape;
  io.KeyMap[ImGuiKey_A] = ImGuiKey_A;
  io.KeyMap[ImGuiKey_C] = ImGuiKey_C;
  io.KeyMap[ImGuiKey_V] = ImGuiKey_V;
  io.KeyMap[ImGuiKey_X] = ImGuiKey_X;
  io.KeyMap[ImGuiKey_Y] = ImGuiKey_Y;
  io.KeyMap[ImGuiKey_Z] = ImGuiKey_Z;

//   io.SetClipboardTextFn = ;
//   io.GetClipboardTextFn = ;

  mTimeline.start();
}

void ImguiPort::Load()
{
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  int pixel_size;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &pixel_size);
  io.Fonts->TexID = 0;

  mTexture.resize(2);

  ImageView2D image{ PixelFormat::RGBA, PixelType::UnsignedByte, { width, height },{ pixels, std::size_t(pixel_size * width * height) } };
  mTexture[0].setMagnificationFilter(Sampler::Filter::Linear)
    .setMinificationFilter(Sampler::Filter::Linear)
    .setStorage(0, TextureFormat::RGBA, image.size())
    .setImage(0, TextureFormat::RGBA, image);


  mAtlas.LoadDirectory("data/items");

  mTexture[1].setMagnificationFilter(Sampler::Filter::Linear)
    .setMinificationFilter(Sampler::Filter::Linear)
    .setStorage(0, TextureFormat::RGBA, mAtlas.Size());

  // Перед использованием setSubImage нужно залить текстуру данными с помощью setImage
  {
    std::vector<UnsignedByte> zero_data(mAtlas.Size().x() * mAtlas.Size().y() * 4);
    mTexture[1].setImage(0, TextureFormat::RGBA,
      ImageView2D(PixelFormat::RGBA, PixelType::UnsignedByte, mAtlas.Size(), { zero_data.data(), zero_data.size() }));
  }
  mAtlas.Fill(mTexture[1]);

  mesh.setPrimitive(MeshPrimitive::Triangles);
  mesh.addVertexBuffer(mVertexBuffer, 0, ImguiShader::Position{}, ImguiShader::TextureCoordinates{},
    ImguiShader::Color(ImguiShader::Color::Components::Four, ImguiShader::Color::DataType::UnsignedByte, ImguiShader::Color::DataOption::Normalized));
}

void ImguiPort::keyPressReleaseEvent(const Platform::Application::KeyEvent& event, bool value)
{
  ImGuiIO& io = ImGui::GetIO();

  const auto &mods = event.modifiers();
  if (mods & Platform::Application::KeyEvent::Modifier::Shift) io.KeyShift = value;
  if (mods & Platform::Application::KeyEvent::Modifier::Ctrl) io.KeyCtrl = value;
  if (mods & Platform::Application::KeyEvent::Modifier::Alt) io.KeyAlt = value;

  switch (event.key())
  {
  case Platform::Application::KeyEvent::Key::Tab:
    io.KeysDown[ImGuiKey_Tab] = value;
    break;
  case Platform::Application::KeyEvent::Key::Up:
    io.KeysDown[ImGuiKey_UpArrow] = value;
    break;
  case Platform::Application::KeyEvent::Key::Down:
    io.KeysDown[ImGuiKey_DownArrow] = value;
    break;
  case Platform::Application::KeyEvent::Key::Left:
    io.KeysDown[ImGuiKey_LeftArrow] = value;
    break;
  case Platform::Application::KeyEvent::Key::Right:
    io.KeysDown[ImGuiKey_RightArrow] = value;
    break;
  case Platform::Application::KeyEvent::Key::Home:
    io.KeysDown[ImGuiKey_Home] = value;
    break;
  case Platform::Application::KeyEvent::Key::End:
    io.KeysDown[ImGuiKey_End] = value;
    break;
  case Platform::Application::KeyEvent::Key::PageUp:
    io.KeysDown[ImGuiKey_PageUp] = value;
    break;
  case Platform::Application::KeyEvent::Key::PageDown:
    io.KeysDown[ImGuiKey_PageDown] = value;
    break;
  case Platform::Application::KeyEvent::Key::Enter:
    io.KeysDown[ImGuiKey_Enter] = value;
    break;
  case Platform::Application::KeyEvent::Key::Esc:
    io.KeysDown[ImGuiKey_Escape] = value;
    break;
  case Platform::Application::KeyEvent::Key::Backspace:
    io.KeysDown[ImGuiKey_Backspace] = value;
    break;
  case Platform::Application::KeyEvent::Key::Delete:
    io.KeysDown[ImGuiKey_Delete] = value;
    break;
  case Platform::Application::KeyEvent::Key::A:
    io.KeysDown[ImGuiKey_A] = value;
    break;
  case Platform::Application::KeyEvent::Key::C:
    io.KeysDown[ImGuiKey_C] = value;
    break;
  case Platform::Application::KeyEvent::Key::V:
    io.KeysDown[ImGuiKey_V] = value;
    break;
  case Platform::Application::KeyEvent::Key::X:
    io.KeysDown[ImGuiKey_X] = value;
    break;
  case Platform::Application::KeyEvent::Key::Y:
    io.KeysDown[ImGuiKey_Y] = value;
    break;
  case Platform::Application::KeyEvent::Key::Z:
    io.KeysDown[ImGuiKey_Z] = value;
    break;
  default:
    break;
  }
}

void ImguiPort::mousePressReleaseEvent(const Platform::Application::MouseEvent& event, bool value)
{
  switch (event.button())
  {
  case Magnum::Platform::Application::MouseEvent::Button::Left:
    mMousePressed[0] = value;
    break;
  case Magnum::Platform::Application::MouseEvent::Button::Right:
    mMousePressed[1] = value;
    break;
  case Magnum::Platform::Application::MouseEvent::Button::Middle:
    mMousePressed[2] = value;
    break;
  default:
    break;
  }
}

ImguiPort::ImguiPort()
{
  Init();
  Load();
}

void ImguiPort::NewFrame(const Vector2i &winSize, const Vector2i &viewportSize)
{
  mTimeline.nextFrame();

  ImGuiIO& io = ImGui::GetIO();

  // Setup display size (every frame to accommodate for window resizing)
  io.DisplaySize = ImVec2((float)winSize.x(), (float)winSize.y());
  io.DisplayFramebufferScale = ImVec2(winSize.x() > 0 ? ((float)viewportSize.x() / winSize.x()) : 0,
    winSize.y() > 0 ? ((float)viewportSize.y() / winSize.y()) : 0);

  io.DeltaTime = mTimeline.previousFrameDuration();
  io.MousePos = ImVec2(float(mMousePos.x()), float(mMousePos.y()));

  for (int i = 0; i < 3; i++)
  {
    io.MouseDown[i] = mMousePressed[i];    
  }

  io.MouseWheel = mMouseScroll;
  mMouseScroll = 0.0f;

  ImGui::NewFrame();
}

void ImguiPort::Draw()
{
  ImGui::Render();

  ImGuiIO& io = ImGui::GetIO();
  int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
  int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
  if (fb_width == 0 || fb_height == 0)
    return;

  auto draw_data = ImGui::GetDrawData();
  if (!draw_data)
    return;

  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  Renderer::enable(Renderer::Feature::Blending);
  Renderer::setBlendEquation(Magnum::Renderer::BlendEquation::Add, Magnum::Renderer::BlendEquation::Add);
  Renderer::setBlendFunction(Renderer::BlendFunction::SourceAlpha, Renderer::BlendFunction::OneMinusSourceAlpha);
  Renderer::disable(Renderer::Feature::FaceCulling);
  Renderer::disable(Renderer::Feature::DepthTest);
  Renderer::enable(Renderer::Feature::ScissorTest);

  const Matrix4 ortho_projection
  {
    { 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
    { 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
    { 0.0f,                  0.0f,                  -1.0f, 0.0f },
    { -1.0f,                  1.0f,                   0.0f, 1.0f },
  };
  mShader.setProjectMatrix(ortho_projection);
  

  for (int n = 0; n < draw_data->CmdListsCount; n++)
  {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    ImDrawIdx idx_buffer_offset = 0;
	const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;

    mVertexBuffer.setData({ cmd_list->VtxBuffer.Data, std::size_t(cmd_list->VtxBuffer.Size) }, BufferUsage::StreamDraw);
    mIndexBuffer.setData({ cmd_list->IdxBuffer.Data, std::size_t(cmd_list->IdxBuffer.Size) }, BufferUsage::StreamDraw);
    
    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
    {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

	  if((size_t)(intptr_t)pcmd->TextureId < 2)
		  mShader.setTexture(mTexture[(size_t)(intptr_t)pcmd->TextureId]);
	  /*else
	  {
		  glUseProgram(mShader.id());
		  glActiveTexture(GL_TEXTURE0);
		  glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
		  glEnable(GL_BLEND);
	  }*/
      Renderer::setScissor({ {(int)pcmd->ClipRect.x, fb_height - (int)(pcmd->ClipRect.w)}, {(int)(pcmd->ClipRect.z), fb_height - (int)(pcmd->ClipRect.y)} });

      mesh.setCount(pcmd->ElemCount);
      mesh.setIndexBuffer(mIndexBuffer, idx_buffer_offset * sizeof(ImDrawIdx), sizeof(ImDrawIdx) == 2 ? Mesh::IndexType::UnsignedShort : Mesh::IndexType::UnsignedInt);

      idx_buffer_offset += pcmd->ElemCount;

	  mesh.draw(mShader);
    }
  }

  Renderer::disable(Renderer::Feature::ScissorTest);
}

void ImguiPort::mousePressEvent(const Platform::Application::MouseEvent& event)
{
  mousePressReleaseEvent(event, true);
}

void ImguiPort::mouseReleaseEvent(const Platform::Application::MouseEvent& event)
{
  mousePressReleaseEvent(event, false);
}

void ImguiPort::mouseScrollEvent(const Platform::Application::MouseScrollEvent& event)
{
  mMouseScroll += event.offset().y();
}

void ImguiPort::mouseMoveEvent(const Platform::Application::MouseMoveEvent& event)
{
  mMousePos = event.position();
}

void ImguiPort::keyPressEvent(const Platform::Application::KeyEvent& event)
{
  keyPressReleaseEvent(event, true);
}

void ImguiPort::keyReleaseEvent(const Platform::Application::KeyEvent& event)
{
  keyPressReleaseEvent(event, false);
}

void ImguiPort::textInputEvent(const Platform::Application::TextInputEvent& event)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddInputCharactersUTF8(event.text().data());
}

const TextureAtlas & ImguiPort::Atlas() const
{
  return mAtlas;
}

ImguiShader::ImguiShader()
{
  const char *vertex_shader =
    "uniform mat4 ProjMtx;\n"
    "layout(location = 0) in vec2 Position;\n"
    "layout(location = 1) in vec2 UV;\n"
    "layout(location = 2) in vec4 Color;\n"
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "	Frag_UV = UV;\n"
    "	Frag_Color = Color;\n"
    "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const char* fragment_shader =
    "uniform sampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
    "}\n";

  MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

  Shader vert{ Version::GL330, Shader::Type::Vertex };
  Shader frag{ Version::GL330, Shader::Type::Fragment };

  vert.addSource({ vertex_shader });
  frag.addSource({ fragment_shader });

  CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({ vert, frag }));

  attachShaders({ vert, frag });

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  mProjMatrixUniform = uniformLocation("ProjMtx");

  setUniform(uniformLocation("Texture"), TextureLayer);
}
