#pragma once

#include <array>

#include "../config/theme_constants.h"
#include "titlebar_button.h"

namespace gwingui {

namespace titlebar {

constexpr uint32_t kTitlebarButtonWidth = 46;
constexpr uint32_t kTitlebarButtonHeight = 29;

constexpr RECT kTitlebarButtonRect = { 0, 0, kTitlebarButtonWidth,
                                       kTitlebarButtonHeight };

constexpr RECT kIconRect = { 0, 0, 16, 16 };

enum TitlebarButtonIndex {
  TitlebarButtonIndexX,
  TitlebarButtonIndexMaximize,
  TitlebarButtonIndexMinimize,
};

std::vector<RECT> GetTitlebarButtonRects( const uint32_t window_width );

void DrawTitlebar( const HWND window_handle, const WPARAM wparam );

INT_PTR HandleHitTest( const HWND window_handle, const LPARAM lparam );

INT_PTR HandleLeftButtonDown( const int32_t hittest, const HWND window_handle );
void HandleLeftButtonUp( const HWND window_handle );

void HandleMouseLeave( const HWND window_handle );

}  // namespace titlebar

}  // namespace gwingui