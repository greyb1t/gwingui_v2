#pragma once

namespace gwingui {

namespace theme {

void InitializeControls( const HWND window_handle );

bool EnableVisualStyles();

POINT GetCenterPosition( const RECT& rect, const SIZE text_size );

// On a listcontrol, a common issue is drawing the border as well as when it is
// disabled. This function handles that and draws two borders, inner and outer.
// Also if a scrollbar exists, it is kept and not overdrawn.
LRESULT HandleListNcPaint( const HWND list_handle,
                           const uint32_t message,
                           const WPARAM wparam,
                           const LPARAM lpara,
                           const COLORREF border_color,
                           const COLORREF border_color_disabled,
                           const COLORREF inner_border_color,
                           const COLORREF inner_border_color_disabled );

}  // namespace theme

}  // namespace gwingui