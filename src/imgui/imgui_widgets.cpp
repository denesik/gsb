#include "imgui_widgets.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "..\imgui\imgui_internal.h"

namespace ImGui
{
  void Image(ImTextureID user_texture_id, const char* label, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
    const ImVec4& tint_col, const ImVec4& border_col)
  {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
      return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    if (border_col.w > 0.0f)
      bb.Max += ImVec2(2, 2);
    ItemSize(bb);
    if (!ItemAdd(bb, NULL))
      return;

    ImRect bb_text(bb.Max - label_size - ImVec2(2, 1), bb.Max - ImVec2(2, 1));

    if (border_col.w > 0.0f)
    {
      window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
      window->DrawList->AddImage(user_texture_id, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), uv0, uv1, GetColorU32(tint_col));
      RenderTextClipped(bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1),
        label, NULL, &label_size, style.ButtonTextAlign, &bb);
    }
    else
    {
      window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col));
      RenderTextClipped(bb_text.Min, bb_text.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb);
    }
  }

}