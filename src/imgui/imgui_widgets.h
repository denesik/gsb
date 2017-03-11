#pragma once
#ifndef imgui_widgets_h__
#define imgui_widgets_h__

#include "../imgui/imgui.h"

namespace ImGui
{
  // TODO: Нормально посчитать bb для текста.
  void Image(ImTextureID user_texture_id, const char* label, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
    const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
}


#endif // imgui_widgets_h__
